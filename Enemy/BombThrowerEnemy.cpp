#include "BombThrowerEnemy.hpp"
#include "Bomb.hpp"
#include "Scene/PlayScene.hpp"
#include "Engine/GameEngine.hpp"
#include "Engine/Point.hpp"

BombThrowerEnemy::BombThrowerEnemy(float x, float y) 
    : Enemy("play/enemy-5.png", x, y, 20.0f, 50.0f, 100.0f, 10), throwInterval(20.0f), throwTimer(0.0f) {
    // 初始化敵人屬性，圖片路徑、位置、半徑、速度、血量、傷害
}

void BombThrowerEnemy::Update(float deltaTime) {
    Enemy::Update(deltaTime); // 調用基類的更新函數
    throwTimer += deltaTime;  // 更新投擲計時器
    if (throwTimer >= throwInterval) {
        ThrowBomb();          // 當計時器達到間隔時投擲炸彈
        throwTimer = 0.0f;    // 重置計時器
    }
}

void BombThrowerEnemy::ThrowBomb() {
    PlayScene* scene = dynamic_cast<PlayScene*>(Engine::GameEngine::GetInstance().GetActiveScene());
    if (!scene) return; // 確保場景有效

    // 獲取玩家位置作為投擲目標
    Engine::Point targetPos = scene->character->Position;
    // 添加隨機性，使炸彈不會總是精確落在玩家身上
    targetPos.x += (rand() % 100 - 50);
    targetPos.y += (rand() % 100 - 50);

    // 創建炸彈物件並加入場景
    Bomb* bomb = new Bomb("play/bomb.png", targetPos.x, targetPos.y, 100.0f, 50.0f, 2.0f);
    // scene->EnemyGroup->AddNewObject(bomb);
    scene->AddNewObject(bomb);
}