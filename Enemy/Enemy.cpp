#include <allegro5/allegro_primitives.h>
#include <allegro5/color.h>
#include <cmath>
#include <random>
#include <string>
#include <vector>

#include "Bullet/Bullet.hpp"
#include "Enemy.hpp"
#include "Engine/AudioHelper.hpp"
#include "Engine/GameEngine.hpp"
#include "Engine/Group.hpp"
#include "Engine/IScene.hpp"
#include "Engine/LOG.hpp"
#include "Scene/PlayScene.hpp"
#include "Turret/Turret.hpp"
#include "UI/Animation/DirtyEffect.hpp"
#include "UI/Animation/ExplosionEffect.hpp"

PlayScene *Enemy::getPlayScene() const{
    return dynamic_cast<PlayScene *>(Engine::GameEngine::GetInstance().GetActiveScene());
}
void Enemy::OnExplode() {
    getPlayScene()->EffectGroup->AddNewObject(new ExplosionEffect(Position.x, Position.y));
    std::random_device dev;
    std::mt19937 rng(dev());
    std::uniform_int_distribution<std::mt19937::result_type> distId(1, 3);
    std::uniform_int_distribution<std::mt19937::result_type> dist(1, 20);
    for (int i = 0; i < 10; i++) {
        getPlayScene()->GroundEffectGroup->AddNewObject(new DirtyEffect("play/dirty-" + std::to_string(distId(rng)) + ".png", dist(rng), Position.x, Position.y));
    }
}
Enemy::Enemy(std::string img, float x, float y, float radius, float speed, float hp, int money) : Engine::Sprite(img, x, y), speed(speed), hp(hp), money(money) 
, currentDirection(0, 0)//新增
{
    max_hp = hp;
    CollisionRadius = radius;
}

void Enemy::HandlePlayerCollision() {
    Hit(hp, false);
}

void Enemy::Hit(float damage,bool byplayer) {
    hp -= damage;
    Playerhit = byplayer; // 標記是被玩家打的
    if (hp <= 0) {
        OnExplode();
        for (auto &it : lockedTurrets)
            it->Target = nullptr;
        for (auto &it : lockedBullets)
            it->Target = nullptr;

        //getPlayScene()->EarnMoney(money);
        if(Playerhit){
            getPlayScene()->player_exp += money * 1.945;
            getPlayScene()->SpawnCoin(Position.x, Position.y, money);
        }
        getPlayScene()->EnemyGroup->RemoveObject(objectIterator);
        AudioHelper::PlayAudio("explosion.wav");
    }
}
void Enemy::Update(float deltaTime) {
    int x = static_cast<int>(floor(Position.x / PlayScene::BlockSize));
    int y = static_cast<int>(floor(Position.y / PlayScene::BlockSize));
    if (x < 0) x = 0;
    if (x >= PlayScene::MapWidth) x = PlayScene::MapWidth - 1;
    if (y < 0) y = 0;
    if (y >= PlayScene::MapHeight) y = PlayScene::MapHeight - 1;

    PlayScene* scene = getPlayScene();
    int minDist = scene->mapDistance[y][x];
    Engine::Point nextMove = Engine::Point(x, y);
    for (auto &dir : PlayScene::directions) {
        int nx = x + dir.x;
        int ny = y + dir.y;
        if (nx >= 0 && nx < PlayScene::MapWidth && ny >= 0 && ny < PlayScene::MapHeight &&
            scene->mapState[ny][nx] != PlayScene::TILE_OCCUPIED &&
            scene->mapDistance[ny][nx] != -1 && scene->mapDistance[ny][nx] < minDist) {
            minDist = scene->mapDistance[ny][nx];
            nextMove = Engine::Point(nx, ny);
        }
    }
    if (nextMove != Engine::Point(x, y)) {
        Engine::Point target = nextMove * PlayScene::BlockSize + Engine::Point(PlayScene::BlockSize / 2, PlayScene::BlockSize / 2);
        Engine::Point vec = target - Position;
        Velocity = vec.Normalize() * speed;
        currentDirection = Velocity.Normalize(); // 更新當前方向
        Position = Position + Velocity * deltaTime;
    } else {
        if (currentDirection != Engine::Point(0, 0)) {
            Velocity = currentDirection * speed; // 沿原本方向繼續移動
            Position = Position + Velocity * deltaTime;
        } else {
            Velocity = Engine::Point(0, 0); // 初始無方向時停止
        }
    }
    float distanceToPlayer = (Position - scene->character->Position).Magnitude();
    if (distanceToPlayer < CollisionRadius + scene->character->CollisionRadius) {
        
        scene->Hit(dmg);
        Playerhit = false;
        HandlePlayerCollision();
        return;
    }
    Rotation = atan2(Velocity.y, Velocity.x);
    Sprite::Update(deltaTime);
    if (speed_changed) {
        speed_timer += deltaTime;
        if (speed_timer >= speed_duration) {
            speed = original_speed;
            speed_changed = false;
            speed_duration = 0;
        }
    }
}


void Enemy::Draw() const {
    Sprite::Draw();
    DrawBars();

    if (PlayScene::DebugMode) {
        al_draw_circle(Position.x, Position.y, CollisionRadius, al_map_rgb(255, 0, 0), 2);
    }
}
void Enemy::change_speed(float dv_mul, float duration) {
    if (!speed_changed) {
        original_speed = speed;
    } else {
        speed = original_speed;
    }
    speed_changed = true;
    speed *= dv_mul;
    speed_duration = duration;
    speed_timer = 0;
}

void Enemy::DrawBars() const{
    // 血
    const float barWidth = 60;
    const float barHeight = 7;
    const float offsetY = -48; // 在腳色頭上

    float healthPercent = (float)hp / max_hp;

    ALLEGRO_COLOR bgColor = al_map_rgb(100, 100, 100);
    ALLEGRO_COLOR frontColor = al_map_rgb(255, 0, 0);

    float barX = Position.x - barWidth / 2 - getPlayScene()->CameraPos.x;
    float barY = Position.y + offsetY - getPlayScene()->CameraPos.y;

    al_draw_filled_rectangle(barX, barY, barX + barWidth, barY + barHeight, bgColor);
    al_draw_filled_rectangle(barX, barY, barX + barWidth * healthPercent, barY + barHeight, frontColor);
}


/*void Enemy::Update(float deltaTime) {
    int x = static_cast<int>(floor(Position.x / PlayScene::BlockSize));
    int y = static_cast<int>(floor(Position.y / PlayScene::BlockSize));
    if (x < 0) x = 0;
    if (x >= PlayScene::MapWidth) x = PlayScene::MapWidth - 1;
    if (y < 0) y = 0;
    if (y >= PlayScene::MapHeight) y = PlayScene::MapHeight - 1;

    PlayScene* scene = getPlayScene();
    int minDist = scene->mapDistance[y][x];
    Engine::Point nextMove = Engine::Point(x, y);
    for (auto &dir : PlayScene::directions) {
        int nx = x + dir.x;
        int ny = y + dir.y;
        if (nx >= 0 && nx < PlayScene::MapWidth && ny >= 0 && ny < PlayScene::MapHeight &&
            scene->mapState[ny][nx] != PlayScene::TILE_OCCUPIED &&
            scene->mapDistance[ny][nx] != -1 && scene->mapDistance[ny][nx] < minDist) {
            minDist = scene->mapDistance[ny][nx];
            nextMove = Engine::Point(nx, ny);
        }
    }
    if (nextMove != Engine::Point(x, y)) {
        Engine::Point target = nextMove * PlayScene::BlockSize + Engine::Point(PlayScene::BlockSize / 2, PlayScene::BlockSize / 2);
        Engine::Point vec = target - Position;
        Velocity = vec.Normalize() * speed;
        Position = Position + Velocity * deltaTime;
    } else {
        Velocity = Engine::Point(0, 0);
    }
    float distanceToPlayer = (Position - scene->character->Position).Magnitude();
    if (distanceToPlayer < CollisionRadius + scene->character->CollisionRadius) {
        scene->Hit();
        Hit(hp);
        return;
    }
    Rotation = atan2(Velocity.y, Velocity.x);
    Sprite::Update(deltaTime);
    if (speed_changed) {
        speed_timer += deltaTime;
        if (speed_timer >= speed_duration) {
            speed = original_speed;
            speed_changed = false;
            speed_duration = 0;
        }
    }
}*/


// #include <allegro5/allegro_primitives.h>
// #include <allegro5/color.h>
// #include <cmath>
// #include <random>
// #include <string>
// #include <vector>
