#include "BulletBoom.hpp"
#include "Enemy/Enemy.hpp"
#include "Engine/Collider.hpp"
#include "Engine/GameEngine.hpp"
#include "Engine/Group.hpp"
#include "Scene/PlayScene.hpp"

namespace Engine {
    BulletBoom::BulletBoom(float x, float y, Point velocity, float damage)
        : Sprite("play/bullet-1.png", x, y, 64, 32, 0.5f, 0.5f, 0, velocity.x, velocity.y), damage(damage) {
        CollisionRadius = 4.0f;
    }

    void BulletBoom::Update(float deltaTime) {
        Sprite::Update(deltaTime);

        auto* scene = dynamic_cast<PlayScene*>(GameEngine::GetInstance().GetActiveScene());
        for (auto& it : scene->EnemyGroup->GetObjects()) {
            Enemy* enemy = dynamic_cast<Enemy*>(it);
            if (!enemy || !enemy->Visible) continue;
            if (Collider::IsCircleOverlap(Position, CollisionRadius, enemy->Position, enemy->CollisionRadius)) {
                enemy->Hit(damage);
                Visible = false;
                return;
            }
        }

        Point screenSize = GameEngine::GetInstance().GetScreenSize();

        if (Position.x < 0 || Position.x >  PlayScene::MapWidth*64 || Position.y < 0 || Position.y > PlayScene::MapHeight*64) {
            Visible = false;
            
        }
        // else if (scene){ 會CRASH
        //     int mx = (Position.x + scene->BlockSize) / scene->BlockSize + 1;
        //     int my = (Position.y + scene->BlockSize) / scene->BlockSize + 1;
        //     if (mx >= 0 && my >= 0 && scene->mapState[mx][my] == scene->TILE_HOME){
        //         Visible = false;
        //     }
        // }
    }
}
