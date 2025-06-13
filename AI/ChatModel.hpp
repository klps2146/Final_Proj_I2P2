#pragma once
#include <onnxruntime_cxx_api.h>
#include <string>
#include <vector>
#include <unordered_map>

class ChatModel {
public:
    explicit ChatModel(const std::wstring& model_path);

    std::vector<int64_t> Tokenize(const std::string& text);
    std::string Detokenize(const std::vector<int64_t>& tokens);

    std::vector<int64_t> Infer(const std::vector<int64_t>& input_ids);

private:
    void InitVocab();

    Ort::Env env;
    Ort::SessionOptions session_options;
    Ort::Session session;

    std::unordered_map<std::string, int64_t> token2id;
    std::unordered_map<int64_t, std::string> id2token;
};
