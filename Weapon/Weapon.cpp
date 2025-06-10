// Weapon.cpp
#include "Weapon.hpp"
#include "Scene/PlayScene.hpp"
#include "Engine/GameEngine.hpp"

PlayScene* Weapon::getPlayScene() {
    return dynamic_cast<PlayScene*>(Engine::GameEngine::GetInstance().GetActiveScene());
}

Weapon::Weapon(std::string img, float x, float y, float coolDown)
    : Engine::Sprite(img, x, y), coolDown(coolDown), reload(0) {
}

void Weapon::Update(float deltaTime) {
    Sprite::Update(deltaTime);
    if (reload > 0) {
        reload -= deltaTime; // 更新冷卻計時器
    }
}

void Weapon::Fire() {
    if (reload <= 0) { // 如果冷卻完成
        CreateBullet(); // 創建子彈
        reload = coolDown; // 重置冷卻計時器
    }
}

void Weapon::Draw() const {
    Sprite::Draw();
}