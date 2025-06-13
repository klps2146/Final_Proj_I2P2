#include "Drone.hpp"
#include "Scene/PlayScene.hpp"
#include "Enemy/Enemy.hpp"
#include "Engine/AudioHelper.hpp"
#include "Bullet/FireBullet.hpp"


Drone::Drone(float x, float y, float radius, float cooldown, float lifeTime, Engine::Point followOffset = Engine::Point(0, 0))
    : Turret("play/tool-base.png", "play/turret-1.png", x, y, radius, 0, cooldown),
      lifeTime(lifeTime), followOffset(followOffset) {
    Enabled = true;
}

void Drone::Update(float deltaTime) {
    PlayScene* scene = getPlayScene();
    if (scene) {
        Engine::Point playerPos = scene->character->Position + followOffset;
        Engine::Point dir = playerPos - Position;
        float distance = dir.Magnitude();

        // 朝向玩家移動
        if (distance > 4) {
            Engine::Point move = dir.Normalize() * 200 * deltaTime; // 移動速度
            Position = Position + move;
        }
    }

    // 正常攻擊與壽命邏輯
    Turret::Update(deltaTime);

    lifeTime -= deltaTime;
    if (lifeTime <= 0) {
        Enabled = false;
        scene->DroneGroup->RemoveObject(objectIterator);
    }
}

void Drone::CreateBullet() {
    if (!Target) return;

    Engine::Point diff = (Target->Position - Position);
    float rotation = atan2(diff.y, diff.x);
    Engine::Point normalized = diff.Normalize();

    getPlayScene()->DroneBulletGroup->AddNewObject(new FireBullet(Position + normalized * 36, diff, rotation, this));
    
    // AudioHelper::PlayAudio("gun.wav");
}