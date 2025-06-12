#ifndef SHOOTENEMY_HPP
#define SHOOTENEMY_HPP

#include "Enemy.hpp"

class ShootEnemy : public Enemy {
private:
    float shootTimer;
    float shootInterval;
public:
    ShootEnemy(int x, int y);
    void Update(float deltaTime) override;
    void Shoot();
};

#endif // SHOOTENEMY_HPP