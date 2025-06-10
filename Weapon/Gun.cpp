#include "Gun.hpp"
#include <allegro5/allegro.h>
#include <cmath>
#include "Engine/GameEngine.hpp"
#include "Engine/IScene.hpp"
#include "Engine/Point.hpp"
#include "BulletBoom.hpp" // Assume this is a simple bullet class

namespace Engine {
    Gun::Gun(IScene* scene, Point characterPos) 
        : Sprite("play/Weapon1.png", characterPos.x, characterPos.y+10, 50, 28, 0.5f, 0.5f),
          scene(scene), fireCooldown(0.1f), cooldownTimer(0.0f), characterPosition(characterPos) {
    }

    void Gun::Update(float deltaTime) {
        Sprite::Update(deltaTime); // Update position if velocity exists
        cooldownTimer = std::max(0.0f, cooldownTimer - deltaTime); // Decrease cooldown

        // Check if left mouse button is held
        ALLEGRO_MOUSE_STATE mouseState;
        al_get_mouse_state(&mouseState);
        if (mouseState.buttons & 1 && cooldownTimer <= 0.0f) { // Left button pressed
            // Create a bullet at the gun's tip
            float bulletSpeed = 400.0f;
            Point bulletDir(std::cos(Rotation), std::sin(Rotation));
            Point bulletPos = Position + bulletDir * 30.0f; // Offset to gun tip
            BulletBoom* bullet = new BulletBoom(bulletPos.x, bulletPos.y, bulletDir * bulletSpeed,10);
            scene->AddNewObject(bullet); // Add bullet to scene
            cooldownTimer = fireCooldown; // Reset cooldown
        }
    }

    void Gun::OnMouseMove(int mx, int my) {
        // Rotate gun toward mouse position
        Point mousePos(mx, my);
        Point direction = mousePos - characterPosition;
        Rotation = std::atan2(direction.y, direction.x); // Set rotation in radians
    }

    void Gun::OnMouseDown(int button, int mx, int my) {
        // No immediate action needed; firing is handled in Update
    }

    void Gun::OnMouseUp(int button, int mx, int my) {
        // No action needed; firing stops when button is released
    }

    void Gun::SetCharacterPosition(Point pos) {
        characterPosition = pos;
        Position.x = pos.x; // Update gun position to match character
        Position.y = pos.y+10;
    }
}