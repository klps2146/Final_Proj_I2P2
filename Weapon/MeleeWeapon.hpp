#ifndef MELEEWEAPON_HPP
#define MELEEWEAPON_HPP
#include <string>
#include "Engine/IControl.hpp"
#include "Engine/Sprite.hpp"
#include "Engine/Point.hpp"

namespace Engine {
    class IScene; // Forward declaration

    class MeleeWeapon : public Sprite, public IControl {
    private:
        IScene* scene; // Pointer to the current scene for enemy interaction
        float attackCooldown; // Cooldown time between attacks (seconds)
        float cooldownTimer; // Current cooldown timer
        Point characterPosition; // Position of the character holding the weapon
        float attackRange; // Range of the melee attack
        int damage; // Damage dealt to enemies
        bool isAttacking; // Flag to indicate attack animation
        float attackAngle; // Angle for attack animation
         bool isFlipped; // 判斷角色是否面向左側
    public:
    bool isActive; // Added to track if the melee weapon is active
        explicit MeleeWeapon(IScene* scene, Point characterPos);
        void Update(float deltaTime) override;
        void OnMouseMove(int mx, int my) override;
        void OnMouseDown(int button, int mx, int my) override;
        void OnMouseUp(int button, int mx, int my) override;
        void SetCharacterPosition(Point pos);
        void Attack(); // Trigger the melee attack
    };
}
#endif // MELEEWEAPON_HPP



// #ifndef MELEEWEAPON_HPP
// #define MELEEWEAPON_HPP

// #include <string>
// #include "Engine/IControl.hpp"
// #include "Engine/Sprite.hpp"
// #include "Engine/Point.hpp"

// namespace Engine {
//     class IScene; // Forward declaration

//     class MeleeWeapon : public Sprite, public IControl {
//     private:
//         IScene* scene; // Pointer to the current scene for enemy interaction
//         float attackCooldown; // Cooldown time between attacks (seconds)
//         float cooldownTimer; // Current cooldown timer
//         Point characterPosition; // Position of the character holding the weapon
//         float attackRange; // Range of the melee attack
//         int damage; // Damage dealt to enemies
//         bool isAttacking; // Flag to indicate attack animation
//         float attackAngle; // Angle for attack animation
//         bool flip; // Flag to indicate if the weapon should be flipped horizontally

//     public:
//         explicit MeleeWeapon(IScene* scene, Point characterPos);
//         void Update(float deltaTime) override;
//         void OnMouseMove(int mx, int my) override;
//         void OnMouseDown(int button, int mx, int my) override;
//         void OnMouseUp(int button, int mx, int my) override;
//         void SetCharacterPosition(Point pos);
//         void Attack();
//         void Draw() const override; // Override Draw to support flipping
//     };
// }

// #endif // MELEEWEAPON_HPP