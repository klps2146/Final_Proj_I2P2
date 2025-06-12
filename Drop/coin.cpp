#include "coin.hpp"
#include "Scene/PlayScene.hpp"
#include "Engine/Point.hpp"
#include "Engine/AudioHelper.hpp"
Coin::Coin(PlayScene* scene, float x, float y, int val) 
    : Sprite("play/coin.png", x, y,32,32), playScene(scene), value(val) {}

void Coin::Update(float deltaTime) {
    Engine::Point playerPos = playScene->character->Position;
    float distance = (Position - playerPos).Magnitude();
    if (distance < 50) {
        playScene->EarnMoney(value);
        //AudioHelper::SFXVolume = 5.0;  // 比預設 1.0 大，就會變大聲
        AudioHelper::PlaySample("GetCoin.wav", false, 5.0);
        //AudioHelper::SFXVolume = 1.0;  // 播放完後改回來，以免影響其他音效
        playScene->GroundEffectGroup->RemoveObject(objectIterator);
    }
}