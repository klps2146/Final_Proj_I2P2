#include "ChatModel.hpp"
#include <iostream>
#include <sstream>
#include <vector>

ChatModel::ChatModel(const std::wstring& model_path)
    : env(ORT_LOGGING_LEVEL_WARNING, "GPT2"), session_options(), session(nullptr)
{
    session_options.SetIntraOpNumThreads(1);
    session_options.SetGraphOptimizationLevel(GraphOptimizationLevel::ORT_ENABLE_EXTENDED);

    session = Ort::Session(env, model_path.c_str(), session_options);

    InitVocab();
}

void ChatModel::InitVocab() {
    // 你必須用 GPT2 的 vocab 或 BPE tokenizer，這裡示範簡單字典
    token2id = {
        {"hello", 15496},
        {"world", 995},
        {"I", 40},
        {"am", 617},
        {"a", 257},
        {"bot", 8007},
        {"<pad>", 0}
    };
    for (auto& [k,v] : token2id) {
        id2token[v] = k;
    }
}

std::vector<int64_t> ChatModel::Tokenize(const std::string& text) {
    std::vector<int64_t> tokens;
    std::istringstream iss(text);
    std::string word;
    while (iss >> word) {
        if (token2id.find(word) != token2id.end())
            tokens.push_back(token2id[word]);
        else
            tokens.push_back(token2id["<pad>"]); // 未知字用 <pad>
    }
    return tokens;
}

std::string ChatModel::Detokenize(const std::vector<int64_t>& tokens) {
    std::string result;
    for (auto t : tokens) {
        if (id2token.find(t) != id2token.end())
            result += id2token[t] + " ";
        else
            result += "<unk> ";
    }
    return result;
}

std::vector<int64_t> ChatModel::Infer(const std::vector<int64_t>& input_ids) {
    Ort::AllocatorWithDefaultOptions allocator;
    Ort::MemoryInfo memory_info = Ort::MemoryInfo::CreateCpu(OrtArenaAllocator, OrtMemTypeDefault);

    // 取得輸入數量
    size_t input_count = session.GetInputCount();
    std::vector<const char*> input_names;
    std::vector<Ort::AllocatedStringPtr> input_name_ptrs;  // 儲存智慧指標，避免指標失效
    std::vector<Ort::Value> input_tensors;

    for (size_t i = 0; i < input_count; ++i) {
        // 取得輸入名稱並存智慧指標
        auto name_ptr = session.GetInputNameAllocated(i, allocator);
        input_names.push_back(name_ptr.get());
        input_name_ptrs.push_back(std::move(name_ptr));

        // 取得該輸入形狀
        auto type_info = session.GetInputTypeInfo(i);
        auto tensor_info = type_info.GetTensorTypeAndShapeInfo();
        std::vector<int64_t> input_shape = tensor_info.GetShape();

        // 依輸入索引建立 tensor，假設第一個是 input_ids，第二個是 attention_mask
        if (i == 0) {
            // input_ids shape [1, seq_len]
            input_shape[0] = 1;
            input_shape[1] = (int64_t)input_ids.size();

            input_tensors.emplace_back(Ort::Value::CreateTensor<int64_t>(
                memory_info,
                const_cast<int64_t*>(input_ids.data()),
                input_ids.size(),
                input_shape.data(),
                input_shape.size()
            ));
        }
        else if (i == 1) {
            // attention_mask shape同input_ids，值全1
            std::vector<int64_t> attention_mask(input_ids.size(), 1);
            input_shape[0] = 1;
            input_shape[1] = (int64_t)attention_mask.size();

            input_tensors.emplace_back(Ort::Value::CreateTensor<int64_t>(
                memory_info,
                attention_mask.data(),
                attention_mask.size(),
                input_shape.data(),
                input_shape.size()
            ));
        }
        else {
            // 其他輸入如果有，先塞零，視模型需求調整
            size_t total_len = 1;
            for (auto d : input_shape) total_len *= d;
            std::vector<float> zeros(total_len, 0.0f);

            input_tensors.emplace_back(Ort::Value::CreateTensor<float>(
                memory_info,
                zeros.data(),
                zeros.size(),
                input_shape.data(),
                input_shape.size()
            ));
        }
    }

    // 取得輸出名稱
    size_t output_count = session.GetOutputCount();
    std::vector<const char*> output_names;
    std::vector<Ort::AllocatedStringPtr> output_name_ptrs;
    for (size_t i = 0; i < output_count; ++i) {
        auto name_ptr = session.GetOutputNameAllocated(i, allocator);
        output_names.push_back(name_ptr.get());
        output_name_ptrs.push_back(std::move(name_ptr));
    }

    // 呼叫推理
    auto output_tensors = session.Run(Ort::RunOptions{nullptr},
                                      input_names.data(),
                                      input_tensors.data(),
                                      input_count,
                                      output_names.data(),
                                      output_count);

    // 這裡假設第一個輸出是 token ids (int64_t)，依模型調整
    int64_t* output_data = output_tensors[0].GetTensorMutableData<int64_t>();
    size_t output_len = output_tensors[0].GetTensorTypeAndShapeInfo().GetElementCount();

    return std::vector<int64_t>(output_data, output_data + output_len);
}
