#ifndef DRONE_HPP
#define DRONE_HPP
#pragma once
#include "Turret/Turret.hpp"

class Drone : public Turret {
public:
    float lifeTime;
    Engine::Point followOffset = Engine::Point(0, 0);

    Drone(float x, float y, float radius, float cooldown, float lifeTime, Engine::Point followOffset);

    void Update(float deltaTime) override;
    void CreateBullet() override;
};
#endif