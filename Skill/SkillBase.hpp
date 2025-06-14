#ifndef SKILLBASE_HPP
#define SKILLBASE_HPP
#include <string>
#include <queue>
#include <utility>
#include "Engine/SpriteFixed.hpp"

class PlayScene;

class SkillBase : public Engine::SpriteFixed {
protected:
    std::string name;
    float CDTimer;
    float maxCD;
    

    float PowerExpense = 0;

public:
    bool isUnlocked;
    int level = 1;
    int MaxLevel = 5;
    std::queue<int> upgradeExpenseMoney; 

    SkillBase(const std::string& name, const std::string& img, float x, float y, float maxCD, float powerExpense);

    void Update(float deltaTime) override;
    void Draw() const override;
    virtual void SkillAnimation();
    virtual void Activate();

    int getLevel();

    bool IsReady() const;
    float GetCooldownRatio() const;
    std::string GetName() const;
    void Unlock();
    bool getUnlock();
    bool consumePower();

    static PlayScene* getPlayScene();
};
#endif