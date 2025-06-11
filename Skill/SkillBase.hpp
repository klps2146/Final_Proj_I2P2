#ifndef SKILLBASE_HPP
#define SKILLBASE_HPP
#include <string>
#include "Engine/SpriteFixed.hpp"

class PlayScene;

class SkillBase : public Engine::SpriteFixed {
protected:
    std::string name;
    float CDTimer;
    float maxCD;
    bool isUnlocked;

    int level = 1;

public:
    SkillBase(const std::string& name, const std::string& img, float x, float y, float maxCD);

    void Update(float deltaTime) override;
    void Draw() const override;
    virtual void SkillAnimation();
    virtual void Activate();

    int getLevel();

    bool IsReady() const;
    float GetCooldownRatio() const;
    std::string GetName() const;
    void Unlock();

    static PlayScene* getPlayScene();
};
#endif