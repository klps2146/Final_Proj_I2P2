#ifndef NUCLEARSKILL_HPP
#define NUCLEARSKILL_HPP
#include "SkillBase.hpp"

class NuclearSkill : public SkillBase {
public:
    NuclearSkill();
    void SkillAnimation() override;
};

#endif