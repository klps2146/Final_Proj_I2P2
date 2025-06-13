// #ifndef BOMB_HPP
// #define BOMB_HPP
// #include "Engine/Sprite.hpp"

// class PlayScene;

// class Bomb : public Engine::Sprite {
// protected:
//     float explosionRadius;
//     float explosionDamage;
//     float explosionDelay;
//     float timer;
//     bool exploded;
//     PlayScene* getPlayScene();

// public:
//     Bomb(std::string img, float x, float y, float radius, float damage, float delay);
//     void Update(float deltaTime) override;
//     void Explode();
// };
// #endif // BOMB_HPP


#ifndef BOMB_HPP
#define BOMB_HPP
#include "Engine/Sprite.hpp"
#include "Engine/Point.hpp"

class PlayScene;

class Bomb : public Engine::Sprite {
protected:
    float explosionRadius;
    float explosionDamage;
    float explosionDelay;
    float timer;
    bool exploded;
    Engine::Point velocity; // 速度向量，用於發射效果
    PlayScene* getPlayScene();

public:
    Bomb(std::string img, float x, float y, float radius, float damage, float delay, Engine::Point velocity = Engine::Point(0, 0));
    virtual void Update(float deltaTime) override;
    virtual void Explode() = 0; // 純虛函數，子類必須實現
};
#endif // BOMB_HPP