#ifndef ORBITINGBULLET_HPP
#define ORBITINGBULLET_HPP

#include "Bullet.hpp"

class OrbitingBullet : public Bullet {
protected:
    float angle;
    float radius;
    float angularSpeed;
    float lifeTime;
    float elapsedTime;

public:
    OrbitingBullet(Engine::Point initPos, float initAngle, float radius, float angularSpeed, float damage, float lifeTime);

    void Update(float deltaTime) override;
    void OnExplode(Enemy* enemy) override;
};

#endif
