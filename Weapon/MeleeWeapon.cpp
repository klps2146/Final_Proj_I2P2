
#include "MeleeWeapon.hpp"
#include <allegro5/allegro.h>
#include <cmath>
#include "Engine/GameEngine.hpp"
#include "Engine/IScene.hpp"
#include "Engine/Point.hpp"
#include "Scene/PlayScene.hpp"
#include "Enemy/Enemy.hpp"

namespace Engine {
    MeleeWeapon::MeleeWeapon(IScene* scene, Point characterPos) 
        : Weapon("play/Sword.png", characterPos.x, characterPos.y + 10, 160, 160, 0.5f, 0.5f),
          scene(scene), attackCooldown(0.5f), cooldownTimer(0.0f), characterPosition(characterPos),
          attackRange(150.0f), damage(30), isAttacking(false), attackAngle(0.0f) {
    }

    void MeleeWeapon::Update(float deltaTime) {
        Sprite::Update(deltaTime);
        cooldownTimer = std::max(0.0f, cooldownTimer - deltaTime);
        if (!isActive) return;
        if (isAttacking) {
            attackAngle += 10.0f * deltaTime;
            Rotation = attackAngle;
            if (attackAngle >= 2 * ALLEGRO_PI) {
                isAttacking = false;
                Rotation = 0.0f;
            }
        }
    }

    void MeleeWeapon::OnMouseMove(int mx, int my) {
        if (!isActive) return;
        PlayScene* playScene = dynamic_cast<PlayScene*>(scene);
        Point cameraPos = playScene->CameraPos;
        Point mouseWorldPos(mx + cameraPos.x, my + cameraPos.y);
        Point direction = mouseWorldPos - characterPosition;
        if (!isAttacking) Rotation = std::atan2(direction.y, direction.x);
    }

    void MeleeWeapon::OnMouseDown(int button, int mx, int my) {
        if (!isActive) return;
        if (button == 1 && cooldownTimer <= 0.0f) {
            Attack();
        }
    }

    void MeleeWeapon::OnMouseUp(int button, int mx, int my) {
        // No action needed
    }

    void MeleeWeapon::SetCharacterPosition(Point pos) {
        characterPosition = pos;
        Position.x = pos.x;
        Position.y = pos.y + 10;
    }

    void MeleeWeapon::Attack() {
        isAttacking = true;
        attackAngle = 0.0f;
        cooldownTimer = attackCooldown;
        auto* scene = dynamic_cast<PlayScene*>(GameEngine::GetInstance().GetActiveScene());
        for (auto& it : scene->EnemyGroup->GetObjects()) {
            Enemy* enemy = dynamic_cast<Enemy*>(it);
            if (!enemy || !enemy->Visible) continue;
            Point enemyPos = enemy->Position;
            float distance = (enemyPos - Position).Magnitude();
            if (distance <= attackRange) {
                enemy->Hit(damage, true);
            }
        }
    }

    void MeleeWeapon::Draw() const {
        if (isActive) Sprite::Draw();
    }
}