#include "Character.hpp"
#include <allegro5/allegro.h>
#include "Engine/GameEngine.hpp"
#include "Engine/Collider.hpp"
#include "Scene/PlayScene.hpp"
#include <iostream>

#include <allegro5/allegro_primitives.h>

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
        HP = 1000;
        POWER = 500;
        // std::cout << ">>>>>>>>Bitmap width: " << GetBitmapWidth() << ", height: " << GetBitmapHeight() << std::endl;
    }

    void Character::OnKeyDown(int keyCode) {
        if (keyCode == ALLEGRO_KEY_K){ // 自殺測試用
            isDying = 1;
            HP = 0;
            cur_frame = frame_timer = 0;
        }
        else if (keyCode == ALLEGRO_KEY_BACKSPACE){
            HP -= 100;
            std::cout <<"HURT: " << HP<<std::endl;
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
        
        if(HP == 0 && !isDying) { // 快死了
            cur_frame = frame_timer = 0;
            isDying = 1;
        }
        else if (HP <= 0 && isDying){
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
        if (isDead == 1) { // 真死了
            std::cout << "YOU FUCKED UP" << std::endl;

        }


        // Calculate new position based on velocity and delta time
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

        tileLeft = std::max(0, tileLeft);
        tileRight = std::min(mapWidth - 1, tileRight);
        tileTop = std::max(0, tileTop);
        tileBottom = std::min(mapHeight - 1, tileBottom);

        for (int ty = tileTop; ty <= tileBottom; ++ty) {
            for (int tx = tileLeft; tx <= tileRight; ++tx) {
                int tile = mapState[ty][tx];

                if (tile == getPlayScene()->TILE_WATER || tile == getPlayScene()->TILE_ROCK) {
                    Point tileMin(tx * BlockSize, ty * BlockSize);
                    Point tileMax = tileMin + Point(BlockSize, BlockSize);

                    // 找角色圓心在 tile 上的最近點
                    float closestX = std::max(tileMin.x, std::min(newPosition.x, tileMax.x));
                    float closestY = std::max(tileMin.y, std::min(newPosition.y, tileMax.y));
                    Point closestPoint(closestX, closestY);

                    // 把腳色碰撞鄉一道腳附近
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
        } else {
            Velocity = Point(0, 0);
        }

        // 保持角色在螢幕內
        Point screenSize = GameEngine::GetInstance().GetScreenSize();
        /*Position.x = std::max(CollisionRadius, std::min(screenSize.x - CollisionRadius, Position.x));
        Position.y = std::max(Colliu y 一sionRadius, std::min(screenSize.y - CollisionRadius, Position.y));*/

        getPlayScene()->CameraPos = Position - screenSize / 2;
    }

    void Character::DrawBars() {
        // 血
        const float barWidth = 60;
        const float barHeight = 7;
        const float offsetY = -48; // 在腳色頭上

        float healthPercent = (float)HP / MAX_HP;

        ALLEGRO_COLOR bgColor = al_map_rgb(100, 100, 100);
        ALLEGRO_COLOR frontColor = al_map_rgb(255, 0, 0);

        float barX = Position.x - barWidth / 2 - getPlayScene()->CameraPos.x;
        float barY = Position.y + offsetY - getPlayScene()->CameraPos.y;

        al_draw_filled_rectangle(barX, barY, barX + barWidth, barY + barHeight, bgColor);
        al_draw_filled_rectangle(barX, barY, barX + barWidth * healthPercent, barY + barHeight, frontColor);
        
        // 魔力
        const float offsetY2 = -40; // 在腳色頭上

        float powerPercent = (float)POWER / MAX_POWER;

        frontColor = al_map_rgb(0, 0, 255);

        barX = Position.x - barWidth / 2 - getPlayScene()->CameraPos.x;
        barY = Position.y + offsetY2 - getPlayScene()->CameraPos.y;

        al_draw_filled_rectangle(barX, barY, barX + barWidth, barY + barHeight, bgColor);
        al_draw_filled_rectangle(barX, barY, barX + barWidth * powerPercent, barY + barHeight, frontColor);

    }

    void Character::Draw() const {
        Sprite::Draw();
    }

    bool Character::IsAlive(){
        return !isDead;
    }
}