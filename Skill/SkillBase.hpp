#ifndef SKILLBASE_HPP
#define SKILLBASE_HPP

#include <string>
#include <memory>
#include "Engine/Point.hpp"
#include "Engine/Sprite.hpp"


class PlayScene;

class SkillBase : public Engine::Sprite {
protected:
    std::string name;
    float CDTimer;
    float maxCD;
    bool isUnlocked;
    PlayScene *getPlayScene();
    
public:
    SkillBase(const std::string& name, const std::string& img, float x, float y, float maxCD);    
    virtual ~SkillBase() = default;

    virtual void Activate(Engine::Point target) = 0;
    virtual void Update(float deltaTime) override;
    virtual void Draw() const override;

    bool IsReady() const;
    void Unlock();

    std::string GetName() const;
    float GetCooldownRatio() const; // 回傳 0~1，UI 可用於畫進度條
};

#endif // SKILLBASE_HPP
