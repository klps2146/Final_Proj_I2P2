#include "Boss1.hpp"
#include "Scene/PlayScene.hpp"
#include "Engine/GameEngine.hpp"
#include "Engine/AudioHelper.hpp"
Boss1::Boss1(float x, float y)
    : BossEnemy("play/boss1.png", x, y, 30.0f, 200.0f, 1000.0f, 50), shootTimer(0), shootInterval(1.5f) {
    dmg = 10;
}

void Boss1::Shoot() {
    PlayScene* scene = getPlayScene();
    if (scene && scene->character) {
        Engine::Point playerPos = scene->character->Position;
        Engine::Point direction = playerPos - Position;
        direction = direction.Normalize();
        float speed = 400.0f;
        Engine::Point velocity = direction * speed;
        float damage = 15.0f;
        
        //Engine::EnemyBullet1* bullet = new Engine::EnemyBullet1(Position.x, Position.y, velocity, damage);
        //Engine::EnemyBullet2* bullet = new Engine::EnemyBullet2(Position.x, Position.y, velocity, damage);  // 修改：使用StarBullet
        //scene->BulletGroup->AddNewObject(bullet);

        int numBullets = 9;              // 子彈數量
        float totalAngle = 180.0f;       // 半圓總角度 (180 度)
        float angleIncrement = totalAngle / (numBullets - 1); // 每顆子彈的角度間隔
        
        // 起始角度：以朝向玩家方向為中心，向左偏移 90 度開始
        float startAngle = -90.0f;       // 從 -90 度開始到 +90 度結束
        
        // 將角度轉換為弧度，因為三角函數需要弧度
        float startAngleRad = startAngle * (M_PI / 180.0f);
        float angleIncrementRad = angleIncrement * (M_PI / 180.0f);
        
        // 計算旋轉角度，使半圓對齊朝向玩家的方向
        float cosTheta = direction.x;    // 朝向玩家的方向 x 分量
        float sinTheta = direction.y;    // 朝向玩家的方向 y 分量
        
        // 遍歷生成每顆子彈
        for (int i = 0; i < numBullets; ++i) {
            // 計算當前子彈的角度
            float angle = startAngleRad + i * angleIncrementRad;
            
            // 計算子彈在局部座標系中的方向
            float bulletDirX = cos(angle);
            float bulletDirY = sin(angle);
            
            // 將子彈方向旋轉到朝向玩家的方向
            float rotatedX = bulletDirX * cosTheta - bulletDirY * sinTheta;
            float rotatedY = bulletDirX * sinTheta + bulletDirY * cosTheta;
            
            // 正規化方向並設定速度
            Engine::Point bulletDirection(rotatedX, rotatedY);
            bulletDirection = bulletDirection.Normalize();
            float speed = 350.0f;
            Engine::Point velocity = bulletDirection * speed;
            float damage = 300.0f;
            Engine::EnemyBullet2* bullet = new Engine::EnemyBullet2(Position.x, Position.y, velocity, damage);
            
            scene->BulletGroup->AddNewObject(bullet);
        }
        AudioHelper::PlaySample("star.wav", false, 5.0);

        
    }
}

void Boss1::Update(float deltaTime) {
    BossEnemy::Update(deltaTime);
    shootTimer += deltaTime;
    if (shootTimer >= shootInterval) {
        Shoot();
        shootTimer = 0;
    }
    if (hp <= 0) {
        SetBossStatus(1, false);
    }
}