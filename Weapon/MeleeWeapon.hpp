#ifndef MELEEWEAPON_HPP
#define MELEEWEAPON_HPP
#include "Weapon.hpp"
#include "Engine/IScene.hpp"
#include "Engine/Point.hpp"

namespace Engine {
    class MeleeWeapon : public Weapon {
    private:
        IScene* scene;
        float attackCooldown;
        float cooldownTimer;
        Point characterPosition;
        float attackRange;
        int damage;
        bool isAttacking;
        float attackAngle;
    public:
        explicit MeleeWeapon(IScene* scene, Point characterPos);
        void Update(float deltaTime) override;
        void OnMouseMove(int mx, int my) override;
        void OnMouseDown(int button, int mx, int my) override;
        void OnMouseUp(int button, int mx, int my) override;
        void SetCharacterPosition(Point pos) override;
        void Draw() const override;
        void Attack();
    };
}
#endif