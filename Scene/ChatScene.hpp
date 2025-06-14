#ifndef CHATSCENE_HPP
#define CHATSCENE_HPP
#include "Engine/IScene.hpp"
#include "UI/Component/Label.hpp"
#include <vector>
#include <allegro5/allegro_audio.h>

class ChatScene final : public Engine::IScene {
private:
    float ticks;
    ALLEGRO_SAMPLE_ID bgmId;
    std::string user_name = "";
    Engine::Label *name_label;
    std::string score;

    std::vector<Engine::Label*> chatLabels;
    Engine::Label* inputLabel = nullptr;

    Engine::Label* userInput;
    

    std::string aiResponseBuffer;   // 用來暫存 AI 回答累積的字串
    Engine::Label* aiResponseLabel; // 顯示 AI 回答的 Label
    bool isResponding = false;

    struct Rect {                                  // 區塊資料結構
        float left, top, right, bottom;
    };
    Rect chatHistoryRect;                          // 顯示訊息區塊
    Rect chatInputRect;                            // 輸入框區塊

    void save_date();
public:
    int state = 1;
    const int MAX_LINES = 10;
    void AddChatLine(const std::string& message, bool isUser);
    void SendUserMessage(const std::string& message);
    void AddAIResponseLines(const std::string& text, int maxCharsPerLine);
    explicit ChatScene() = default;
    void Initialize() override;
    void Terminate() override;
    void Update(float deltaTime) override;
    void BackOnClick(int stage);
    void OnKeyDown(int keyCode) override;
    void OnKeyUp(int keyCode) override;
    void Draw() const override;
    void ConstructUI();
};

#endif   // WINSCENE_HPP
