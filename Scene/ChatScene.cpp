#include <functional>
#include <string>
#include <fstream>
#include <ctime>
#include <iostream>
#include <sstream>

#include "Engine/AudioHelper.hpp"
#include "Engine/GameEngine.hpp"
#include "Engine/Point.hpp"
#include "PlayScene.hpp"
#include "UI/Component/Image.hpp"
#include "UI/Component/ImageButton.hpp"
#include "UI/Component/Label.hpp"
#include "ChatScene.hpp"
#include "AI/ChatModel.hpp"
#include "Engine/SpriteFixed.hpp"

void ChatScene::Initialize() {
    state= 1;

    ticks = 0;
    int w = Engine::GameEngine::GetInstance().GetScreenSize().x;
    int h = Engine::GameEngine::GetInstance().GetScreenSize().y;
    int halfW = w / 2;
    int halfH = h / 2;
    SetBackgroundColor(al_map_rgba(100, 100, 100, 100));
    
    try {
        std::string modelPath = "C:/Users/klps2/OneDrive - NTHU/FINAL_PROJ/Resource/model/tinyllama-1.1b-chat-v1.0.Q4_0.gguf";
        if (!InitLlamaChat(modelPath)) {
            std::cerr << "Failed to initialize LLaMA model.\n";
            AddChatLine("[Error] Unable to load AI model.", false);
            state = 0;
        }
        
    } catch (const std::bad_alloc& e) {
        std::cerr << "Out of memory: Model initialization failed.\n";
        AddChatLine("[Error] Not enough memory to start chat.", false);
    }
    ConstructUI();
    aiResponseBuffer = "";
    std::cout << "SSS\n" ;
}

void ChatScene::ConstructUI() {
    int w = Engine::GameEngine::GetInstance().GetScreenSize().x;
    int h = Engine::GameEngine::GetInstance().GetScreenSize().y;
    int halfW = w / 2;
    int halfH = h / 2;

    // 聊天視窗大小
    int chatWidth = w * 0.8;
    int chatHeight = h * 0.9;
    float chatX = (w - chatWidth) / 2;
    float chatY = (h - chatHeight) / 2;
    chatHistoryRect = {chatX, chatY, chatX + chatWidth, chatY + chatHeight - 80};
    chatInputRect = {chatX, chatY + chatHeight - 80, chatX + chatWidth, chatY + chatHeight};


    // 初始 Label：提示輸入名稱（你原本有的）
    AddNewObject(name_label = new Engine::Label("..........", "consola-1.ttf", 24, chatInputRect.left + 6 , chatInputRect.top + 7, 0, 255, 255, 200, 0.0, 0.0));

    AddNewObject(userInput = new Engine::Label("...", "consola-1.ttf", 26, chatHistoryRect.right - 10, chatHistoryRect.top + 30, 255, 255, 255, 255, 1.0f, 0.0f));
    AddNewObject(new Engine::SpriteFixed("AI/WTH.png", chatHistoryRect.left - 32, chatHistoryRect.top + 80, 60, 60));
    AddNewObject(aiResponseLabel = new Engine::Label("...", "consola-1.ttf", 26, chatHistoryRect.left + 10, chatHistoryRect.top + 80, 255, 255, 255, 255, 0.0f, 0.0f));
    // auto* label = new Engine::Label( "...", "consola-1.ttf", 26, chatHistoryRect.left + 10, chatHistoryRect.top + 80, 255, 255, 200, 255, 0.0f, 0.0f);
    // chatLabels.push_back(label);

    // AddAIResponseLines("..." ,   70);

}

void ChatScene::AddAIResponseLines(const std::string& text, int maxCharsPerLine) {
    if (text.empty()) return;

    float lineHeight = 30;
    float baseY = chatHistoryRect.top + 80 + chatLabels.size() * lineHeight;

    std::istringstream stream(text);
    std::string paragraph;
    while (std::getline(stream, paragraph)) {
        size_t i = 0;
        size_t paragraphLength = paragraph.length();
        while (i < paragraphLength) {
            size_t remaining = paragraphLength - i;
            size_t len = std::min(static_cast<size_t>(maxCharsPerLine), remaining);
            if (len == 0) break; // 避免死循環
            std::string line = paragraph.substr(i, len);
            i += len;

            auto* label = new Engine::Label(
                line,
                "consola-1.ttf", 26,
                chatHistoryRect.left + 10, baseY,
                255, 255, 200, 255,
                0.0f, 0.0f
            );

            chatLabels.push_back(label);
            AddNewObject(label);
            baseY += lineHeight;
        }
    }
}

void ChatScene::AddChatLine(const std::string& message, bool isUser) {
    float lineHeight = 30;
    float baseY = chatHistoryRect.top - 30 + chatLabels.size() * lineHeight;

    float x;
    float anchorX;
    if (isUser) {
        x = chatHistoryRect.right - 10;  // 靠右
        anchorX = 1.0f;
    } else {
        x = chatHistoryRect.left + 10;   // 靠左
        anchorX = 0.0f;
    }

    auto* label = new Engine::Label(message, "consola-1.ttf", 24, chatHistoryRect.right - 10, chatHistoryRect.top - 30, 255, 255, 255, 255, 1.0f, 0.0f);
    chatLabels.push_back(label);
    AddNewObject(label);
}

void ChatScene::SendUserMessage(const std::string& message) {
    // for (auto* label : chatLabels) {
    //     delete label;
    // }
    // chatLabels.clear();
    aiResponseBuffer = "";
    aiResponseLabel->Text = "";
    // AddChatLine(message, true);

    try {
        QueryLlamaStream("Respond in one short sentence: "  + message, [this](const std::string& token) {
            std::cout << token << std::flush;
            aiResponseBuffer += token;
            // std::cout << "<<" << aiResponseBuffer << ">>";

            // aiResponseLabel->Text = aiResponseBuffer;
        }, 25);
    } catch (const std::bad_alloc& e) {
        std::cerr << "Out of memory: Unable to generate AI response.\n";
        aiResponseBuffer = "[Error] Not enough memory. Please restart the game or close other programs.";
        // aiResponseLabel->Text = "[Error] Not enough memory. Please restart the game or close other programs.";
    } catch (const std::exception& e) {
        std::cerr << "Exception: " << e.what() << "\n";
        aiResponseBuffer = "[Error] Something went wrong. Please try again later.";
        // aiResponseLabel->Text = "[Error] Something went wrong. Please try again later.";
    }

    std::cout << "\n[FULL]:" << aiResponseBuffer << std::endl;
    aiResponseLabel->Text = aiResponseBuffer;
    // AddAIResponseLines(aiResponseBuffer, 70);
}

void ChatScene::Terminate() {
    IScene::Terminate();
    std::cout <<"STOP\n";

    FreeLlamaChat();
    // AudioHelper::StopBGM(bgmId);
}

void ChatScene::Update(float deltaTime) {
    ticks += deltaTime;
    if (!state)
    BackOnClick(0);
    // if (ticks > 4 && ticks < 100 &&
    //     dynamic_cast<PlayScene *>(Engine::GameEngine::GetInstance().GetScene("play"))->MapId == 2) {
    //     ticks = 100;
    //     bgmId = AudioHelper::PlayBGM("happy.ogg");
    // }
}

void ChatScene::Draw() const{
    IScene::Draw();
    al_draw_filled_rounded_rectangle(chatInputRect.left, chatInputRect.top,
        chatInputRect.right, chatInputRect.bottom, 18, 18,
        al_map_rgba(50, 50, 50, 255));

    al_draw_rounded_rectangle(chatInputRect.left, chatInputRect.top,
        chatInputRect.right, chatInputRect.bottom, 18, 18,
        al_map_rgb(200, 200, 200), 2);

    if (int(ticks * 2) % 2 == 0) {
        std::string text = user_name + "|";
        name_label->Text = text;
    } else {
        name_label->Text = user_name;
    }

    name_label->Draw();
    // for (auto* label : chatLabels) {
    //     if (label) label->Draw();
    // }
}


void ChatScene::BackOnClick(int stage) {
    // Change to select scene.
    if (stage == 1){
        save_date();
    }
    Engine::GameEngine::GetInstance().ChangeScene("stage-select");
}

bool shift_pressed = false;

void ChatScene::OnKeyDown(int keyCode) {
    if (keyCode == ALLEGRO_KEY_ESCAPE){
        BackOnClick(0);
    }

    if (keyCode == ALLEGRO_KEY_LSHIFT || keyCode == ALLEGRO_KEY_RSHIFT) {
        shift_pressed = true;
        return;
    }

    if (keyCode >= ALLEGRO_KEY_A && keyCode <= ALLEGRO_KEY_Z) {
        char c = (shift_pressed ? 'A' : 'a') + (keyCode - ALLEGRO_KEY_A);
        user_name += c;
    }
    else if (keyCode >= ALLEGRO_KEY_0 && keyCode <= ALLEGRO_KEY_9) {
        if (!shift_pressed) {
            user_name += (keyCode - ALLEGRO_KEY_0) + '0';
        } else {
            const char* shift_nums = ")!@#$%^&*(";
            user_name += shift_nums[keyCode - ALLEGRO_KEY_0];
        }
    }
    else {
        switch (keyCode) {
            case ALLEGRO_KEY_SPACE:
                user_name += ' ';
                break;
            case ALLEGRO_KEY_MINUS:     // -
                user_name += shift_pressed ? '_' : '-';
                break;
            case ALLEGRO_KEY_EQUALS:    // = or +
                user_name += shift_pressed ? '+' : '=';
                break;
            case ALLEGRO_KEY_SEMICOLON: // ; or :
                user_name += shift_pressed ? ':' : ';';
                break;
            case ALLEGRO_KEY_QUOTE:     // ' or "
                user_name += shift_pressed ? '\"' : '\'';
                break;
            case ALLEGRO_KEY_COMMA:     // , or <
                user_name += shift_pressed ? '<' : ',';
                break;
            case ALLEGRO_KEY_FULLSTOP:  // . or >
                user_name += shift_pressed ? '>' : '.';
                break;
            case ALLEGRO_KEY_SLASH:     // / or ?
                user_name += shift_pressed ? '?' : '/';
                break;
            case ALLEGRO_KEY_BACKSLASH: // \ or |
                user_name += shift_pressed ? '|' : '\\';
                break;
            case ALLEGRO_KEY_OPENBRACE: // [ or {
                user_name += shift_pressed ? '{' : '[';
                break;
            case ALLEGRO_KEY_CLOSEBRACE: // ] or }
                user_name += shift_pressed ? '}' : ']';
                break;
            case ALLEGRO_KEY_BACKSPACE:
                if (!user_name.empty()) user_name.pop_back();
                break;
            case ALLEGRO_KEY_ENTER:
                if (user_name == "") break;
                std::cout << "running......\n";
                userInput->Text = user_name;

                aiResponseBuffer = "";
                aiResponseLabel->Text = "...";
                
                SendUserMessage(user_name);
                name_label->Text = "";
                user_name = "";

                break;
        }
    }

    name_label->Text = user_name;

}

void ChatScene::OnKeyUp(int keyCode) {
    if (keyCode == ALLEGRO_KEY_LSHIFT || keyCode == ALLEGRO_KEY_RSHIFT) {
        shift_pressed = false;
    }
}

#include <filesystem>
void ChatScene::save_date(){
    // std::time_t t = std::time(nullptr);
    // std::tm* now = std::localtime(&t);
    // std::cout << (now->tm_year + 1900) << "-"<< (now->tm_mon + 1) << "-"<< now->tm_mday << std::endl;
    
    // std::ofstream ofs("../Resource/scoreboard.txt", std::ios::out | std::ios::app);
    // ofs << user_name << " " << score << " " << (now->tm_year + 1900) << "-"<< (now->tm_mon + 1) << "-"<< now->tm_mday << "\n";
    // user_name = "";
    // score = "";
}