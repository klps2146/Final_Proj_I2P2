#include "coin.hpp"
#include "Scene/PlayScene.hpp"
#include "Engine/Point.hpp"

Coin::Coin(PlayScene* scene, float x, float y, int val) 
    : Sprite("play/coin.png", x, y), playScene(scene), value(val) {}

void Coin::Update(float deltaTime) {
    Engine::Point playerPos = playScene->character->Position;
    float distance = (Position - playerPos).Magnitude();
    if (distance < 50) {
        playScene->EarnMoney(value);
        playScene->GroundEffectGroup->RemoveObject(objectIterator);
    }
}