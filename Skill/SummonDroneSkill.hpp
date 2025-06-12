#pragma once
#include "SkillBase.hpp"

class SummonDroneSkill : public SkillBase {
public:
    int droneCount;
    float droneLife, droneRadius, droneCooldown;

    SummonDroneSkill(int count, float life, float radius, float cooldown);
    void SkillAnimation() override;
};
