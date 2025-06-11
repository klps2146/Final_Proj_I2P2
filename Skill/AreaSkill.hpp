#ifndef AREASKILL_HPP
#define AREASKILL_HPP

#include "SkillBase.hpp"
#include "Engine/Point.hpp"
#include "UI/Effect/ParticleSystemManager.hpp"
#include <allegro5/allegro_primitives.h>

class AreaSkill : public SkillBase {
protected:
    float radius;
    float effectDuration;
    float effectTimer;
    Engine::Point effectPosition;
    bool showingEffect;

    ParticleSystemManager particleSystem;

public:
    AreaSkill(const std::string& name, const std::string& img, float radius, float maxCD, float duration);
    void SkillAnimation() override;
    void Update(float deltaTime) override;
    void Draw() const override;
};

#endif
