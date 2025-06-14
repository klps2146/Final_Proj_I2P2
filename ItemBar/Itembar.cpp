#include "Itembar.hpp"
#include "Skill/SkillBase.hpp"

#include "Skill/SummonDroneSkill.hpp"
#include "UI/Component/Label.hpp"
#include "UI/Component/ColoredRectangle.hpp"
#include "Scene/PlayScene.hpp"
#include "Engine/GameEngine.hpp"
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
            
            // SummonDroneSkill* sdk = dynamic_cast<SummonDroneSkill*>(slots[i]);
            // if (sdk && sdk)

            slots[i]->Draw();
            if (slots[i]->getUnlock()){
                Engine::Label levelLab(std::to_string((int)slots[i]->level), "pirulen.ttf", 20, x + 2, y - 4, 255, 255, 255, 255);
                ColoredRectangle bgrec(x, y, 18, 20, al_map_rgba(100, 100, 100 , 190));
                bgrec.Draw();
                levelLab.Draw();
            }
            else{
                // 畫叉叉
                float padding = 4;
                ALLEGRO_COLOR crossColor = al_map_rgba(255, 50, 50, 215);
                al_draw_line(x + padding, y + padding, x + slotSize - padding, y + slotSize - padding, crossColor, 2.0);
                al_draw_line(x + padding, y + slotSize - padding, x + slotSize - padding, y + padding, crossColor, 2.0);
            }

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
        std::cout << "AAA " << selectedIndex << std::endl;
        if (GetSelectedSkill() && getPlayScene()){
            if (getPlayScene()->scenenum != 2){
                GetSelectedSkill()->Activate();
                std::cout << "BBB " << selectedIndex << std::endl;
            }
            else{
                getPlayScene()->SkillWarn->Text = "Skills Are BANNED Here";
            }
        }
    }
    if (keyCode >= ALLEGRO_KEY_1 && keyCode <= ALLEGRO_KEY_1 + SlotAmount - 1)
        SelectSlot(keyCode - ALLEGRO_KEY_1);
        
}

SkillBase* ItemBar::GetSelectedSkill() const {
    return slots[selectedIndex];
}
PlayScene *ItemBar::getPlayScene() {
    return dynamic_cast<PlayScene *>(Engine::GameEngine::GetInstance().GetActiveScene());
}