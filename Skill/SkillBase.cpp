#include "SkillBase.hpp"
#include "Scene/PlayScene.hpp"
#include "Engine/GameEngine.hpp"

SkillBase::SkillBase(const std::string& name, const std::string& img, float x, float y, float maxCD)
    : Engine::SpriteFixed(img, x, y), name(name), CDTimer(0), maxCD(maxCD), isUnlocked(false) {
}

void SkillBase::Update(float deltaTime) {
    if (CDTimer > 0) {
        CDTimer -= deltaTime;
        if (CDTimer < 0) CDTimer = 0;
    }
    Engine::SpriteFixed::Update(deltaTime);
}

void SkillBase::Draw() const {
    Engine::SpriteFixed::Draw();
}

bool SkillBase::IsReady() const {
    return CDTimer <= 0 && isUnlocked;
}

float SkillBase::GetCooldownRatio() const {
    if (maxCD == 0) return 0;
    return CDTimer / maxCD;
}

std::string SkillBase::GetName() const {
    return name;
}

void SkillBase::Unlock() {
    isUnlocked = true;
}

void SkillBase::SkillAnimation() {
    
}

void SkillBase::Activate() {
    if (IsReady()) {
        CDTimer = maxCD;
        SkillAnimation();
    }
}

int SkillBase::getLevel(){
    return level;
}


PlayScene* SkillBase::getPlayScene() {
    return dynamic_cast<PlayScene*>(Engine::GameEngine::GetInstance().GetActiveScene());
}
