#ifndef ORBITBULLETSKILL_HPP
#define ORBITBULLETSKILL_HPP

#include "SkillBase.hpp"

class OrbitSkill : public SkillBase {
public:
    OrbitSkill(float x, float y);
    void SkillAnimation() override;
};

#endif
