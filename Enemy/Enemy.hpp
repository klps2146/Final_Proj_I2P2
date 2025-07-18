#ifndef ENEMY_HPP
#define ENEMY_HPP
#include <list>
#include <string>
#include <vector>
#include "Engine/Point.hpp"
#include "Engine/Sprite.hpp"
class Bullet;
class PlayScene;
class Turret;
class Enemy : public Engine::Sprite {
protected:
    float speed;
    bool speed_changed = false;
    float original_speed = 0;
    float speed_timer = 0;
    float speed_duration = 0;
    float hp;
    float max_hp;
    int money;
    PlayScene *getPlayScene() const;
    virtual void OnExplode();

    void DrawBars() const;

    Engine::Point currentDirection;
public:
    virtual void HandlePlayerCollision();
    bool Playerhit = false;
    std::list<Turret *> lockedTurrets;
    std::list<Bullet *> lockedBullets;
    Enemy(std::string img, float x, float y, float radius, float speed, float hp, int money);
    
    
    
    //void Hit(float damage,bool byplayer);
    //void Update(float deltaTime) override;
    //void Draw() const override;
    
    virtual void Hit(float damage, bool byplayer); // Ensure Hit is virtual
    virtual void Update(float deltaTime) override;
    virtual void Draw() const override;
    
    void change_speed(float dv_mul, float duration);
    float dmg;
};
#endif  
