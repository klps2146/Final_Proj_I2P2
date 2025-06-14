#include "EnemyBullet1.hpp"
#include "Character/Character.hpp"
#include "Engine/Collider.hpp"
#include "Engine/GameEngine.hpp"
#include "Engine/Group.hpp"
#include "Scene/PlayScene.hpp"

namespace Engine {
    EnemyBullet1::EnemyBullet1(float x, float y, Point velocity, float damage)
        : Sprite("play/bullet-8.png", x, y, 64, 64, 0.5f, 0.5f, 0, velocity.x, velocity.y), damage(damage) {
        CollisionRadius = 4.0f;
    }

    void EnemyBullet1::Update(float deltaTime) {
        Rotation = atan2(Velocity.y, Velocity.x);
        Sprite::Update(deltaTime);

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