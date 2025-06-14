#ifndef BOSS2_HPP
#define BOSS2_HPP
#include "BossEnemy.hpp"
#include "Bomb.hpp"

class Boss2 : public BossEnemy {
private:
    float throwTimer;
    float throwInterval;

    void ThrowBomb();

public:
    Boss2(float x, float y);
    void Update(float deltaTime) override;
};
#endif // BOSS2_HPP