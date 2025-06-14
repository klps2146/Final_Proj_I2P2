#include "ChatModel.hpp"
#include <llama.h>
#include <cstdio>
#include <cstring>
#include <cstdlib>
#include <iostream>
#include <vector>
#include <functional>

static llama_model* model = nullptr;
static llama_context* ctx = nullptr;
static llama_sampler* smpl = nullptr;
static const llama_vocab* vocab = nullptr;
static std::vector<llama_chat_message> messages;
static std::vector<char> formatted;
static int prev_len = 0;

bool InitLlamaChat(const std::string& modelPath, int contextLength, int gpuLayers) {
    llama_log_set([](enum ggml_log_level level, const char * text, void *) {
        if (level >= GGML_LOG_LEVEL_ERROR)
            fprintf(stderr, "%s", text);
    }, nullptr);

    ggml_backend_load_all();

    llama_model_params model_params = llama_model_default_params();
    model_params.n_gpu_layers = gpuLayers;

    model = llama_model_load_from_file(modelPath.c_str(), model_params);
    if (!model) {
        fprintf(stderr, "Error loading model\n");
        return false;
    }

    vocab = llama_model_get_vocab(model);

    llama_context_params ctx_params = llama_context_default_params();
    ctx_params.n_ctx = contextLength;
    ctx_params.n_batch = contextLength;

    ctx = llama_init_from_model(model, ctx_params);
    if (!ctx) {
        fprintf(stderr, "Error creating context\n");
        return false;
    }

    smpl = llama_sampler_chain_init(llama_sampler_chain_default_params());
    llama_sampler_chain_add(smpl, llama_sampler_init_min_p(0.05f, 1));
    llama_sampler_chain_add(smpl, llama_sampler_init_temp(0.8f));
    llama_sampler_chain_add(smpl, llama_sampler_init_dist(LLAMA_DEFAULT_SEED));

    formatted.resize(contextLength);

    return true;
}

void QueryLlamaStream(const std::string& userInput, const std::function<void(const std::string&)>& onToken, int maxTokens) {
    const char* tmpl = llama_model_chat_template(model, nullptr);
    messages.push_back({"user", strdup(userInput.c_str())});

    int new_len = llama_chat_apply_template(tmpl, messages.data(), messages.size(), true, formatted.data(), formatted.size());
    if (new_len > (int)formatted.size()) {
        formatted.resize(new_len);
        new_len = llama_chat_apply_template(tmpl, messages.data(), messages.size(), true, formatted.data(), formatted.size());
    }
    if (new_len < 0) {
        onToken("[ERROR: template failed]");
        return;
    }

    std::string prompt(formatted.begin() + prev_len, formatted.begin() + new_len);
    const bool is_first = llama_memory_seq_pos_max(llama_get_memory(ctx), 0) == 0;

    int n_prompt_tokens = -llama_tokenize(vocab, prompt.c_str(), prompt.size(), nullptr, 0, is_first, true);
    std::vector<llama_token> prompt_tokens(n_prompt_tokens);
    if (llama_tokenize(vocab, prompt.c_str(), prompt.size(), prompt_tokens.data(), prompt_tokens.size(), is_first, true) < 0) {
        onToken("[ERROR: tokenization failed]");
        return;
    }

    llama_batch batch = llama_batch_get_one(prompt_tokens.data(), prompt_tokens.size());
    llama_token new_token_id;
    std::string response;

    int token_count = 0;  // 計數產生幾個 token

    while (true) {
        int n_ctx = llama_n_ctx(ctx);
        int n_ctx_used = llama_memory_seq_pos_max(llama_get_memory(ctx), 0);
        if (n_ctx_used + batch.n_tokens > n_ctx) break;
        if (llama_decode(ctx, batch)) break;

        new_token_id = llama_sampler_sample(smpl, ctx, -1);
        if (llama_vocab_is_eog(vocab, new_token_id)) break;

        char buf[256];
        int n = llama_token_to_piece(vocab, new_token_id, buf, sizeof(buf), 0, true);
        if (n < 0) break;

        std::string piece(buf, n);
        response += piece;

        onToken(piece);
        token_count++;

        // 超過限制
        if (maxTokens > 0 && token_count >= maxTokens) break;

        batch = llama_batch_get_one(&new_token_id, 1);
    }

    messages.push_back({"assistant", strdup(response.c_str())});
    prev_len = llama_chat_apply_template(tmpl, messages.data(), messages.size(), false, nullptr, 0);
}


// void QueryLlamaStream(const std::string& userInput, const std::function<void(const std::string&)>& onToken) {
//     const char* tmpl = llama_model_chat_template(model, nullptr);
//     messages.push_back({"user", strdup(userInput.c_str())});

//     int new_len = llama_chat_apply_template(tmpl, messages.data(), messages.size(), true, formatted.data(), formatted.size());
//     if (new_len > (int)formatted.size()) {
//         formatted.resize(new_len);
//         new_len = llama_chat_apply_template(tmpl, messages.data(), messages.size(), true, formatted.data(), formatted.size());
//     }
//     if (new_len < 0) {
//         onToken("[ERROR: template failed]");
//         return;
//     }

//     std::string prompt(formatted.begin() + prev_len, formatted.begin() + new_len);
//     const bool is_first = llama_memory_seq_pos_max(llama_get_memory(ctx), 0) == 0;

//     int n_prompt_tokens = -llama_tokenize(vocab, prompt.c_str(), prompt.size(), nullptr, 0, is_first, true);
//     std::vector<llama_token> prompt_tokens(n_prompt_tokens);
//     if (llama_tokenize(vocab, prompt.c_str(), prompt.size(), prompt_tokens.data(), prompt_tokens.size(), is_first, true) < 0) {
//         onToken("[ERROR: tokenization failed]");
//         return;
//     }

//     llama_batch batch = llama_batch_get_one(prompt_tokens.data(), prompt_tokens.size());
//     llama_token new_token_id;
//     std::string response;

//     while (true) {
//         int n_ctx = llama_n_ctx(ctx);
//         int n_ctx_used = llama_memory_seq_pos_max(llama_get_memory(ctx), 0);
//         if (n_ctx_used + batch.n_tokens > n_ctx) break;
//         if (llama_decode(ctx, batch)) break;

//         new_token_id = llama_sampler_sample(smpl, ctx, -1);
//         if (llama_vocab_is_eog(vocab, new_token_id)) break;

//         char buf[256];
//         int n = llama_token_to_piece(vocab, new_token_id, buf, sizeof(buf), 0, true);
//         if (n < 0) break;

//         std::string piece(buf, n);
//         response += piece;

//         // 即時回傳
//         onToken(piece);

//         batch = llama_batch_get_one(&new_token_id, 1);
//     }

//     messages.push_back({"assistant", strdup(response.c_str())});
//     prev_len = llama_chat_apply_template(tmpl, messages.data(), messages.size(), false, nullptr, 0);
// }

void FreeLlamaChat() {
    for (auto& msg : messages) {
        free(const_cast<char*>(msg.content));
    }
    messages.clear();
    if (smpl) llama_sampler_free(smpl);
    if (ctx) llama_free(ctx);
    if (model) llama_model_free(model);
}


/* USAGE

std::string modelPath = "C:/Users/klps2/OneDrive - NTHU/FINAL_PROJ/Resource/model/tinyllama-1.1b-chat-v1.0.Q4_0.gguf";
    if (!InitLlamaChat(modelPath)) {
        std::cerr << "Failed to initialize llama chat." << std::endl;
        return 1;
    }

    std::string userInput;
    while (true) {
        std::cout << "\033[32m> \033[0m";
        std::getline(std::cin, userInput);
        if (userInput.empty()) break;

        std::cout << "\033[33m";

        QueryLlamaStream(userInput, [](const std::string& token) {
            std::cout << token << std::flush;
            // 如果你是在 GUI，這裡可以是：chatBox->appendText(token);
        });

        std::cout << "\033[0m\n";
    }

    FreeLlamaChat();

*/