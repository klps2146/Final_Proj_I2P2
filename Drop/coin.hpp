#ifndef COIN_HPP
#define COIN_HPP

#include "Engine/Sprite.hpp"

class PlayScene;

class Coin : public Engine::Sprite {
private:
    PlayScene* playScene;
    int value;

public:
    Coin(PlayScene* scene, float x, float y, int val);
    void Update(float deltaTime) override;
};

#endif // COIN_HPP