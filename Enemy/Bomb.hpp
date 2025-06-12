#ifndef BOMB_HPP
#define BOMB_HPP
#include "Engine/Sprite.hpp"

class PlayScene;

class Bomb : public Engine::Sprite {
protected:
    float explosionRadius;
    float explosionDamage;
    float explosionDelay;
    float timer;
    bool exploded;
    PlayScene* getPlayScene();

public:
    Bomb(std::string img, float x, float y, float radius, float damage, float delay);
    void Update(float deltaTime) override;
    void Explode();
};
#endif // BOMB_HPP