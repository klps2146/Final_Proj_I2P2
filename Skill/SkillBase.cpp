#include "SkillBase.hpp"
#include "Scene/PlayScene.hpp"
#include "Engine/GameEngine.hpp"
#include "Engine/Point.hpp"

SkillBase::SkillBase(const std::string& name, const std::string& img, float x, float y, float maxCD, float powerExpense)
    : Engine::SpriteFixed(img, x, y), name(name), CDTimer(0), maxCD(maxCD), isUnlocked(false), PowerExpense(powerExpense) {
    Position = Engine::Point(x, y);
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
        if (!consumePower()){
            getPlayScene()->SkillWarn->Text = "Not Enough Power";
            return; // 不夠
        }

        CDTimer = maxCD;
        SkillAnimation();
        getPlayScene()->SkillWarn->Text = "";
    }
    else if (!getUnlock()){
        getPlayScene()->SkillWarn->Text = "Please Unlock Fist At Store";
    }
}

int SkillBase::getLevel(){
    return level;
}


PlayScene* SkillBase::getPlayScene() {
    return dynamic_cast<PlayScene*>(Engine::GameEngine::GetInstance().GetActiveScene());
}

bool SkillBase::consumePower(){
    if (getPlayScene()){
        return getPlayScene()->character->ChangePOWER(-1 * PowerExpense);
    }
    return false;
}

bool SkillBase::getUnlock(){
    return isUnlocked;
}