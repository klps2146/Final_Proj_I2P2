#include "Store.hpp"
#include <allegro5/allegro_primitives.h>
#include <allegro5/allegro5.h>
#include <iostream>
#include "Engine/GameEngine.hpp"
#include "Scene/PlayScene.hpp"
#include "Engine/Group.hpp"


PlayScene *Store::getPlayScene() {
    return dynamic_cast<PlayScene *>(Engine::GameEngine::GetInstance().GetActiveScene());
}

Store::Store() {
    //items.resize(SlotAmount, nullptr);
    //AddNewControlObject(UIGroup = new Engine::Group());
}

void Store::Update(float deltaTime) {
    if(getPlayScene()->buying){
        getPlayScene()->ConstructStore();
    }
    /*for (StoreItem* item : items) {
        if (item)
            item->Update(deltaTime);
    }*/
}

void Store::Draw(const Engine::Point& cameraPos, const Engine::Point& screenSize) {
    float barY = screenSize.y - 120; // 商店位置稍微高於 ItemBar
    float slotSize = 60; // 商店槽位稍大
    float spacing = 15;
    float totalWidth = SlotAmount * (slotSize + spacing) - spacing;
    float startX = (screenSize.x - totalWidth) / 2;
    UIGroup->AddNewObject(new Engine::Image("play/sand.png",  cameraPos.x, cameraPos.y, 500, 500));
    /*
    for (int i = 0; i < SlotAmount; ++i) {
        float x = startX + i * (slotSize + spacing);
        float y = barY;
        ALLEGRO_COLOR borderColor = (i == selectedIndex) ? al_map_rgb(255, 215, 0) : al_map_rgb(180, 180, 180);
        al_draw_rectangle(x, y, x + slotSize, y + slotSize, borderColor, 3);

        if (items[i]) {
            items[i]->Position.x = x;
            items[i]->Position.y = y;
            items[i]->Anchor = Engine::Point(0, 0);
            items[i]->SetSize(slotSize, slotSize);
            items[i]->Draw();

            // 繪製商品可用性遮罩（例如已售出）
            float availabilityRatio = items[i]->GetAvailabilityRatio();
            if (availabilityRatio < 1.0f) {
                al_draw_filled_rectangle(x, y, x + slotSize, y + slotSize * (1.0f - availabilityRatio), al_map_rgba(100, 100, 100, 150));
            }
        }
    }*/
}

void Store::SelectSlot(int index) {
    if (index >= 0 && index < SlotAmount)
        selectedIndex = index;
}

void Store::OnKeyDown(int keyCode) {
    if (selectedIndex + ALLEGRO_KEY_1 == keyCode) { // 連按激活
        /*if (GetSelectedItem()) {
            GetSelectedItem()->Purchase();
        }*/
    }
    if (keyCode >= ALLEGRO_KEY_1 && keyCode <= ALLEGRO_KEY_1 + SlotAmount - 1)
        SelectSlot(keyCode - ALLEGRO_KEY_1);
}
/*
StoreItem* Store::GetSelectedItem() const {
    return items[selectedIndex];
}
#include "Itembar.hpp"
#include "Skill/SkillBase.hpp"
#include <allegro5/allegro_primitives.h>
#include <allegro5/allegro5.h>
#include <iostream>

ItemBar::ItemBar() {
    slots.resize(SlotAmount, nullptr);
}

void ItemBar::Update(float deltaTime) {
    for (SkillBase* skill : slots) {
        if (skill)
            skill->Update(deltaTime);
    }
}

void ItemBar::Draw(const Engine::Point& cameraPos, const Engine::Point& screenSize) {
    float barY = screenSize.y - 80;
    float slotSize = 48;
    float spacing = 10;
    float totalWidth = SlotAmount * (slotSize + spacing) - spacing;
    float startX = (screenSize.x - totalWidth) / 2;

    for (int i = 0; i < SlotAmount; ++i) {
        float x = startX + i * (slotSize + spacing);
        float y = barY;
        ALLEGRO_COLOR borderColor = (i == selectedIndex) ? al_map_rgb(255, 255, 0) : al_map_rgb(150, 150, 150);
        al_draw_rectangle(x, y, x + slotSize, y + slotSize, borderColor, 3);

        if (slots[i]) {
            slots[i]->Position.x = x;
            slots[i]->Position.y = y;

            slots[i]->Anchor = Engine::Point(0, 0);

            slots[i]->SetSize(slotSize, slotSize);

            slots[i]->Draw();

            // 畫冷卻遮罩
            float ratio = slots[i]->GetCooldownRatio();
            if (ratio > 0) {
                al_draw_filled_rectangle(x, y, x + slotSize, y + slotSize * ratio, al_map_rgba(0, 0, 0, 150));
            }
        }
    }
}

void ItemBar::SelectSlot(int index) {
    if (index >= 0 && index < SlotAmount)
        selectedIndex = index;
}
#include <iostream>
void ItemBar::OnKeyDown(int keyCode) {
    if (selectedIndex + ALLEGRO_KEY_1 == keyCode){ // 連按
        // std::cout << "AAA" << selectedIndex << std::endl;
        if (GetSelectedSkill()){
            GetSelectedSkill()->Activate();
            // std::cout << "BBB" << selectedIndex << std::endl;
        }
    }
    if (keyCode >= ALLEGRO_KEY_1 && keyCode <= ALLEGRO_KEY_1 + SlotAmount - 1)
        SelectSlot(keyCode - ALLEGRO_KEY_1);
        
}

SkillBase* ItemBar::GetSelectedSkill() const {
    return slots[selectedIndex];
}
*/