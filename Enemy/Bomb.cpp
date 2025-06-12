#include "Bomb.hpp"
#include "Engine/GameEngine.hpp"
#include "Scene/PlayScene.hpp"
#include "Engine/AudioHelper.hpp"
#include "UI/Animation/ExplosionEffect.hpp"

PlayScene* Bomb::getPlayScene() {
    return dynamic_cast<PlayScene*>(Engine::GameEngine::GetInstance().GetActiveScene());
}

Bomb::Bomb(std::string img, float x, float y, float radius, float damage, float delay) 
    : Sprite(img, x, y), explosionRadius(radius), explosionDamage(damage), explosionDelay(delay), timer(0), exploded(false) {
}

void Bomb::Update(float deltaTime) {
    timer += deltaTime;
    if (timer >= explosionDelay && !exploded) {
        Explode();
    }
    Sprite::Update(deltaTime);
}

void Bomb::Explode() {
    exploded = true;
    PlayScene* scene = getPlayScene();
    float distance = (scene->character->Position - Position).Magnitude();
    /*if (distance <= explosionRadius) {
        scene->Hit(explosionDamage); // 對玩家造成傷害
    }*/
    // 添加爆炸效果
    scene->EffectGroup->AddNewObject(new ExplosionEffect(Position.x, Position.y));
    // 播放音效
    //AudioHelper::PlayAudio("explosion.wav");
    // 移除炸彈
    getPlayScene()->EnemyGroup->RemoveObject(objectIterator);
}