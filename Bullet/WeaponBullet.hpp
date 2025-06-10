#ifndef WEAPON_BULLET_HPP
#define WEAPON_BULLET_HPP

#include <string>
#include "Engine/Point.hpp"
#include "Engine/Sprite.hpp"

class Enemy;
class Weapon;  // 前置聲明
class PlayScene;

class WeaponBullet : public Engine::Sprite {
public:
    WeaponBullet(std::string img, float speed, float damage, Engine::Point position, Engine::Point forwardDirection, float rotation, Weapon* parent);
    void Update(float deltaTime) override;
    virtual void OnExplode(Enemy* enemy);  // 虛函數，子類可覆蓋

protected:
    float speed;        // 子彈速度
    float damage;       // 子彈傷害
    Weapon* parent;     // 發射子彈的武器
    PlayScene* getPlayScene();  // 獲取當前遊戲場景
};

#endif