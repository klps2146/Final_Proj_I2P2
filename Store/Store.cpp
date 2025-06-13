#include "Store.hpp"
#include <allegro5/allegro_primitives.h>
#include <allegro5/allegro5.h>
#include <iostream>
#include <string>
#include "Engine/GameEngine.hpp"
#include "Scene/PlayScene.hpp"
#include "Engine/LOG.hpp"
#include "Skill/SkillBase.hpp"
#include "Engine/SpriteFixed.hpp"


// StoreItem Implementation
StoreItem::StoreItem(const std::string& imagePath, int price, float x, float y)
    : price(price) {
    iconImage = new Engine::SpriteFixed(imagePath, x, y, 0, 0);
}

void StoreItem::Update(float deltaTime) {}

void StoreItem::Draw() const {
    iconImage ->Draw();
}

bool StoreItem::IsClicked(float mouseX, float mouseY) const {
    float x = iconImage->Position.x;
    float y = iconImage->Position.y;
    float w = iconImage->Size.x;
    float h = iconImage->Size.y;
    return (mouseX >= x && mouseX <= x + w && mouseY >= y && mouseY <= y + h);
}

void StoreItem::SetPosition(float x, float y) {
    iconImage ->Position.x = x;
    iconImage ->Position.y = y;
}

void StoreItem::SetSize(float w, float h) {
    iconImage ->Size.x = w;
    iconImage ->Size.y = h;
}



// Store Implementation
PlayScene* Store::getPlayScene() const {
    return dynamic_cast<PlayScene*>(Engine::GameEngine::GetInstance().GetActiveScene());
}

Store::Store() {
    int w = Engine::GameEngine::GetInstance().GetScreenSize().x;
    int h = Engine::GameEngine::GetInstance().GetScreenSize().y;
    background = ColoredRectangle((w - 500) / 2, (h - 500) / 2, 500, 500, al_map_rgba(200, 100, 225, 235), 2.0f, al_map_rgb(255, 255, 255));

    // 初始化商品
    float slotSize = 80;
    float spacing = 20;
    float totalWidth = 4*(slotSize + spacing) - spacing; // 每行 4 個商品
    float startX = background.Position.x + (background.Size.x - totalWidth) / 2;
    float startY = background.Position.y + 50;

    // getPlayScene()->character->itemBar_.slots.
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
    int idx = 0;

    // 繪製商品
    for (int i = 0; i < items.size(); i++) {
        float x = items[i]->iconImage ->Position.x;
        float y = items[i]->iconImage ->Position.y;
        float w = items[i]->iconImage ->Size.x;
        float h = items[i]->iconImage ->Size.y;

        al_draw_rectangle(x, y, x + w, y + h, al_map_rgb(180, 180, 180), 2); // 繪製邊框

        if (idx < getPlayScene()->character->itemBar_.SlotAmount){
            if (getPlayScene()->character->itemBar_.slots[idx]){
                getPlayScene()->character->itemBar_.slots[idx++]->Position.x = x;
                getPlayScene()->character->itemBar_.slots[idx++]->Position.y = y;
                getPlayScene()->character->itemBar_.slots[idx++]->Draw();
            }
        }

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
        for (int i = 0; i < items.size(); i++) {
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