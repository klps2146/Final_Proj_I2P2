#ifndef SKILLBASE_HPP
#define SKILLBASE_HPP
#include <string>
#include "Engine/Sprite.hpp"

class PlayScene;

class SkillBase : public Engine::Sprite {
protected:
    std::string name;
    float CDTimer;
    float maxCD;
    bool isUnlocked;

public:
    SkillBase(const std::string& name, const std::string& img, float x, float y, float maxCD);

    void Update(float deltaTime) override;
    void Draw() const override;
    virtual void SkillAnimation();  // 可由子類別實作
    virtual void Activate();        // 技能效果發動時呼叫

    bool IsReady() const;
    float GetCooldownRatio() const;
    std::string GetName() const;
    void Unlock();

    static PlayScene* getPlayScene();
};
#endif