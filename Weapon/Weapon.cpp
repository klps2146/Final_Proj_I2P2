#include "Weapon.hpp"

namespace Engine {
    Weapon::Weapon(std::string img, float x, float y, float w, float h,
                   float anchorX, float anchorY)
        : Sprite(img, x, y, w, h, anchorX, anchorY) {
        // 可以在這裡添加其他初始化代碼，例如設置成員變量
        isActive = false; // 初始化武器的活動狀態
    }
}