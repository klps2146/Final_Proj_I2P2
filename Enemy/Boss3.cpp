#include "Boss3.hpp"
#include "Scene/PlayScene.hpp"
#include "Engine/GameEngine.hpp"
#include <allegro5/allegro_primitives.h>

// 構造函數，初始化Boss3屬性
Boss3::Boss3(float x, float y)
    : BossEnemy("play/boss3.png", x, y-150, 30.0f, 180.0f, 1000.0f, 50),
      shootTimer(0), rayActive(false), rayTimer(0), healAmount(100.0f) {
    dmg = 15;  // 設置Boss3的傷害值
}

// 更新射線邏輯
void Boss3::UpdateRay(float deltaTime) {
    PlayScene* scene = getPlayScene();
    if (!scene || !scene->character) return;  // 確保場景和玩家存在

    Engine::Point playerPos = scene->character->Position;
    float distance = (playerPos - Position).Magnitude();

    if (rayActive) {
        rayTimer += deltaTime;
        if (distance > 1500.0f) {
            rayActive = false;
            rayTimer = 0;
        }
        else if (rayTimer >= 5.0f) {
            hp += healAmount;
            if (hp > max_hp) hp = max_hp;  // 個人血量不超過最大值
            shared_hp += healAmount;
            if (shared_hp > 3000.0f) shared_hp = 3000.0f;  // 共享血量不超過3000
            rayActive = false;
            rayTimer = 0;
            scene->Hit(1000);
        }
    } else {
        shootTimer += deltaTime;
        // 每10秒發射一次射線
        if (shootTimer >= 5.0f) {
            rayActive = true;
            shootTimer = 0;
            rayTimer = 0;
        }
    }
}

void Boss3::DrawRay() const {
    if (rayActive) {
        PlayScene* scene = getPlayScene();
        if (scene && scene->character) {
            Engine::Point playerPos = scene->character->Position;
            Engine::Point cameraPos = scene->CameraPos;
            
            // 計算 Boss3 的基礎位置相對於屏幕
            float baseX = Position.x - cameraPos.x;
            float baseY = Position.y - cameraPos.y;
            
            // 將射線起始點偏移到圖片右邊150像素和上方10像素
            // 右邊偏移：沿著Rotation方向150像素
            float rightOffsetX = cos(Rotation) * 150.0f;
            float rightOffsetY = sin(Rotation) * 150.0f;
            // 上方偏移：沿著Rotation旋轉90度（逆時針）的方向10像素
            float upOffsetX = -sin(Rotation) * -15.0f; // cos(Rotation + PI/2) = -sin(Rotation)
            float upOffsetY = cos(Rotation) * -15.0f;  // sin(Rotation + PI/2) = cos(Rotation)
            // 結合偏移
            float screenX1 = baseX + rightOffsetX + upOffsetX;
            float screenY1 = baseY + rightOffsetY + upOffsetY;
            
            // 玩家的屏幕位置
            float screenX2 = playerPos.x - cameraPos.x;
            float screenY2 = playerPos.y - cameraPos.y;
            
            // 計算偏移後的Boss3和玩家之間的距離
            Engine::Point startPos = Engine::Point(Position.x + rightOffsetX + upOffsetX, 
                                                  Position.y + rightOffsetY + upOffsetY);
            float distance = (playerPos - startPos).Magnitude();
            
            // 假設最大距離為 1000.0f，根據距離比例計算顏色
            float maxDistance = 1000.0f;
            float ratio = distance / maxDistance;
            if (ratio > 1.0f) ratio = 1.0f;
            
            // 定義起點和終點顏色
            int startRed = 255, startGreen = 0, startBlue = 0;
            int endRed = 0, endGreen = 0, endBlue = 255;
            int red = static_cast<int>(startRed * (1 - ratio) + endRed * ratio);
            int green = static_cast<int>(startGreen * (1 - ratio) + endGreen * ratio);
            int blue = static_cast<int>(startBlue * (1 - ratio) + endBlue * ratio);
            
            // 計算線條方向向量和法向量
            float dx = screenX2 - screenX1;
            float dy = screenY2 - screenY1;
            float length = sqrt(dx * dx + dy * dy);
            if (length == 0) return; // 避免除以零
            float nx = -dy / length; // 法向量 x
            float ny = dx / length;  // 法向量 y
            
            // 設置線條半寬
            float halfWidth = 4.0f; // 調整這個值來改變線條粗細
            
            // 定義四邊形的四個頂點
            ALLEGRO_VERTEX vertices[4];
            vertices[0].x = screenX1 + nx * halfWidth;
            vertices[0].y = screenY1 + ny * halfWidth;
            vertices[0].z = 0;
            vertices[0].color = al_map_rgb(startRed, startGreen, startBlue);
            
            vertices[1].x = screenX1 - nx * halfWidth;
            vertices[1].y = screenY1 - ny * halfWidth;
            vertices[1].z = 0;
            vertices[1].color = al_map_rgb(startRed, startGreen, startBlue);
            
            vertices[2].x = screenX2 + nx * halfWidth;
            vertices[2].y = screenY2 + ny * halfWidth;
            vertices[2].z = 0;
            vertices[2].color = al_map_rgb(red, green, blue);
            
            vertices[3].x = screenX2 - nx * halfWidth;
            vertices[3].y = screenY2 - ny * halfWidth;
            vertices[3].z = 0;
            vertices[3].color = al_map_rgb(red, green, blue);
            
            // 繪製四邊形（使用三角形條帶）
            al_draw_prim(vertices, NULL, NULL, 0, 4, ALLEGRO_PRIM_TRIANGLE_STRIP);
        }
    }
}

// 更新函數，調用基類更新並處理射線
void Boss3::Update(float deltaTime) {
    BossEnemy::Update(deltaTime);
    UpdateRay(deltaTime);
}

// 繪製函數，調用基類繪製並繪製射線
void Boss3::Draw() const {
    BossEnemy::Draw();
    DrawRay();
}