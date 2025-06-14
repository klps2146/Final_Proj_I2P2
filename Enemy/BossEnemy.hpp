// #ifndef BOSS_ENEMY_HPP
// #define BOSS_ENEMY_HPP
// #include "Enemy.hpp"
// #include <random>

// class BossEnemy : public Enemy {
// protected:
//     static float shared_hp; // Shared health pool for all bosses
//     static bool boss1_alive;
//     static bool boss2_alive;
//     std::mt19937 rng;
//     float path_timer;
//     float path_interval;
//     Engine::Point random_target;

//     virtual void UpdatePath();
//     void DrawSharedHealthBar() const;

// public:
//     static void ResetSharedHealth();
//     static float GetSharedHealth();
//     static void SetBossStatus(int boss_id, bool status);
//     BossEnemy(std::string img, float x, float y, float radius, float speed, float hp, int money);
//     void Update(float deltaTime) override;
//     void Draw() const override;
//     void HandlePlayerCollision() override;
//     void Hit(float damage, bool byplayer) override;
// };
// #endif // BOSS_ENEMY_HPP

#ifndef BOSS_ENEMY_HPP
#define BOSS_ENEMY_HPP
#include "Enemy.hpp"
#include <random>

class BossEnemy : public Enemy {
protected:
    static float shared_hp; // Shared health pool for all bosses
    static bool boss1_alive;
    static bool boss2_alive;
    static bool boss3_alive; // Added flag for Boss3
    std::mt19937 rng;
    float path_timer;
    float path_interval;
    Engine::Point random_target;

    virtual void UpdatePath();
    void DrawSharedHealthBar() const;

public:
    static void ResetSharedHealth();
    static float GetSharedHealth();
    static void SetBossStatus(int boss_id, bool status);
    BossEnemy(std::string img, float x, float y, float radius, float speed, float hp, int money);
    void Update(float deltaTime) override;
    void Draw() const override;
    void HandlePlayerCollision() override;
    void Hit(float damage, bool byplayer) override;
};
#endif // BOSS_ENEMY_HPP