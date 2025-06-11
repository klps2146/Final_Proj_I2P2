#ifndef DASHSKILL_HPP
#define DASHSKILL_HPP
#include "SkillBase.hpp"

class DashSkill : public SkillBase {
public:
    DashSkill();
    void SkillAnimation() override;
};

#endif