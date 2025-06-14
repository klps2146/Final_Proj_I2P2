#include <allegro5/base.h>
#include <cmath>
#include <random>
#include <string>

#include "OrbitingBullet.hpp"
#include "Engine/Point.hpp"
#include "Engine/Collider.hpp"
#include "Enemy/Enemy.hpp"
#include "Scene/PlayScene.hpp"
#include "UI/Animation/DirtyEffect.hpp"

OrbitingBullet::OrbitingBullet(Engine::Point initPos, float initAngle, float radius, float angularSpeed, float damage, float lifeTime)
    : Bullet("play/Shulker_Bullet.png", 0, damage, initPos, Engine::Point(0, 0), 0, nullptr),
    angle(initAngle), radius(radius), angularSpeed(angularSpeed), lifeTime(lifeTime), elapsedTime(0) {
    CollisionRadius = 6;
    SetSize(60, 60);
}

void OrbitingBullet::Update(float deltaTime) {
    elapsedTime += deltaTime;
    if (elapsedTime >= lifeTime) {
        getPlayScene()->BulletGroup->RemoveObject(objectIterator);
        return;
    }

    angle += angularSpeed * deltaTime;
    Position.x = getPlayScene()->character->Position.x + radius * cos(angle);
    Position.y = getPlayScene()->character->Position.y + radius * sin(angle);

    PlayScene* scene = getPlayScene();
    for (auto& it : scene->EnemyGroup->GetObjects()) {
        Enemy* enemy = dynamic_cast<Enemy*>(it);
        if (!enemy->Visible) continue;

        if (Engine::Collider::IsCircleOverlap(Position, CollisionRadius, enemy->Position, enemy->CollisionRadius)) {
            OnExplode(enemy);
            enemy->Hit(damage, true);
        }
    }

    Sprite::Update(deltaTime);
}

void OrbitingBullet::OnExplode(Enemy* enemy) {
    std::random_device dev;
    std::mt19937 rng(dev());
    std::uniform_int_distribution<int> dist(2, 10);
    getPlayScene()->GroundEffectGroup->AddNewObject(
        new DirtyEffect("play/dirty-3.png", dist(rng), enemy->Position.x, enemy->Position.y)
    );
}
