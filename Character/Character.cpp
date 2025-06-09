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
        std::cout << "Created Character at (" << x << ", " << y << ") with speed " << speed << std::endl;
        SetSpriteSource(0, 0, 96, 96);
        isDying = 0;
        isDead = 0;
        // std::cout << ">>>>>>>>Bitmap width: " << GetBitmapWidth() << ", height: " << GetBitmapHeight() << std::endl;
    }

    void Character::OnKeyDown(int keyCode) {
        if (keyCode == ALLEGRO_KEY_K){ // 自殺測試用
            isDying = 1;
            cur_frame = frame_timer = 0;
        }
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
        if (isMoving && direction.Magnitude() > 0) {
            direction = direction.Normalize();
        }
        // std::cout << "Key down: " << keyCode << ", Direction: (" << direction.x << ", " << direction.y << ")" << std::endl;
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
        // std::cout << "Key up: " << keyCode << ", Direction: (" << direction.x << ", " << direction.y << ")" << std::endl;
    }

    void Character::Update(float deltaTime) {
        if (!isDying){
            if (isMoving) {
                Velocity = direction * speed;

                frame_timer += deltaTime;

                if (frame_timer >= frame_duration){
                    frame_timer -= frame_duration;
                    cur_frame = (cur_frame + 1) % max_frames;

                    if (direction.x > 0){ // 右
                        SetSpriteSource(cur_frame * 96, 0, 96, 96);
                        last_idle_dir = direction;
                    }
                    else if (direction.x < 0){ // 左
                        SetSpriteSource((7 - cur_frame) * 96, 96, 96, 96);  
                        last_idle_dir = direction;
                    }
                    else if (direction.x == 0){ // 看最後方向 (default 右)
                        if (last_idle_dir.x >= 0) SetSpriteSource(cur_frame * 96, 0, 96, 96); // right
                        else SetSpriteSource((7 - cur_frame) * 96, 96, 96, 96);
                    }
                }
            }
            else {
                /// IDLE 動畫
                frame_timer += deltaTime;

                if (frame_timer >= frame_duration){
                    frame_timer -= frame_duration;
                    cur_frame = (cur_frame + 1) % (max_frames+1);

                    if (last_idle_dir.x >= 0)
                        SetSpriteSource(cur_frame * 96, 96*3, 96, 96);
                    else
                        SetSpriteSource(cur_frame * 96, 96*5, 96, 96);
                }
            }
        }
        else if(!isDead) { // 快死了
            frame_timer += deltaTime;

            if (frame_timer >= (frame_duration+0.13)){
                frame_timer -= (frame_duration+0.13);
                cur_frame = (cur_frame + 1);

                if (cur_frame > max_frames-1){
                    std::cout << "お前はもう死んでる" << std::endl;
                    isDead = 1;
                }

                if (last_idle_dir.x >= 0)
                    SetSpriteSource(cur_frame * 96, 96*2, 96, 96);
                else
                    SetSpriteSource(cur_frame * 96, 96*4, 96, 96);
            }
        }
        else{ // 真死了
            std::cout << "YOU FUCKED UP" << std::endl;

        }

        Sprite::Update(deltaTime);

        Point screenSize = GameEngine::GetInstance().GetScreenSize();
        Position.x = std::max(CollisionRadius, std::min(screenSize.x - CollisionRadius, Position.x));
        Position.y = std::max(CollisionRadius, std::min(screenSize.y - CollisionRadius, Position.y));
    }
    
}