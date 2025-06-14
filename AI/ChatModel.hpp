#ifndef CHATMODEL_HPP
#define CHATMODEL_HPP

#include <string>
#include <vector>
#include <functional>

bool InitLlamaChat(const std::string& modelPath, int contextLength = 2048, int gpuLayers = 99);
void FreeLlamaChat();

// 回呼函式：每生成一段就會呼叫一次 callback// 輸入使用者訊息，回傳 AI 回應
void QueryLlamaStream(const std::string& userInput, const std::function<void(const std::string&)>& onToken, int maxTokens = -1);

#endif 
