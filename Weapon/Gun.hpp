
#ifndef GUN_HPP
#define GUN_HPP
#include <string>
#include "Engine/IControl.hpp"
#include "Engine/Sprite.hpp"
#include "Engine/Point.hpp"

namespace Engine {
    class IScene; // Forward declaration

    class Gun : public Sprite, public IControl {
    private:
        IScene* scene; // Pointer to the current scene for adding bullets
        float fireCooldown; // Cooldown time between shots (seconds)
        float cooldownTimer; // Current cooldown timer
        Point characterPosition; // Position of the character holding the gun

    public:
        bool isActive; // Added to track if the gun is active
        explicit Gun(IScene* scene, Point characterPos);
        void Update(float deltaTime) override;
        void OnMouseMove(int mx, int my) override;
        void OnMouseDown(int button, int mx, int my) override;
        void OnMouseUp(int button, int mx, int my) override;
        void SetCharacterPosition(Point pos);
    
    };
}
#endif // GUN_HPP