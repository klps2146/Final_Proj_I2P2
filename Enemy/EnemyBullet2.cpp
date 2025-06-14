// #include "EnemyBullet2.hpp"
// #include "Character/Character.hpp"
// #include "Engine/Collider.hpp"
// #include "Engine/GameEngine.hpp"
// #include "Engine/Group.hpp"
// #include "Scene/PlayScene.hpp"

// namespace Engine {
//     EnemyBullet2::EnemyBullet2(float x, float y, Point velocity, float damage)
//         : Sprite("play/star.png", x, y, 64, 64, 0.5f, 0.5f, 0, velocity.x, velocity.y), damage(damage){
//         CollisionRadius = 4.0f;
//     }

//     void EnemyBullet2::Update(float deltaTime) {
//         Sprite::Update(deltaTime);

//         auto* scene = dynamic_cast<PlayScene*>(GameEngine::GetInstance().GetActiveScene());
//         if (scene && scene->character) {
//             Character* player = scene->character;
//             if (Collider::IsCircleOverlap(Position, CollisionRadius, player->Position, player->CollisionRadius)) {
//                 scene->Hit(damage);
//                 Visible = false;
//                 return;
//             }
//         }

//         Point screenSize = GameEngine::GetInstance().GetScreenSize();
//         if (Position.x < 0 || Position.x > PlayScene::MapWidth * 64 || Position.y < 0 || Position.y > PlayScene::MapHeight * 64) {
//             Visible = false;
//         }
//     }
// }
#include "EnemyBullet2.hpp"
#include "Character/Character.hpp"
#include "Engine/Collider.hpp"
#include "Engine/GameEngine.hpp"
#include "Engine/Group.hpp"
#include "Scene/PlayScene.hpp"
#include <cmath>

namespace Engine {
    EnemyBullet2::EnemyBullet2(float x, float y, Point velocity, float damage)
        : Sprite("play/star.png", x, y, 64, 64, 0.5f, 0.5f, std::atan2(velocity.y, velocity.x), velocity.x, velocity.y),
          damage(damage), angularVelocity(20.0f) {  // 將角速度從 10.0f 增加到 20.0f
        CollisionRadius = 4.0f;
    }

    void EnemyBullet2::Update(float deltaTime) {
        Sprite::Update(deltaTime);
        Rotation += angularVelocity * deltaTime;  // 更新旋轉角度

        auto* scene = dynamic_cast<PlayScene*>(GameEngine::GetInstance().GetActiveScene());
        if (scene && scene->character) {
            Character* player = scene->character;
            if (Collider::IsCircleOverlap(Position, CollisionRadius, player->Position, player->CollisionRadius)) {
                scene->Hit(damage);
                Visible = false;
                return;
            }
        }

        Point screenSize = GameEngine::GetInstance().GetScreenSize();
        if (Position.x < 0 || Position.x > PlayScene::MapWidth * 64 || Position.y < 0 || Position.y > PlayScene::MapHeight * 64) {
            Visible = false;
        }
    }
}