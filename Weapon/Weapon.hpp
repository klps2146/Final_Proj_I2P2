// Weapon.hpp
#ifndef WEAPON_HPP
#define WEAPON_HPP

#include <string>
#include "Engine/Sprite.hpp"
#include "Engine/Point.hpp"

class PlayScene;

class Weapon : public Engine::Sprite {
public:
    Weapon(std::string img, float x, float y, float coolDown);
    void Update(float deltaTime) override;
    void Draw() const override;
    void Fire(); // 發射子彈
protected:
    virtual void CreateBullet() = 0; // 純虛函數，讓子類實現具體的子彈創建
    PlayScene* getPlayScene();
private:
    float coolDown; // 冷卻時間
    float reload;   // 當前冷卻計時器
};

#endif