

#include "Gun.hpp"
#include <allegro5/allegro.h>
#include <cmath>
#include "Engine/GameEngine.hpp"
#include "Engine/IScene.hpp"
#include "Engine/Point.hpp"
#include "BulletBoom.hpp"
#include "Engine/AudioHelper.hpp"
#include "Scene/PlayScene.hpp"

namespace Engine {
    Gun::Gun(IScene* scene, Point characterPos) 
        : Weapon("play/Weapon1.png", characterPos.x, characterPos.y + 10, 50, 28, 0.5f, 0.5f),
          scene(scene), fireCooldown(0.1f), cooldownTimer(0.0f), characterPosition(characterPos) {
    }

    void Gun::Update(float deltaTime) {
        Sprite::Update(deltaTime);
        cooldownTimer = std::max(0.0f, cooldownTimer - deltaTime);
        if (!isActive) return;
        ALLEGRO_MOUSE_STATE mouseState;
        al_get_mouse_state(&mouseState);
        if (mouseState.buttons & 1 && cooldownTimer <= 0.0f) {
            float bulletSpeed = 400.0f;
            Point bulletDir(std::cos(Rotation), std::sin(Rotation));
            Point bulletPos = Position + bulletDir * 30.0f;
            BulletBoom* bullet = new BulletBoom(bulletPos.x, bulletPos.y, bulletDir * bulletSpeed, 30);
            scene->AddNewObject(bullet);
            AudioHelper::PlayAudio("laser.wav");
            cooldownTimer = fireCooldown;
        }
    }

    void Gun::OnMouseMove(int mx, int my) {
        if (!isActive) return;
        PlayScene* playScene = dynamic_cast<PlayScene*>(scene);
        Point cameraPos = playScene->CameraPos;
        Point mouseWorldPos(mx + cameraPos.x, my + cameraPos.y);
        Point direction = mouseWorldPos - characterPosition;
        Rotation = std::atan2(direction.y, direction.x);
    }

    void Gun::OnMouseDown(int button, int mx, int my) {
        // Handled in Update
    }

    void Gun::OnMouseUp(int button, int mx, int my) {
        // No action needed
    }

    void Gun::SetCharacterPosition(Point pos) {
        characterPosition = pos;
        Position.x = pos.x;
        Position.y = pos.y + 10;
    }

    void Gun::Draw() const {
        if (isActive) Sprite::Draw();
    }
}