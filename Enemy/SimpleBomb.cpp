#include "SimpleBomb.hpp"
#include "Engine/GameEngine.hpp"
#include "Scene/PlayScene.hpp"
#include "Engine/AudioHelper.hpp"
#include "UI/Animation/ExplosionEffect.hpp"
#include <iostream>

SimpleBomb::SimpleBomb(std::string img, float x, float y, float radius, float damage, float delay, Engine::Point velocity) 
    : Bomb(img, x, y, radius, damage, delay, velocity) {
}

void SimpleBomb::Explode() {
    exploded = true;
    PlayScene* scene = getPlayScene();
    float distance = (scene->character->Position - Position).Magnitude();
    if (distance <= explosionRadius) {
        scene->Hit(explosionDamage); // 對玩家造成傷害
    }
    // 添加爆炸效果
    scene->EffectGroup->AddNewObject(new ExplosionEffect(Position.x, Position.y));
    // 播放音效
    //AudioHelper::PlayAudio("explosion.wav");
    // 移除炸彈
    getPlayScene()->RemoveObject(objectIterator);
}