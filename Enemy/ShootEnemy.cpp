// #include "ShootEnemy.hpp"
// #include "EnemyBullet1.hpp"
// #include "Engine/GameEngine.hpp"
// #include "Scene/PlayScene.hpp"

// ShootEnemy::ShootEnemy(int x, int y) : Enemy("play/enemy-7.png", x, y, 10, 80, 5, 5), shootTimer(0), shootInterval(2.0f) {
//     dmg = 6;
// }

// void ShootEnemy::Update(float deltaTime) {
//     Enemy::Update(deltaTime);
//     shootTimer += deltaTime;
//     if (shootTimer >= shootInterval) {
//         Shoot();
//         shootTimer = 0;
//     }
// }

// void ShootEnemy::Shoot() {
//     auto* scene = dynamic_cast<PlayScene*>(Engine::GameEngine::GetInstance().GetActiveScene());
//     if (scene) {
//         Engine::Point direction =  Position;
//         direction = direction.Normalize();
//         float speed = 200.0f;
//         Engine::Point velocity = direction * speed;
//         float damage = 10.0f;
//         Engine::EnemyBullet1* bullet = new Engine::EnemyBullet1(Position.x, Position.y, velocity, damage);
//         scene->BulletGroup->AddNewObject(bullet);
//     }
// }
#include "ShootEnemy.hpp"
#include "EnemyBullet1.hpp"
#include "Engine/GameEngine.hpp"
#include "Scene/PlayScene.hpp"
#include <cmath> // 為了使用 atan2

ShootEnemy::ShootEnemy(int x, int y) : Enemy("play/monster5.png", x, y, 10, 80, 5, 5), shootTimer(0), shootInterval(2.0f) {
    dmg = 6;
}

void ShootEnemy::Update(float deltaTime) {
    Enemy::Update(deltaTime);
    auto* scene = dynamic_cast<PlayScene*>(Engine::GameEngine::GetInstance().GetActiveScene());
    if (scene && scene->character) {
        Engine::Point playerPos = scene->character->Position;
        Engine::Point direction = playerPos - Position;
        direction = direction.Normalize();
        float angle = atan2(direction.y, direction.x);
        Rotation = angle; // 假設 Rotation 是以弧度為單位
    }
    shootTimer += deltaTime;
    if (shootTimer >= shootInterval) {
        Shoot();
        shootTimer = 0;
    }
}

void ShootEnemy::Shoot() {
    auto* scene = dynamic_cast<PlayScene*>(Engine::GameEngine::GetInstance().GetActiveScene());
    if (scene && scene->character) {
        Engine::Point playerPos = scene->character->Position;
        Engine::Point direction = playerPos - Position;
        direction = direction.Normalize();
        float speed = 200.0f;
        Engine::Point velocity = direction * speed;
        float damage = 10.0f;
        Engine::EnemyBullet1* bullet = new Engine::EnemyBullet1(Position.x, Position.y, velocity, damage);
        scene->BulletGroup->AddNewObject(bullet);
    }
}