#include "Character.hpp"
#include <allegro5/allegro.h>
#include "Engine/GameEngine.hpp"
#include "Engine/Collider.hpp"
#include "Scene/PlayScene.hpp"
#include <iostream>
#include <allegro5/allegro_primitives.h>
#include "UI/Component/Label.hpp"
#include <cassert>

PlayScene *Engine::Character::getPlayScene() {
    return dynamic_cast<PlayScene *>(Engine::GameEngine::GetInstance().GetActiveScene());
}

namespace Engine {
    const int BlockSize = 64;

    Character::Character(std::string img, float x, float y, float w, float h,
                       float anchorX, float anchorY, float speed, float collisionRadius)
        : Sprite(img, x, y, w, h, anchorX, anchorY, 0, 0, 0, 255, 255, 255, 255),
          speed(speed), direction(0, 0), isMoving(false) {
        this->CollisionRadius = collisionRadius;
        std::cout << "Created Character at (" << x << ", " << y << ") with speed " << speed << std::endl;
        SetSpriteSource(0, 0, 96, 96);
        isDying = 0;
        isDead = 0;
        HP = 99999;
        POWER = 500000;
        VisableLevel = 1;
    }

    void Character::AddWeapon(std::unique_ptr<Weapon> weapon) {
        weaponManager.AddWeapon(std::move(weapon));
    }

    void Character::SwitchWeapon(int index) {
        weaponManager.SwitchWeapon(index);
    }

    void Character::OnKeyDown(int keyCode) {
        if (keyCode == ALLEGRO_KEY_K) {
            isDying = 1;
            HP = 0;
            cur_frame = frame_timer = 0;
        } else if (keyCode == ALLEGRO_KEY_BACKSPACE) {
            HP -= 100;
            std::cout << "HURT: " << HP << std::endl;
        } else if (keyCode == ALLEGRO_KEY_SPACE) {
            speed += 100;
            std::cout << "SPEED: " << speed << std::endl;
        } else if (keyCode == ALLEGRO_KEY_Q) {
            SwitchWeapon(0); // Switch to gun
        } else if (keyCode == ALLEGRO_KEY_E) {
            SwitchWeapon(1); // Switch to melee weapon
        }
        itemBar_.OnKeyDown(keyCode);
        switch (keyCode) {
            case ALLEGRO_KEY_W:
            case ALLEGRO_KEY_UP:
                direction.y = -1;
                isMoving = true;
                break;
            case ALLEGRO_KEY_S:
            case ALLEGRO_KEY_DOWN:
                direction.y = 1;
                isMoving = true;
                break;
            case ALLEGRO_KEY_A:
            case ALLEGRO_KEY_LEFT:
                direction.x = -1;
                isMoving = true;
                break;
            case ALLEGRO_KEY_D:
            case ALLEGRO_KEY_RIGHT:
                direction.x = 1;
                isMoving = true;
                break;
        }
        if (isMoving && direction.Magnitude() > 0) {
            direction = direction.Normalize();
        }
    }

    void Character::OnKeyUp(int keyCode) {
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
        isMoving = (direction.x != 0 || direction.y != 0);
        if (!isMoving) {
            Velocity = Point(0, 0);
        }
    }

    void Character::Update(float deltaTime) {
        if (speedTimer > 0) {
            speedTimer -= deltaTime;
            if (speedTimer <= 0) {
                speedTimer = 0;
                speed = originalSpeed;
            }
        }
        itemBar_.Update(deltaTime);
        weaponManager.Update(deltaTime);

        if (!isDying) {
            if (isMoving) {
                Velocity = direction * speed;
                frame_timer += deltaTime;
                if (frame_timer >= frame_duration) {
                    frame_timer -= frame_duration;
                    cur_frame = (cur_frame + 1) % max_frames;
                    if (direction.x > 0) {
                        SetSpriteSource(cur_frame * 96, 0, 96, 96);
                        last_idle_dir = direction;
                    } else if (direction.x < 0) {
                        SetSpriteSource((7 - cur_frame) * 96, 96, 96, 96);
                        last_idle_dir = direction;
                    } else if (direction.x == 0) {
                        if (last_idle_dir.x >= 0) SetSpriteSource(cur_frame * 96, 0, 96, 96);
                        else SetSpriteSource((7 - cur_frame) * 96, 96, 96, 96);
                    }
                }
            } else {
                frame_timer += deltaTime;
                if (frame_timer >= frame_duration) {
                    frame_timer -= frame_duration;
                    cur_frame = (cur_frame + 1) % (max_frames + 1);
                    if (last_idle_dir.x >= 0)
                        SetSpriteSource(cur_frame * 96, 96 * 3, 96, 96);
                    else
                        SetSpriteSource(cur_frame * 96, 96 * 5, 96, 96);
                }
            }
        }
        
        if(HP <= 0 && !isDying) { // 快死了
            cur_frame = frame_timer = 0;
            isDying = 1;
        } else if (HP <= 0 && isDying) {
            Velocity = Engine::Point(0, 0);
            frame_timer += deltaTime;
            if (frame_timer >= (frame_duration + 0.13)) {
                frame_timer -= (frame_duration + 0.13);
                cur_frame = (cur_frame + 1);
                if (cur_frame > max_frames - 1) {
                    isDead = 1;
                }
                if (last_idle_dir.x >= 0)
                    SetSpriteSource(cur_frame * 96, 96 * 2, 96, 96);
                else
                    SetSpriteSource(cur_frame * 96, 96 * 4, 96, 96);
            }
        }
        if (isDead == 1) {
            std::cout << "YOU FUCKED UP" << std::endl;
            Engine::GameEngine::GetInstance().ChangeScene("lose");
        }

        Point newPosition = Position + Velocity * deltaTime;
        auto& mapState = getPlayScene()->mapState;
        int mapHeight = mapState.size();
        if (mapHeight == 0) return;
        int mapWidth = mapState[0].size();

        bool collisionDetected = false;
        int tileLeft = (int)((newPosition.x - CollisionRadius) / BlockSize);
        int tileRight = (int)((newPosition.x + CollisionRadius) / BlockSize);
        int tileTop = (int)((newPosition.y - CollisionRadius) / BlockSize);
        int tileBottom = (int)((newPosition.y + CollisionRadius) / BlockSize);

        for (int ty = tileTop; ty <= tileBottom; ++ty) {
            for (int tx = tileLeft; tx <= tileRight; ++tx) {
                if (tx < 0 || tx >= mapWidth || ty < 0 || ty >= mapHeight) {
                    collisionDetected = true;
                    break;
                }
                int tile = mapState[ty][tx];
                if (tile == getPlayScene()->TILE_WATER || tile == getPlayScene()->TILE_ROCK || tile == getPlayScene()->TILE_HOME) {
                    Point tileMin(tx * BlockSize, ty * BlockSize);
                    Point tileMax = tileMin + Point(BlockSize, BlockSize);
                    float closestX = std::max(tileMin.x, std::min(newPosition.x, tileMax.x));
                    float closestY = std::max(tileMin.y, std::min(newPosition.y, tileMax.y));
                    Point closestPoint(closestX, closestY);
                    Point CharCollisionBox = newPosition + Engine::Point(0, 16);
                    float effectiveRadius = (tile == getPlayScene()->TILE_ROCK) ? CollisionRadius - 12 : CollisionRadius - 12;
                    if (Engine::Collider::IsCircleOverlap(CharCollisionBox, effectiveRadius, closestPoint, 0)) {
                        collisionDetected = true;
                        break;
                    }
                }
            }
            if (collisionDetected) break;
        }

        if (!collisionDetected) {
            Position = newPosition;
            weaponManager.SetCharacterPosition(Position);
        } else {
            Velocity = Point(0, 0);
        }

        Point screenSize = GameEngine::GetInstance().GetScreenSize();
        getPlayScene()->CameraPos = Position - screenSize / 2;
    }

    void Character::DrawBars() {
        const float barWidth = 240;
        const float barHeight = 18;
        const float baseY = GameEngine::GetInstance().GetScreenSize().y * 0.85f + 20;

        const float spacing = 10; // 左右間距
        const float screenCenterX = GameEngine::GetInstance().GetScreenSize().x / 2;

        float hpBarX = screenCenterX - spacing - barWidth;
        float hpBarY = baseY;

        float powerBarX = screenCenterX + spacing;
        float powerBarY = baseY;

        ALLEGRO_COLOR bgColor = al_map_rgb(50, 50, 50);
        ALLEGRO_COLOR hpColor = al_map_rgb(255, 120, 120);
        ALLEGRO_COLOR powerColor = al_map_rgb(120, 120, 255);

        float healthPercent = static_cast<float>(HP) / MAX_HP;
        float powerPercent = static_cast<float>(POWER) / MAX_POWER;

        // 血條
        al_draw_filled_rectangle(hpBarX, hpBarY, hpBarX + barWidth, hpBarY + barHeight, bgColor);
        al_draw_filled_rectangle(hpBarX, hpBarY, hpBarX + barWidth * healthPercent, hpBarY + barHeight, hpColor);

        // 能量條
        al_draw_filled_rectangle(powerBarX, powerBarY, powerBarX + barWidth, powerBarY + barHeight, bgColor);
        al_draw_filled_rectangle(powerBarX, powerBarY, powerBarX + barWidth * powerPercent, powerBarY + barHeight, powerColor);

        // 數字
        std::string hpText = std::to_string((int)HP);
        Engine::Label hpLabel(hpText, "pirulen.ttf", 16, hpBarX + 4, hpBarY -3.2, 255, 255, 255, 255);
        hpLabel.Draw();

        std::string powerText = std::to_string((int)POWER);
        Engine::Label powerLabel(powerText, "pirulen.ttf", 16, powerBarX + 4, powerBarY - 3.2, 255, 255, 255, 255);
        powerLabel.Draw();

        // 技能欄
        itemBar_.Draw(getPlayScene()->CameraPos, GameEngine::GetInstance().GetScreenSize());
        weaponManager.DrawWeaponBar(getPlayScene()->CameraPos, GameEngine::GetInstance().GetScreenSize()); // 添加這一行
    }

    void Character::Draw() const {
        Sprite::Draw();
        weaponManager.Draw();
    }

    void Character::OnMouseMove(int mx, int my) {
        weaponManager.OnMouseMove(mx, my);
    }

    void Character::OnMouseDown(int button, int mx, int my) {
        weaponManager.OnMouseDown(button, mx, my);
    }

    void Character::OnMouseUp(int button, int mx, int my) {
        weaponManager.OnMouseUp(button, mx, my);
    }

    bool Character::IsAlive() {
        return !isDead;
    }

    void Character::setTimer(float time) {
        speedTimer = time;
        originalSpeed = speed / 2;
    }

    void Character::AddSkill(SkillBase* skill) {
        for (int i = 0; i < itemBar_.SlotAmount; i++) {
            if (!itemBar_.slots[i]) {
                itemBar_.slots[i] = skill;
                return;
            }
        }
        std::cout << "No empty slot to add skill.\n";
    }

    void Character::UseSkill(int index) {
        if (index < 0 || index > itemBar_.slots.size() - 1) return;
        itemBar_.slots[index]->Activate();
    }

    bool Character::ChangeHP(float dHP){
        HP += dHP;
        if (HP <= 0){
            HP = 0;
            return false;
        }
        else if (HP > MAX_HP){
            HP = MAX_HP;
        }
        return true;
    }

    bool Character::ChangePOWER(float dPOWER){
        POWER += dPOWER;
        
        if (POWER <= 0){
            POWER -= dPOWER;
            return false;
        }
        return true;
    }

}