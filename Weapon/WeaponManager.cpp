

#include "WeaponManager.hpp"
#include <allegro5/allegro_primitives.h>
#include <iostream>

namespace Engine {
    // 現有方法保持不變
    void WeaponManager::AddWeapon(std::unique_ptr<Weapon> weapon) {
        weapons.push_back(std::move(weapon));
        if (currentWeaponIndex == -1 && !weapons.empty()) {
            currentWeaponIndex = 0;
            weapons[currentWeaponIndex]->isActive = true;
        }
    }

    void WeaponManager::SwitchWeapon(int index) {
        std::cout << "Switching to weapon index: " << index << ", weapons size: " << weapons.size() << std::endl;
        if (index >= 0 && index < weapons.size()) {
            if (currentWeaponIndex >= 0 && currentWeaponIndex < weapons.size()) {
                weapons[currentWeaponIndex]->isActive = false;
                std::cout << "Deactivated weapon " << currentWeaponIndex << std::endl;
            }
            currentWeaponIndex = index;
            weapons[currentWeaponIndex]->isActive = true;
            std::cout << "Activated weapon " << index << std::endl;
        } else {
            std::cout << "Invalid weapon index: " << index << std::endl;
        }
    }

    void WeaponManager::Update(float deltaTime) {
        if (currentWeaponIndex >= 0 && currentWeaponIndex < weapons.size()) {
            weapons[currentWeaponIndex]->Update(deltaTime);
        }
    }

    void WeaponManager::Draw() const {
        if (currentWeaponIndex >= 0 && currentWeaponIndex < weapons.size()) {
            weapons[currentWeaponIndex]->Draw();
        }
    }

    void WeaponManager::OnMouseMove(int mx, int my) {
        if (currentWeaponIndex >= 0 && currentWeaponIndex < weapons.size()) {
            weapons[currentWeaponIndex]->OnMouseMove(mx, my);
        }
    }

    void WeaponManager::OnMouseDown(int button, int mx, int my) {
        if (currentWeaponIndex >= 0 && currentWeaponIndex < weapons.size()) {
            weapons[currentWeaponIndex]->OnMouseDown(button, mx, my);
        }
    }

    void WeaponManager::OnMouseUp(int button, int mx, int my) {
        if (currentWeaponIndex >= 0 && currentWeaponIndex < weapons.size()) {
            weapons[currentWeaponIndex]->OnMouseUp(button, mx, my);
        }
    }

    void WeaponManager::SetCharacterPosition(Point pos) {
        for (auto& weapon : weapons) {
            weapon->SetCharacterPosition(pos);
        }
    }

void WeaponManager::DrawWeaponBar(const Engine::Point& cameraPos, const Engine::Point& screenSize) {
    const int WeaponSlots = 5;  // 槽位數量
    float slotSize = 48;        // 每個槽位的大小 (48x48)
    float spacing = 10;         // 槽位間距
    float margin = 20;          // 邊距

    // 將武器欄靠右垂直排列
    float x = screenSize.x - slotSize - margin;
    float startY = margin;

    for (int i = 0; i < WeaponSlots; ++i) {
        float y = startY + i * (slotSize + spacing);
        // 繪製槽位外框，選中時為黃色，未選中為灰色
        ALLEGRO_COLOR borderColor = (i == currentWeaponIndex) ? al_map_rgb(255, 255, 0) : al_map_rgb(150, 150, 150);
        al_draw_rectangle(x, y, x + slotSize, y + slotSize, borderColor, 3);

        // 如果該槽位有武器，繪製武器圖示
        if (i < weapons.size()) {
            auto& weapon = weapons[i];
            ALLEGRO_BITMAP* bitmap = weapon->bmp.get();  // 假設有 GetBitmap() 方法
            if (bitmap) {
                float bw = al_get_bitmap_width(bitmap);  // 原始位圖寬度
                float bh = al_get_bitmap_height(bitmap); // 原始位圖高度
                al_draw_scaled_bitmap(bitmap, 0, 0, bw, bh, x, y, slotSize, slotSize, 0);
            }
        }
    }
}
}