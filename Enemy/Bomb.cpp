// #include "Bomb.hpp"
// #include "Engine/GameEngine.hpp"
// #include "Scene/PlayScene.hpp"
// #include "Engine/AudioHelper.hpp"
// #include "UI/Animation/ExplosionEffect.hpp"
// #include <iostream>
// PlayScene* Bomb::getPlayScene() {
//     return dynamic_cast<PlayScene*>(Engine::GameEngine::GetInstance().GetActiveScene());
// }

// Bomb::Bomb(std::string img, float x, float y, float radius, float damage, float delay) 
//     : Sprite(img, x, y), explosionRadius(radius), explosionDamage(damage), explosionDelay(delay), timer(0), exploded(false) {
//         if (!bmp) { // 檢查圖片是否載入成功
//         std::cout<< "Failed to load bomb image: " << img;
//         exploded = true; // 若失敗，標記為已爆炸，避免後續更新
//     }
// }

// void Bomb::Update(float deltaTime) {
//     timer += deltaTime;
//     if (timer >= explosionDelay && !exploded) {
//         Explode();
//     }
//     Sprite::Update(deltaTime);
// }

// void Bomb::Explode() {
//     exploded = true;
//     PlayScene* scene = getPlayScene();
//     float distance = (scene->character->Position - Position).Magnitude();
//     if (distance <= explosionRadius) {
//         scene->Hit(explosionDamage); // 對玩家造成傷害
//     }
//     // 添加爆炸效果
//     scene->EffectGroup->AddNewObject(new ExplosionEffect(Position.x, Position.y));
//     // 播放音效
//     //AudioHelper::PlayAudio("explosion.wav");
//     // 移除炸彈
//     //getPlayScene()->EnemyGroup->RemoveObject(objectIterator);
//     getPlayScene()->RemoveObject(objectIterator);
// }
#include "Bomb.hpp"
#include "Engine/GameEngine.hpp"
#include "Scene/PlayScene.hpp"
#include "Engine/AudioHelper.hpp"
#include "UI/Animation/ExplosionEffect.hpp"
#include <iostream>

PlayScene* Bomb::getPlayScene() {
    return dynamic_cast<PlayScene*>(Engine::GameEngine::GetInstance().GetActiveScene());
}

Bomb::Bomb(std::string img, float x, float y, float radius, float damage, float delay, Engine::Point velocity) 
    : Sprite(img, x, y), explosionRadius(radius), explosionDamage(damage), explosionDelay(delay), timer(0), exploded(false), velocity(velocity) {
    if (!bmp) { // 檢查圖片是否載入成功
        std::cout << "Failed to load bomb image: " << img << std::endl;
        exploded = true; // 若失敗，標記為已爆炸，避免後續更新
    }
}

void Bomb::Update(float deltaTime) {
    if (!exploded) {
        // 更新位置，實現發射效果
        Position.x += velocity.x * deltaTime;
        Position.y += velocity.y * deltaTime;
        timer += deltaTime;
        if (timer >= explosionDelay) {
            Explode();
        }
    }
    Sprite::Update(deltaTime);
}