#ifndef ENEMYBULLET1_HPP
#define ENEMYBULLET1_HPP

#include "Engine/Sprite.hpp"
#include "Engine/Point.hpp"

class PlayScene;

namespace Engine {
    class EnemyBullet1 : public Sprite {
    public:
        float damage;
        explicit EnemyBullet1(float x, float y, Point velocity, float damage);
        void Update(float deltaTime) override;
    };
}

#endif // ENEMYBULLET1_HPP