#pragma once
#include <onnxruntime_cxx_api.h>
#include <string>
#include <vector>
#include <unordered_map>

class ChatModel {
public:
    ChatModel(const std::wstring& model_path);
    ~ChatModel();

    std::vector<std::string> GetInputNames() const;
    std::vector<int64_t> GetInputShape(size_t index) const;

    // 多輸入推理：key=輸入名，value=float資料
    // input_shapes_map 紀錄每個輸入的形狀
    std::vector<float> Infer(const std::unordered_map<std::string, std::vector<int64_t>>& inputs,
                            const std::unordered_map<std::string, std::vector<int64_t>>& input_shapes_map);

private:
    Ort::Env env;
    Ort::SessionOptions session_options;
    Ort::Session session;
    Ort::AllocatorWithDefaultOptions allocator;

    std::vector<std::string> input_names;
    std::vector<std::vector<int64_t>> input_shapes;
    std::unordered_map<std::string, size_t> input_names_index;
};
