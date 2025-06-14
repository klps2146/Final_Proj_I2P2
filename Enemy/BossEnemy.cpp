// // #include "BossEnemy.hpp"
// // #include "Scene/PlayScene.hpp"
// // #include <allegro5/allegro_primitives.h>
// // #include <random>
// // #include "Boss3.hpp" // Add this include
// // #include "Boss2.hpp" // Add this include
// // #include "Boss1.hpp" // Add this include
// // #include "Turret/Turret.hpp"
// // float BossEnemy::shared_hp = 3000.0f; // Total shared health for all bosses
// // bool BossEnemy::boss1_alive = true;
// // bool BossEnemy::boss2_alive = true;

// // BossEnemy::BossEnemy(std::string img, float x, float y, float radius, float speed, float hp, int money)
// //     : Enemy(img, x, y, radius, speed, hp, money), path_timer(0), path_interval(3.0f) {
// //     std::random_device dev;
// //     rng = std::mt19937(dev());
// //     random_target = Position;
// //     max_hp = hp;
// // }

// // void BossEnemy::ResetSharedHealth() {
// //     shared_hp = 3000.0f;
// //     boss1_alive = true;
// //     boss2_alive = true;
// // }

// // float BossEnemy::GetSharedHealth() {
// //     return shared_hp;
// // }

// // void BossEnemy::SetBossStatus(int boss_id, bool status) {
// //     if (boss_id == 1) boss1_alive = status;
// //     if (boss_id == 2) boss2_alive = status;
// // }

// // void BossEnemy::UpdatePath() {
// //     PlayScene* scene = getPlayScene();
// //     std::uniform_int_distribution<int> dist_x(0, scene->MapWidth - 1);
// //     std::uniform_int_distribution<int> dist_y(0, scene->MapHeight - 1);
// //     int nx = dist_x(rng);
// //     int ny = dist_y(rng);
// //     random_target = Engine::Point(nx * PlayScene::BlockSize + PlayScene::BlockSize / 2,
// //                                   ny * PlayScene::BlockSize + PlayScene::BlockSize / 2);
// // }

// // void BossEnemy::Update(float deltaTime) {
// //     path_timer += deltaTime;
// //     if (path_timer >= path_interval) {
// //         UpdatePath();
// //         path_timer = 0;
// //     }

// //     Engine::Point vec = random_target - Position;
// //     if (vec.Magnitude() > PlayScene::BlockSize / 2) {
// //         Velocity = vec.Normalize() * speed;
// //         currentDirection = Velocity.Normalize();
// //         Position = Position + Velocity * deltaTime;
// //     } else {
// //         Velocity = Engine::Point(0, 0);
// //     }

// //     Rotation = atan2(Velocity.y, Velocity.x);
// //     Sprite::Update(deltaTime);

// //     if (speed_changed) {
// //         speed_timer += deltaTime;
// //         if (speed_timer >= speed_duration) {
// //             speed = original_speed;
// //             speed_changed = false;
// //             speed_duration = 0;
// //         }
// //     }
// // }

// // void BossEnemy::HandlePlayerCollision() {
// //     // Bosses don't die on player collision
// //     PlayScene* scene = getPlayScene();
// //     scene->Hit(dmg);
// // }

// // void BossEnemy::Hit(float damage, bool byplayer) {
// //     // Only allow damage to Boss3 if Boss1 and Boss2 are dead
// //     if (dynamic_cast<Boss3*>(this) && (boss1_alive || boss2_alive)) {
// //         return;
// //     }
// //     hp -= damage;
// //     shared_hp -= damage;
// //     Playerhit = byplayer;
// //     if (hp <= 0 || shared_hp <= 0) {
// //         OnExplode();

// //         if (dynamic_cast<Boss1*>(this)) {
// //             SetBossStatus(1, false);
// //             std::cout << "Boss1 removed, boss1_alive set to false" << std::endl;
// //         } else if (dynamic_cast<Boss2*>(this)) {
// //             SetBossStatus(2, false);
// //             std::cout << "Boss2 removed, boss2_alive set to false" << std::endl;
// //         }
// //         //for (auto& it : lockedTurrets) it->Target = nullptr;
// //         //for (auto& it : lockedBullets) it->Target = nullptr;
// //         if (Playerhit) {
// //             getPlayScene()->player_exp += money * 1.945;
// //             getPlayScene()->SpawnCoin(Position.x, Position.y, money);
// //         }

// //         getPlayScene()->EnemyGroup->RemoveObject(objectIterator);
// //         //AudioHelper::PlayAudio("explosion.wav");
// //     }
// // }

// // void BossEnemy::DrawSharedHealthBar() const {
// //     const float barWidth = 300;
// //     const float barHeight = 20;
// //     const float offsetY = 50; // Top of screen
// //     float healthPercent = shared_hp / 3000.0f;
// //     if (healthPercent < 0) healthPercent = 0;

// //     ALLEGRO_COLOR bgColor = al_map_rgb(100, 100, 100);
// //     ALLEGRO_COLOR frontColor = al_map_rgb(255, 0, 0);

// //     float barX = getPlayScene()->GetClientSize().x / 2 - barWidth / 2;
// //     float barY = offsetY;

// //     al_draw_filled_rectangle(barX, barY, barX + barWidth, barY + barHeight, bgColor);
// //     al_draw_filled_rectangle(barX, barY, barX + barWidth * healthPercent, barY + barHeight, frontColor);
// // }

// // void BossEnemy::Draw() const {
// //     Sprite::Draw();
// //     DrawBars();
// //     DrawSharedHealthBar();
// //     if (PlayScene::DebugMode) {
// //         al_draw_circle(Position.x, Position.y, CollisionRadius, al_map_rgb(255, 0, 0), 2);
// //     }
// // }

// #include "BossEnemy.hpp"
// #include "Scene/PlayScene.hpp"
// #include <allegro5/allegro_primitives.h>
// #include <random>
// #include "Boss3.hpp"
// #include "Boss2.hpp"
// #include "Boss1.hpp"
// #include "Turret/Turret.hpp"

// float BossEnemy::shared_hp = 3000.0f;
// bool BossEnemy::boss1_alive = true;
// bool BossEnemy::boss2_alive = true;

// BossEnemy::BossEnemy(std::string img, float x, float y, float radius, float speed, float hp, int money)
//     : Enemy(img, x, y, radius, speed, hp, money), path_timer(0), path_interval(1.0) {  // 修改path_interval為1.0f
//     std::random_device dev;
//     rng = std::mt19937(dev());
//     random_target = Position;
//     max_hp = hp;
// }

// void BossEnemy::ResetSharedHealth() {
//     shared_hp = 3000.0f;
//     boss1_alive = true;
//     boss2_alive = true;
// }

// float BossEnemy::GetSharedHealth() {
//     return shared_hp;
// }

// void BossEnemy::SetBossStatus(int boss_id, bool status) {
//     if (boss_id == 1) boss1_alive = status;
//     if (boss_id == 2) boss2_alive = status;
// }

// void BossEnemy::UpdatePath() {
//     PlayScene* scene = getPlayScene();
//     std::uniform_int_distribution<int> dist_x(0, scene->MapWidth - 1);
//     std::uniform_int_distribution<int> dist_y(0, scene->MapHeight - 1);
//     int nx = dist_x(rng);
//     int ny = dist_y(rng);
//     random_target = Engine::Point(nx * PlayScene::BlockSize + PlayScene::BlockSize / 2,
//                                   ny * PlayScene::BlockSize + PlayScene::BlockSize / 2);
// }

// void BossEnemy::Update(float deltaTime) {
//     // 時時刻刻面向玩家
//     Engine::Point playerPos = getPlayScene()->character->Position;  // 假設此方法存在
//     Engine::Point direction = playerPos - Position;
//     Rotation = atan2(direction.y, direction.x);

//     // 更新移動目標
//     path_timer += deltaTime;
//     if (path_timer >= path_interval) {
//         UpdatePath();
//         path_timer = 0;
//     }

//     Engine::Point vec = random_target - Position;
//     if (vec.Magnitude() > PlayScene::BlockSize / 2) {
//         Velocity = vec.Normalize() * speed;
//         currentDirection = Velocity.Normalize();
//         Position = Position + Velocity * deltaTime;
//     } else {
//         Velocity = Engine::Point(0, 0);
//     }

//     // Rotation已在前文設置，不再依賴Velocity
//     Sprite::Update(deltaTime);

//     if (speed_changed) {
//         speed_timer += deltaTime;
//         if (speed_timer >= speed_duration) {
//             speed = original_speed;
//             speed_changed = false;
//             speed_duration = 0;
//         }
//     }
// }

// void BossEnemy::HandlePlayerCollision() {
//     PlayScene* scene = getPlayScene();
//     scene->Hit(dmg);
// }

// void BossEnemy::Hit(float damage, bool byplayer) {
//     if (dynamic_cast<Boss3*>(this) && (boss1_alive || boss2_alive)) {
//         return;
//     }
//     hp -= damage;
//     shared_hp -= damage;
//     Playerhit = byplayer;
//     if (hp <= 0 || shared_hp <= 0) {
//         OnExplode();

//         if (dynamic_cast<Boss1*>(this)) {
//             SetBossStatus(1, false);
//             std::cout << "Boss1 removed, boss1_alive set to false" << std::endl;
//         } else if (dynamic_cast<Boss2*>(this)) {
//             SetBossStatus(2, false);
//             std::cout << "Boss2 removed, boss2_alive set to false" << std::endl;
//         }
//         if (Playerhit) {
//             getPlayScene()->player_exp += money * 1.945;
//             getPlayScene()->SpawnCoin(Position.x, Position.y, money);
//         }

//         getPlayScene()->EnemyGroup->RemoveObject(objectIterator);
//     }
// }

// void BossEnemy::DrawSharedHealthBar() const {
//     const float barWidth = 300;
//     const float barHeight = 20;
//     const float offsetY = 50;
//     float healthPercent = shared_hp / 3000.0f;
//     if (healthPercent < 0) healthPercent = 0;

//     ALLEGRO_COLOR bgColor = al_map_rgb(100, 100, 100);
//     ALLEGRO_COLOR frontColor = al_map_rgb(255, 0, 0);

//     float barX = getPlayScene()->GetClientSize().x / 2 - barWidth / 2;
//     float barY = offsetY;

//     al_draw_filled_rectangle(barX, barY, barX + barWidth, barY + barHeight, bgColor);
//     al_draw_filled_rectangle(barX, barY, barX + barWidth * healthPercent, barY + barHeight, frontColor);
// }

// void BossEnemy::Draw() const {
//     Sprite::Draw();
//     DrawBars();
//     DrawSharedHealthBar();
//     if (PlayScene::DebugMode) {
//         al_draw_circle(Position.x, Position.y, CollisionRadius, al_map_rgb(255, 0, 0), 2);
//     }
// }

#include "BossEnemy.hpp"
#include "Scene/PlayScene.hpp"
#include <allegro5/allegro_primitives.h>
#include <random>
#include "Boss3.hpp"
#include "Boss2.hpp"
#include "Boss1.hpp"
#include "Turret/Turret.hpp"

float BossEnemy::shared_hp = 3000.0f;
bool BossEnemy::boss1_alive = true;
bool BossEnemy::boss2_alive = true;
bool BossEnemy::boss3_alive = true; // Define and initialize boss3_alive

BossEnemy::BossEnemy(std::string img, float x, float y, float radius, float speed, float hp, int money)
    : Enemy(img, x, y, radius, speed, hp, money), path_timer(0), path_interval(1.0) {
    std::random_device dev;
    rng = std::mt19937(dev());
    random_target = Position;
    max_hp = hp;
}

void BossEnemy::ResetSharedHealth() {
    shared_hp = 3000.0f;
    boss1_alive = true;
    boss2_alive = true;
    boss3_alive = true; // Reset boss3_alive to true
}

float BossEnemy::GetSharedHealth() {
    return shared_hp;
}

void BossEnemy::SetBossStatus(int boss_id, bool status) {
    if (boss_id == 1) boss1_alive = status;
    if (boss_id == 2) boss2_alive = status;
    if (boss_id == 3) boss3_alive = status; // Handle Boss3 status
}

void BossEnemy::UpdatePath() {
    PlayScene* scene = getPlayScene();
    std::uniform_int_distribution<int> dist_x(0, scene->MapWidth - 1);
    std::uniform_int_distribution<int> dist_y(0, scene->MapHeight - 1);
    int nx = dist_x(rng);
    int ny = dist_y(rng);
    random_target = Engine::Point(nx * PlayScene::BlockSize + PlayScene::BlockSize / 2,
                                  ny * PlayScene::BlockSize + PlayScene::BlockSize / 2);
}

void BossEnemy::Update(float deltaTime) {
    Engine::Point playerPos = getPlayScene()->character->Position;
    Engine::Point direction = playerPos - Position;
    Rotation = atan2(direction.y, direction.x);

    path_timer += deltaTime;
    if (path_timer >= path_interval) {
        UpdatePath();
        path_timer = 0;
    }

    Engine::Point vec = random_target - Position;
    if (vec.Magnitude() > PlayScene::BlockSize / 2) {
        Velocity = vec.Normalize() * speed;
        currentDirection = Velocity.Normalize();
        Position = Position + Velocity * deltaTime;
    } else {
        Velocity = Engine::Point(0, 0);
    }

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

void BossEnemy::HandlePlayerCollision() {
    PlayScene* scene = getPlayScene();
    scene->Hit(dmg);
}

void BossEnemy::Hit(float damage, bool byplayer) {
    // Prevent damage to Boss1 and Boss2 if Boss3 is alive
    if ((dynamic_cast<Boss1*>(this) || dynamic_cast<Boss2*>(this)) && boss3_alive) {
        return;
    }
    hp -= damage;
    shared_hp -= damage;
    Playerhit = byplayer;
    if (hp <= 0 || shared_hp <= 0) {
        OnExplode();

        if (dynamic_cast<Boss1*>(this)) {
            SetBossStatus(1, false);
            std::cout << "Boss1 removed, boss1_alive set to false" << std::endl;
        } else if (dynamic_cast<Boss2*>(this)) {
            SetBossStatus(2, false);
            std::cout << "Boss2 removed, boss2_alive set to false" << std::endl;
        } else if (dynamic_cast<Boss3*>(this)) {
            SetBossStatus(3, false);
            std::cout << "Boss3 removed, boss3_alive set to false" << std::endl;
        }
        if (Playerhit) {
            getPlayScene()->player_exp += money * 1.945;
            getPlayScene()->SpawnCoin(Position.x, Position.y, money);
        }

        getPlayScene()->EnemyGroup->RemoveObject(objectIterator);
    }
}

void BossEnemy::DrawSharedHealthBar() const {
    const float barWidth = 300;
    const float barHeight = 20;
    float healthPercent = shared_hp / 3000.0f;
    if (healthPercent < 0) healthPercent = 0;

    ALLEGRO_COLOR bgColor = al_map_rgb(100, 100, 100);
    ALLEGRO_COLOR frontColor = al_map_rgb(255, 0, 0);

    float barX = getPlayScene()->GetClientSize().x / 2.0f - barWidth / 2.0f;
    float barY = getPlayScene()->GetClientSize().y / 2.0f - barHeight / 2.0f;

    al_draw_filled_rectangle(barX, barY, barX + barWidth, barY + barHeight, bgColor);
    al_draw_filled_rectangle(barX, barY, barX + barWidth * healthPercent, barY + barHeight, frontColor);
}

void BossEnemy::Draw() const {
    Sprite::Draw();
    DrawBars();
    DrawSharedHealthBar();
    if (PlayScene::DebugMode) {
        al_draw_circle(Position.x, Position.y, CollisionRadius, al_map_rgb(255, 0, 0), 2);
    }
}