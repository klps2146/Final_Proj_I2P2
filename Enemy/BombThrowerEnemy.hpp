#ifndef BOMB_THROWER_ENEMY_HPP
#define BOMB_THROWER_ENEMY_HPP
#include "Enemy.hpp"

class BombThrowerEnemy : public Enemy {
protected:
    float throwInterval; // 投擲間隔（秒）
    float throwTimer;    // 計時器
    void ThrowBomb();    // 投擲炸彈的函數

public:
    BombThrowerEnemy(float x, float y); // 構造函數，設置初始位置
    void Update(float deltaTime) override; // 更新函數，覆蓋基類
};
#endif // BOMB_THROWER_ENEMY_HPP