#ifndef FIRE_WEAPON_BULLET_HPP
#define FIRE_WEAPON_BULLET_HPP

#include "WeaponBullet.hpp"
#include "Engine/Point.hpp"

class Weapon;  // 前置聲明

class FireWeaponBullet : public WeaponBullet {
public:
    FireWeaponBullet(Engine::Point position, Engine::Point forwardDirection, float rotation, Weapon* parent);
    void OnExplode(Enemy* enemy) override;  // 覆蓋爆炸效果
};

#endif