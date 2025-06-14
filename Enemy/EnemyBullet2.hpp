#ifndef ENEMYBULLET2_HPP
#define ENEMYBULLET2_HPP

#include "Engine/Sprite.hpp"
#include "Engine/Point.hpp"

class PlayScene;

namespace Engine {
    class EnemyBullet2 : public Sprite {
    public:
        float damage;
        explicit EnemyBullet2(float x, float y, Point velocity, float damage);
        void Update(float deltaTime) override;
        float angularVelocity;
    };
}

#endif // ENEMYBULLET2_HPP