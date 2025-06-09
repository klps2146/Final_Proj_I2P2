#include "Character.hpp"
#include <allegro5/allegro.h>
#include "Engine/GameEngine.hpp"
#include <iostream>


namespace Engine {
    Character::Character(std::string img, float x, float y, float w, float h,
                       float anchorX, float anchorY, float speed, float collisionRadius)
        : Sprite(img, x, y, w, h, anchorX, anchorY, 0, 0, 0, 255, 255, 255, 255),
          speed(speed), direction(0, 0), isMoving(false) {
        this->CollisionRadius = collisionRadius;
        std::cout << "Created Character at (" << x << ", " << y << ") with speed " << speed;
    }

    void Character::OnKeyDown(int keyCode) {
        // Handle WASD and arrow keys for movement
        switch (keyCode) {
            case ALLEGRO_KEY_W:
            case ALLEGRO_KEY_UP:
                direction.y = -1; // Move up
                isMoving = true;
                break;
            case ALLEGRO_KEY_S:
            case ALLEGRO_KEY_DOWN:
                direction.y = 1;  // Move down
                isMoving = true;
                break;
            case ALLEGRO_KEY_A:
            case ALLEGRO_KEY_LEFT:
                direction.x = -1; // Move left
                isMoving = true;
                break;
            case ALLEGRO_KEY_D:
            case ALLEGRO_KEY_RIGHT:
                direction.x = 1;  // Move right
                isMoving = true;
                break;
        }
        // Normalize direction to prevent faster diagonal movement
        if (isMoving && direction.Magnitude() > 0) {
            direction = direction.Normalize();
        }
        std::cout << "Key down: " << keyCode << ", Direction: (" << direction.x << ", " << direction.y << ")";
    }

    void Character::OnKeyUp(int keyCode) {
        // Reset direction components when keys are released
        switch (keyCode) {
            case ALLEGRO_KEY_W:
            case ALLEGRO_KEY_UP:
            case ALLEGRO_KEY_S:
            case ALLEGRO_KEY_DOWN:
                direction.y = 0;
                break;
            case ALLEGRO_KEY_A:
            case ALLEGRO_KEY_LEFT:
            case ALLEGRO_KEY_D:
            case ALLEGRO_KEY_RIGHT:
                direction.x = 0;
                break;
        }
        // Check if character is still moving
        isMoving = (direction.x != 0 || direction.y != 0);
        if (!isMoving) {
            Velocity = Point(0, 0);
        }
        std::cout << "Key up: " << keyCode << ", Direction: (" << direction.x << ", " << direction.y << ")";
    }

    void Character::Update(float deltaTime) {
        // Update velocity based on direction and speed
        if (isMoving) {
            Velocity = direction * speed;
        }
        // Update position using Sprite's Update (Position += Velocity * deltaTime)
        Sprite::Update(deltaTime);

        // Optional: Clamp position to screen boundaries
        Point screenSize = GameEngine::GetInstance().GetScreenSize();
        Position.x = std::max(CollisionRadius, std::min(screenSize.x - CollisionRadius, Position.x));
        Position.y = std::max(CollisionRadius, std::min(screenSize.y - CollisionRadius, Position.y));
    }
}