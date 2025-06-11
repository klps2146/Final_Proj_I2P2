#include "WeaponBullet.hpp"
#include "Enemy/Enemy.hpp"
#include "Engine/Collider.hpp"
#include "Engine/GameEngine.hpp"
#include "Engine/Group.hpp"
#include "Engine/IObject.hpp"
#include "Engine/IScene.hpp"
#include "Engine/Point.hpp"
#include "Scene/PlayScene.hpp"

PlayScene* WeaponBullet::getPlayScene() {
    return dynamic_cast<PlayScene*>(Engine::GameEngine::GetInstance().GetActiveScene());
}

void WeaponBullet::OnExplode(Enemy* enemy) {
    // 預設為空，子類可實現爆炸效果
}

WeaponBullet::WeaponBullet(std::string img, float speed, float damage, Engine::Point position, Engine::Point forwardDirection, float rotation, Weapon* parent)
    : Sprite(img, position.x, position.y), speed(speed), damage(damage), parent(parent) {
    Velocity = forwardDirection.Normalize() * speed;  // 設定子彈速度方向
    Rotation = rotation;                              // 設定子彈旋轉角度
    CollisionRadius = 4;                              // 碰撞半徑
}

void WeaponBullet::Update(float deltaTime) {
    Sprite::Update(deltaTime);  // 更新位置
    PlayScene* scene = getPlayScene();
    
    // 檢測與敵人的碰撞
    for (auto& it : scene->EnemyGroup->GetObjects()) {
        Enemy* enemy = dynamic_cast<Enemy*>(it);
        if (!enemy->Visible)
            continue;
        if (Engine::Collider::IsCircleOverlap(Position, CollisionRadius, enemy->Position, enemy->CollisionRadius)) {
            OnExplode(enemy);                            // 觸發爆炸效果

            enemy->Hit(damage,true);                          // 對敵人造成傷害
            scene->WeaponBulletGroup->RemoveObject(objectIterator);  // 移除子彈
            return;
        }
    }
    
    // 如果子彈超出螢幕範圍，移除它
    if (!Engine::Collider::IsRectOverlap(Position - Size / 2, Position + Size / 2, Engine::Point(0, 0), PlayScene::GetClientSize()))
        scene->WeaponBulletGroup->RemoveObject(objectIterator);
}