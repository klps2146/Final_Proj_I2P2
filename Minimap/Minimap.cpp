#include "Minimap.hpp"
#include "Scene/PlayScene.hpp"
#include "Engine/GameEngine.hpp"

#include <allegro5/allegro_primitives.h>
#include <cmath>
#include <iostream>

PlayScene* MiniMap::getPlayScene() const{
    return dynamic_cast<PlayScene*>(Engine::GameEngine::GetInstance().GetActiveScene());
}


MiniMap::MiniMap(float worldWidth, float worldHeight,
                 float x, float y, float width, float height,
                 ALLEGRO_COLOR bgColor, ALLEGRO_COLOR borderColor, float borderThickness)
    : worldWidth(worldWidth), worldHeight(worldHeight),
      background(x, y, width, height, bgColor, borderThickness, borderColor) {
    minimapX = x;
    minimapY = y;
    minimapWidth = width;
    minimapHeight = height;

}

void MiniMap::SetPlayer(const Engine::Point& pos, float rotation) {
    playerPos = pos;
    playerRotation = rotation;
}

void MiniMap::SetEnemyPositions(Engine::Group* Enemies) {
    EnemyGroup = Enemies;
}

void MiniMap::Update(float deltaTime) {
    // 預留
}

Engine::Point MiniMap::WorldToMinimap(const Engine::Point& pos) const {
    PlayScene* scene = dynamic_cast<PlayScene*>(Engine::GameEngine::GetInstance().GetActiveScene());
    float scaleX = minimapWidth / scene->MapWidth / 64;
    float scaleY = minimapHeight / scene->MapHeight / 64;

    float x = minimapX + pos.x * scaleX;
    float y = minimapY + pos.y * scaleY;
    // std::cout << "Map size: " << scene->MapWidth << " x " << scene->MapHeight << std::endl;
    // std::cout << "Scale: " << scaleX << " " << scaleY << std::endl;
    return Engine::Point(x, y);
}

void MiniMap::Draw() const{
    background.Draw();

    // player
    Engine::Point p = WorldToMinimap(playerPos);
    float r = 10.0f;

    // 三角形三個頂點（箭頭朝向 playerRotation）
    float angle = playerRotation;
    Engine::Point tip = Engine::Point(p.x + std::cos(angle) * r, p.y + std::sin(angle) * r );
    Engine::Point left = Engine::Point(p.x + std::cos(angle + 2.5f) * r * 0.6f, p.y + std::sin(angle + 2.5f) * r * 0.6f );
    Engine::Point right = Engine::Point(p.x + std::cos(angle - 2.5f) * r * 0.6f, p.y + std::sin(angle - 2.5f) * r * 0.6f );

    al_draw_filled_triangle(tip.x, tip.y, left.x, left.y, right.x, right.y, al_map_rgb(255, 255, 255));

    // 迷霧
    const float visibleRadius = 600.0f * getPlayScene()->character->VisableLevel;

    Engine::Point visionCenter = WorldToMinimap(playerPos);

    // 視野範圍
    PlayScene* scene = dynamic_cast<PlayScene*>(Engine::GameEngine::GetInstance().GetActiveScene());
    float scaleX = minimapWidth / scene->MapWidth / 64;
    float scaleY = minimapHeight / scene->MapHeight / 64;
    float miniVisionRadius = (scaleX + scaleY) / 2.0f * visibleRadius;

    al_draw_circle(visionCenter.x, visionCenter.y, miniVisionRadius + 4.5, al_map_rgba(200, 200, 255, 170), 1.5);

    // enemies
    for (const auto& e : EnemyGroup->GetObjects()) {
        float dx = e->Position.x - playerPos.x;
        float dy = e->Position.y - playerPos.y;
        float distSq = dx * dx + dy * dy;

        if (distSq <= visibleRadius * visibleRadius && e->Position.x >= -1 && e->Position.y >= -1) {
            Engine::Point ep = WorldToMinimap(e->Position);
            // std::cout << ep.x << " " << ep.y << std::endl;
            al_draw_filled_circle(ep.x, ep.y, 4, al_map_rgb(255, 50, 50));
        }
    }
}
