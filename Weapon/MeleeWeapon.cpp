#include "MeleeWeapon.hpp"
#include <allegro5/allegro.h>
#include <cmath>
#include "Engine/GameEngine.hpp"
#include "Engine/IScene.hpp"
#include "Engine/Point.hpp"
#include "Scene/PlayScene.hpp" // Access PlayScene for enemy interaction
#include "Enemy/Enemy.hpp"

namespace Engine {
    MeleeWeapon::MeleeWeapon(IScene* scene, Point characterPos) 
        : Sprite("play/Sword.png", characterPos.x, characterPos.y + 10, 160, 160, 0.5f, 0.5f),
          scene(scene), attackCooldown(0.5f), cooldownTimer(0.0f), characterPosition(characterPos),
          attackRange(150.0f), damage(30), isAttacking(false), attackAngle(0.0f),isActive(false) {
    }

    // void MeleeWeapon::Update(float deltaTime) {
    //     Sprite::Update(deltaTime);
    //     cooldownTimer = std::max(0.0f, cooldownTimer - deltaTime);

    //     if (isAttacking) {
    //         // Simple attack animation: rotate the sword
    //         attackAngle += 2.0f * deltaTime; // Rotate at 2 radians per second
    //         Rotation = attackAngle;
    //         if (attackAngle >= 1.0f) { // Stop after a short swing (approx 60 degrees)
    //             isAttacking = false;
    //             Rotation = 0.0f; // Reset rotation
    //         }
    //     }
    // }

    void MeleeWeapon::Update(float deltaTime) {
        Sprite::Update(deltaTime);
        cooldownTimer = std::max(0.0f, cooldownTimer - deltaTime);
        if (!isActive) return;
        if (isAttacking) {
            // 加快旋轉速度，例如 10 弧度/秒（大約573度/秒）
            attackAngle += 10.0f * deltaTime;
            Rotation = attackAngle;

            // 2π 弧度 = 360 度
            if (attackAngle >= 2 * ALLEGRO_PI) {
                isAttacking = false;
                Rotation = 0.0f; // 重設旋轉角度
            }
        }
    }   

    void MeleeWeapon::OnMouseMove(int mx, int my) {
      if (!isActive) return;
        PlayScene* playScene = dynamic_cast<PlayScene*>(scene);
        Point cameraPos = playScene->CameraPos;
        Point mouseWorldPos(mx + cameraPos.x, my + cameraPos.y);

        // Rotate weapon toward mouse position (optional, can be removed if not needed)
        Point direction = mouseWorldPos - characterPosition;
        if (!isAttacking) Rotation = std::atan2(direction.y, direction.x);
    }

    void MeleeWeapon::OnMouseDown(int button, int mx, int my) {
       if (!isActive) return;
        if (button == 1 && cooldownTimer <= 0.0f) { // Left mouse button
            Attack();
        }
    }

    void MeleeWeapon::OnMouseUp(int button, int mx, int my) {
        // No action needed
        if (!isActive) return;
    }

    void MeleeWeapon::SetCharacterPosition(Point pos) {
        characterPosition = pos;
        Position.x = pos.x;
        Position.y = pos.y ;
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
}

// #include "MeleeWeapon.hpp"
// #include <allegro5/allegro.h>
// #include <cmath>
// #include "Engine/GameEngine.hpp"
// #include "Engine/IScene.hpp"
// #include "Engine/Point.hpp"
// #include <allegro5/allegro.h>
// #include <allegro5/allegro_image.h>
// #include <allegro5/allegro_font.h>
// #include <allegro5/allegro_color.h>
// #include <allegro5/allegro_primitives.h>
// #include "Scene/PlayScene.hpp"
// #include "Enemy/Enemy.hpp"

// namespace Engine {
//     MeleeWeapon::MeleeWeapon(IScene* scene, Point characterPos) 
//         : Sprite("play/Sword.png", characterPos.x, characterPos.y + 10, 128, 128, 0.1f, 0.5f),
//           scene(scene), attackCooldown(0.5f), cooldownTimer(0.0f), characterPosition(characterPos),
//           attackRange(50.0f), damage(30), isAttacking(false), attackAngle(0.0f), flip(false) {
//     }

//     void MeleeWeapon::Update(float deltaTime) {
//         Sprite::Update(deltaTime);
//         cooldownTimer = std::max(0.0f, cooldownTimer - deltaTime);

//         if (isAttacking) {
//             attackAngle += 4.0f * deltaTime; // 加快旋轉速度
//             Rotation = attackAngle;
//             if (attackAngle >= 3.14f) { // 旋轉 180 度
//                 isAttacking = false;
//                 Rotation = 0.0f; // 重置旋轉
//             }
//         }
//     }

//     void MeleeWeapon::OnMouseMove(int mx, int my) {
//         PlayScene* playScene = dynamic_cast<PlayScene*>(scene);
//         Point cameraPos = playScene->CameraPos;
//         Point mouseWorldPos(mx + cameraPos.x, my + cameraPos.y);

//         Point direction = mouseWorldPos - characterPosition;
//         if (!isAttacking) {
//             Rotation = std::atan2(direction.y, direction.x);
//             // 根據滑鼠位置判斷是否翻轉刀面
//             flip = (mouseWorldPos.x < characterPosition.x);
//         }
//     }

//     void MeleeWeapon::OnMouseDown(int button, int mx, int my) {
//         if (button == 1 && cooldownTimer <= 0.0f) {
//             Attack();
//         }
//     }

//     void MeleeWeapon::OnMouseUp(int button, int mx, int my) {
//         // 無需動作
//     }

//     void MeleeWeapon::SetCharacterPosition(Point pos) {
//         characterPosition = pos;
//         Position.x = pos.x;
//         Position.y = pos.y + 10;
//     }

//     void MeleeWeapon::Attack() {
//         isAttacking = true;
//         attackAngle = 0.0f;
//         cooldownTimer = attackCooldown;

//         auto* scene = dynamic_cast<PlayScene*>(GameEngine::GetInstance().GetActiveScene());
//         for (auto& it : scene->EnemyGroup->GetObjects()) {
//             Enemy* enemy = dynamic_cast<Enemy*>(it);
//             if (!enemy || !enemy->Visible) continue;
//             Point enemyPos = enemy->Position;
//             float distance = (enemyPos - Position).Magnitude();
//             if (distance <= attackRange) {
//                 enemy->Hit(damage, true);
//             }
//         }
//     }

//     void MeleeWeapon::Draw() const {
//         if (flip) {
//             al_draw_scaled_rotated_bitmap(bmp.get(), Anchor.x * GetBitmapWidth(), Anchor.y * GetBitmapHeight(),
//                         Position.x, Position.y, Size.x / GetBitmapWidth(), Size.y / GetBitmapHeight(), 
//                         Rotation, ALLEGRO_FLIP_HORIZONTAL);
//         } else {
//             Sprite::Draw();
//         }
//     }
// }


// #include "MeleeWeapon.hpp"
// #include <allegro5/allegro.h>
// #include <cmath>
// #include "Engine/GameEngine.hpp"
// #include "Engine/IScene.hpp"
// #include "Engine/Point.hpp"
// #include "Scene/PlayScene.hpp"
// #include "Enemy/Enemy.hpp"

// namespace Engine {
//     MeleeWeapon::MeleeWeapon(IScene* scene, Point characterPos) 
//         : Sprite("play/Sword.png", characterPos.x, characterPos.y + 10, 64, 64, 0.1f, 0.5f),
//           scene(scene), attackCooldown(0.5f), cooldownTimer(0.0f), characterPosition(characterPos),
//           attackRange(50.0f), damage(30), isAttacking(false), attackAngle(0.0f), flip(false) {
//     }

//     void MeleeWeapon::Update(float deltaTime) {
//         Sprite::Update(deltaTime);
//         cooldownTimer = std::max(0.0f, cooldownTimer - deltaTime);

//         if (isAttacking) {
//             attackAngle += 4.0f * deltaTime; // Faster rotation for attack
//             Rotation = attackAngle;
//             if (attackAngle >= 3.14f) { // 180-degree swing
//                 isAttacking = false;
//                 Rotation = 0.0f; // Reset rotation
//             }
//         }
//     }

//     void MeleeWeapon::OnMouseMove(int mx, int my) {
//         PlayScene* playScene = dynamic_cast<PlayScene*>(scene);
//         Point cameraPos = playScene->CameraPos;
//         Point mouseWorldPos(mx + cameraPos.x, my + cameraPos.y);

//         Point direction = mouseWorldPos - characterPosition;
//         if (!isAttacking) {
//             Rotation = std::atan2(direction.y, direction.x);
//             flip = (mouseWorldPos.x < characterPosition.x); // Flip if mouse is left of character
//         }
//     }

//     void MeleeWeapon::OnMouseDown(int button, int mx, int my) {
//         if (button == 1 && cooldownTimer <= 0.0f) {
//             Attack();
//         }
//     }

//     void MeleeWeapon::OnMouseUp(int button, int mx, int my) {
//         // No action needed
//     }

//     void MeleeWeapon::SetCharacterPosition(Point pos) {
//         characterPosition = pos;
//         Position.x = pos.x-20;
//         Position.y = pos.y + 10; // Keep weapon slightly below character
//     }

//     void MeleeWeapon::Attack() {
//         isAttacking = true;
//         attackAngle = 0.0f;
//         cooldownTimer = attackCooldown;

//         auto* scene = dynamic_cast<PlayScene*>(GameEngine::GetInstance().GetActiveScene());
//         for (auto& it : scene->EnemyGroup->GetObjects()) {
//             Enemy* enemy = dynamic_cast<Enemy*>(it);
//             if (!enemy || !enemy->Visible) continue;
//             Point enemyPos = enemy->Position;
//             float distance = (enemyPos - Position).Magnitude();
//             if (distance <= attackRange) {
//                 enemy->Hit(damage, true);
//             }
//         }
//     }

//     void MeleeWeapon::Draw() const {
//         if (flip) {
//             al_draw_scaled_rotated_bitmap(bmp.get(), Anchor.x * GetBitmapWidth(),Anchor.y * GetBitmapWidth(),
//                         Position.x, Position.y, 1, 1,  Rotation, ALLEGRO_FLIP_HORIZONTAL);
//             //al_draw_bitmap(bmp.get(), characterPosition.x, characterPosition.y,ALLEGRO_FLIP_HORIZONTAL);
//         } else {
//             Sprite::Draw(); // Draw normally without flip
//         }

//     }   
// }
