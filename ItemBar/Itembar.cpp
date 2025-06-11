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

void ItemBar::OnKeyDown(int keyCode) {
    if (selectedIndex + ALLEGRO_KEY_1 == keyCode) // 連按
        if (GetSelectedSkill())
            GetSelectedSkill()->Activate();
    if (keyCode >= ALLEGRO_KEY_1 && keyCode <= ALLEGRO_KEY_1 + SlotAmount - 1)
        SelectSlot(keyCode - ALLEGRO_KEY_1);
        
}

SkillBase* ItemBar::GetSelectedSkill() const {
    return slots[selectedIndex];
}
