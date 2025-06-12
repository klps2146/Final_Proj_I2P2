#ifndef CHARACTER_HPP
#define CHARACTER_HPP
#include "Engine/Sprite.hpp"
#include "Engine/IControl.hpp"
#include "ItemBar/Itembar.hpp"
#include "Engine/Group.hpp"
#include "Skill/SkillBase.hpp"

class PlayScene;

namespace Engine {
    
    class Character : public Sprite, public IControl {
    private:
        float speedTimer = 0;
        float originalSpeed;

        Point direction;
        float rotation = 0.0f;

        bool isMoving;
        bool isDying;
        bool isDead;
        
        const float MAX_HP = 1000;
        const float MAX_POWER = 500;

        Engine::Point last_idle_dir;
        float frame_timer = 0.0f;
        int cur_frame = 0;
        const int max_frames = 7;
        const float frame_duration = 0.11f;

    public:
        float HP;
        float speed;
        float POWER;
        int VisableLevel;
    
        ItemBar itemBar_; // 裡面有 std::vector<SkillBase*> slots; 技能在裡面

        void AddSkill(SkillBase* skill);
        void UseSkill(int index);
        
        PlayScene *getPlayScene();
        Character(std::string img, float x, float y, float w = 0, float h = 0,
                 float anchorX = 0.5f, float anchorY = 0.5f, float speed = 200.0f,
                 float collisionRadius = 16.0f);
        
        void OnKeyDown(int keyCode) override;
        void OnKeyUp(int keyCode) override;
        
        void Update(float deltaTime) override;
        void DrawBars();
        void Draw() const override;
        bool IsAlive();
        void setTimer(float time);

        float GetRotation() const { return rotation; }
        void SetRotation(float angle) { rotation = angle; }
    };   
}
#endif //