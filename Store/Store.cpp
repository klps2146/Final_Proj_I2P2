#include "Store.hpp"
#include <allegro5/allegro_primitives.h>
#include <allegro5/allegro5.h>
#include <iostream>
#include <string>
#include "Engine/GameEngine.hpp"
#include "Scene/PlayScene.hpp"
#include "Engine/LOG.hpp"


// StoreItem Implementation
StoreItem::StoreItem(const std::string& imagePath, int price, float x, float y)
    : price(price) {
    bmp = new Engine::Image(imagePath, x, y, 0, 0);
}

void StoreItem::Update(float deltaTime) {
    
}

void StoreItem::Draw() const {
    bmp->Draw();
}

bool StoreItem::IsClicked(float mouseX, float mouseY) const {
    float x = bmp->Position.x;
    float y = bmp->Position.y;
    float w = bmp->Size.x;
    float h = bmp->Size.y;
    return (mouseX >= x && mouseX <= x + w && mouseY >= y && mouseY <= y + h);
}

void StoreItem::SetPosition(float x, float y) {
    bmp->Position.x = x;
    bmp->Position.y = y;
}

void StoreItem::SetSize(float w, float h) {
    bmp->Size.x = w;
    bmp->Size.y = h;
}

// Store Implementation
PlayScene* Store::getPlayScene() const {
    return dynamic_cast<PlayScene*>(Engine::GameEngine::GetInstance().GetActiveScene());
}

Store::Store() {
    int w = Engine::GameEngine::GetInstance().GetScreenSize().x;
    int h = Engine::GameEngine::GetInstance().GetScreenSize().y;
    background = ColoredRectangle((w - 500) / 2, (h - 500) / 2, 500, 500, al_map_rgb(100, 100, 100), 2.0f, al_map_rgb(255, 255, 255));

    // 初始化商品
    float slotSize = 80;
    float spacing = 20;
    float totalWidth = 4*(slotSize + spacing) - spacing; // 每行 4 個商品
    float startX = background.Position.x + (background.Size.x - totalWidth) / 2;
    float startY = background.Position.y + 50;

    for (int i = 0; i < 16; ++i) {
        int row = i / 4; // 第 0 或 1 行
        int col = i % 4; // 第 0 到 3 列
        float x = startX + col * (slotSize + spacing);
        float y = startY + row * (slotSize + spacing);
        StoreItem* item = new StoreItem("play/home.png", 100, x, y);
        item->SetSize(slotSize, slotSize);
        items.push_back(item);
    }
}


void Store::Update(float deltaTime) {
    for (auto item : items) {
        item->Update(deltaTime);
    }
}

void Store::Draw() const {
    background.Draw();

    // 繪製商品
    for (size_t i = 0; i < items.size(); ++i) {
        float x = items[i]->bmp->Position.x;
        float y = items[i]->bmp->Position.y;
        float w = items[i]->bmp->Size.x;
        float h = items[i]->bmp->Size.y;
        
        al_draw_rectangle(x, y, x + w, y + h, al_map_rgb(180, 180, 180), 2); // 繪製邊框
        items[i]->Draw();
    }

    // 顯示金幣數量
    PlayScene* scene = getPlayScene();
    if (scene) {
        Engine::Label moneyLabel("Money: $" + std::to_string(scene->GetMoney()), "pirulen.ttf", 24,
                                background.Position.x + 10, background.Position.y + background.Size.y - 30);
        moneyLabel.Draw();

        Engine::Label exitLabel("press F to exit", "pirulen.ttf", 40,
                                background.Position.x + 10, background.Position.y + background.Size.y + 30);
        exitLabel.Draw();
    }
}

void Store::OnMouseDown(int button, int mx, int my) {
    PlayScene* scene = getPlayScene();
    if (!scene || !scene->buying) return;

    if (button == 1) {
        for (size_t i = 0; i < items.size(); ++i) {
            if (items[i]->IsClicked(static_cast<float>(mx), static_cast<float>(my))) {
                if (scene->GetMoney() >= items[i]->GetPrice()) {
                    scene->EarnMoney(-items[i]->GetPrice());
                    Engine::LOG(Engine::INFO) << "purchase succeed";
                } else {
                    Engine::LOG(Engine::INFO) << "purchase error, you fucking poor";
                }
                break;
            }
        }
    }
}