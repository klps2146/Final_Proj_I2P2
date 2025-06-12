#include "EnemyBullet.hpp"
#include "Character/Character.hpp"
#include "Engine/Collider.hpp"
#include "Engine/GameEngine.hpp"
#include "Scene/PlayScene.hpp"

namespace Engine {
    EnemyBullet::EnemyBullet(const std::string& texturePath, float x, float y, float w, float h, 
                           float anchorX, float anchorY, float damage, Point velocity)
        : Sprite(texturePath, x, y, w, h, anchorX, anchorY, 0, velocity.x, velocity.y), 
          damage(damage) {
    }

    void EnemyBullet::Update(float deltaTime) {
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
        if (Position.x < 0 || Position.x > PlayScene::MapWidth * 64 || 
            Position.y < 0 || Position.y > PlayScene::MapHeight * 64) {
            Visible = false;
        }
    }
}