#ifndef SHIELDSKILL_HPP
#define SHIELDSKILL_HPP
#include "SkillBase.hpp"

class ShieldSkill : public SkillBase {
public:
    ShieldSkill();
    void SkillAnimation() override;
};

#endif