#ifndef ENEMYBULLET2_HPP
#define ENEMYBULLET2_HPP

#include "Engine/Sprite.hpp"
#include "Engine/Point.hpp"

class PlayScene;

namespace Engine {
    class EnemyBullet2 : public Sprite {
    public:
        float damage;
        float rotationSpeed = 4.0f;  // 新增：旋轉速度（每秒旋轉的角度）
        explicit EnemyBullet2(float x, float y, Point velocity, float damage);
        void Update(float deltaTime) override;
    };
}

#endif // ENEMYBULLET2_HPP