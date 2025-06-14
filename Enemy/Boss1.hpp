#ifndef BOSS1_HPP
#define BOSS1_HPP
#include "BossEnemy.hpp"
#include "EnemyBullet1.hpp"
#include "EnemyBullet2.hpp"
class Boss1 : public BossEnemy {
private:
    float shootTimer;
    float shootInterval;

    void Shoot();

public:
    Boss1(float x, float y);
    void Update(float deltaTime) override;
};
#endif // BOSS1_HPP