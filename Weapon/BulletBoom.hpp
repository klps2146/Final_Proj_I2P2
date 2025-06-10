#ifndef BULLETBOOM_HPP
#define BULLETBOOM_HPP

#include "Engine/Sprite.hpp"
#include "Engine/Point.hpp"

class Enemy;

namespace Engine {
    class BulletBoom : public Sprite {
    public:
        float damage;
        explicit BulletBoom(float x, float y, Point velocity, float damage);
        void Update(float deltaTime) override;
    };
}

#endif // BULLETBOOM_HPP
