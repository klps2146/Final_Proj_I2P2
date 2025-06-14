#include "Boss2.hpp"
#include "Scene/PlayScene.hpp"
#include "Engine/GameEngine.hpp"
#include "SimpleBomb.hpp"
#include <cmath> // for sin, cos
Boss2::Boss2(float x, float y)
    : BossEnemy("play/boss1.png", x, y, 30.0f, 250.0f, 1000.0f, 50), throwTimer(0), throwInterval(2.0f) {
    dmg = 12;
}
void Boss2::ThrowBomb() {
    PlayScene* scene = getPlayScene();

    if (scene && scene->character) {
        const int bombCount = 5;
        const float spreadAngle = 30.0f;    // 扇形總角度
        const float baseTravelTime = 1.2f;  // 原本炸彈應該多久飛到
        const float speedMultiplier = 1.2f; // 讓炸彈飛快一點

        float bombX = Position.x;
        float bombY = Position.y;

        Engine::Point targetPos = scene->character->Position;
        Engine::Point baseDir = targetPos - Position;
        Engine::Point normDir = baseDir.Normalize();

        float baseAngle = atan2(normDir.y, normDir.x);
        float distance = std::sqrt(baseDir.x * baseDir.x + baseDir.y * baseDir.y);

        float speed = (distance / baseTravelTime) * speedMultiplier;
        float explosionDelay = baseTravelTime / speedMultiplier;

        for (int i = 0; i < bombCount; ++i) {
            int index = i - bombCount / 2;
            float angleOffset = (spreadAngle * index) * M_PI / 180.0f;
            float finalAngle = baseAngle + angleOffset;

            Engine::Point direction(std::cos(finalAngle), std::sin(finalAngle));
            Engine::Point velocity = direction * speed;

            SimpleBomb* bomb = new SimpleBomb("play/bomb2.png", bombX, bombY, 50.0f, 10.0f, explosionDelay, velocity);
            scene->AddNewObject(bomb);
        }
    }
}
void Boss2::Update(float deltaTime) {
    BossEnemy::Update(deltaTime);
    throwTimer += deltaTime;
    if (throwTimer >= throwInterval) {
        ThrowBomb();
        throwTimer = 0;
    }
    if (hp <= 0) {
        SetBossStatus(2, false);
    }
}