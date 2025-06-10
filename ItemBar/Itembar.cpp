#include "Itembar.hpp"
#include <iostream>

#include <allegro5/allegro_primitives.h>

ItemBar::ItemBar() {
    slots.resize(SlotAmount);
    
}

void ItemBar::Update(float deltaTime) {
    for (auto& slot : slots) {
        if (!slot.isAvailable) {
            slot.cooldown -= deltaTime;
            if (slot.cooldown <= 0) {
                slot.cooldown = 0;
                slot.isAvailable = true;
            }
        }
    }
}

void ItemBar::Draw(const Engine::Point& cameraPos, const Engine::Point& screenSize) {
    float barY = screenSize.y - 80; // 距下方 80
    float slotSize = 48;
    float spacing = 10;
    float totalWidth = SlotAmount * (slotSize + spacing) - spacing;
    float startX = (screenSize.x - totalWidth) / 2;

    for (int i = 0; i < SlotAmount; ++i) {
        float x = startX + i * (slotSize + spacing);
        float y = barY;
        ALLEGRO_COLOR borderColor = (i == selectedIndex) ? al_map_rgb(255, 255, 0) : al_map_rgb(150, 150, 150);
        al_draw_rectangle(x, y, x + slotSize, y + slotSize, borderColor, 3);

        if (slots[i].icon) {
            al_draw_scaled_bitmap(slots[i].icon, 0, 0,
                al_get_bitmap_width(slots[i].icon), al_get_bitmap_height(slots[i].icon),
                x, y, slotSize, slotSize, 0);
        }

        // 冷卻遮罩
        if (!slots[i].isAvailable && slots[i].maxCooldown > 0) {
            float ratio = slots[i].cooldown / slots[i].maxCooldown;
            al_draw_filled_rectangle(x, y, x + slotSize, y + slotSize * ratio, al_map_rgba(0, 0, 0, 150));
        }
    }
}

void ItemBar::SelectSlot(int index) {
    if (index >= 0 && index < SlotAmount) {
        selectedIndex = index;
        
    }
}

void ItemBar::OnKeyDown(int keyCode){
    if (keyCode >= ALLEGRO_KEY_1 && keyCode <= ALLEGRO_KEY_1 + this->SlotAmount - 1) {
        this->SelectSlot(keyCode - ALLEGRO_KEY_1);
    }
}