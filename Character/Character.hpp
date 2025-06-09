#ifndef CHARACTER_HPP
#define CHARACTER_HPP
#include "Engine/Sprite.hpp"
#include "Engine/IControl.hpp"

namespace Engine {
    class Character : public Sprite, public IControl {
    private:
        // Movement speed in pixels per second
        float speed;
        // Current movement direction (based on input)
        Point direction;
        // Flag to track if the character is moving
        bool isMoving;

        int tick = 0;

    public:
        // Constructor
        Character(std::string img, float x, float y, float w = 0, float h = 0,
                 float anchorX = 0.5f, float anchorY = 0.5f, float speed = 200.0f,
                 float collisionRadius = 16.0f);
        
        // Override IControl methods for keyboard input
        void OnKeyDown(int keyCode) override;
        void OnKeyUp(int keyCode) override;
        
        // Override Update to handle movement
        void Update(float deltaTime) override;
    };
}
#endif //