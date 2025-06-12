#ifndef MINIMAP_HPP
#define MINIMAP_HPP

#include <vector>
#include "Engine/Point.hpp"
#include "Engine/Group.hpp"
#include "UI/Component/ColoredRectangle.hpp"

class PlayScene;

class MiniMap {
public:
    MiniMap(){};
    MiniMap(float worldWidth, float worldHeight,
            float x, float y, float width, float height,
            ALLEGRO_COLOR bgColor = al_map_rgba(30, 30, 30, 100),
            ALLEGRO_COLOR borderColor = al_map_rgba(255, 255, 255, 100),
            float borderThickness = 1.0f);

    void SetPlayer(const Engine::Point& pos, float rotation); // 旋轉角
    void SetEnemyPositions(Engine::Group* Enemies);
    void Draw() const;

private:
    float minimapX = 1595;  // minimap 在螢幕上的位置 X
    float minimapY = 5;  // minimap 在螢幕上的位置 Y
    float minimapWidth = 300;  // minimap 的寬度
    float minimapHeight = 300; // minimap 的高度

    float worldWidth, worldHeight;
    Engine::Point playerPos;
    float playerRotation = 0.0f; // 弧度
    // std::vector<Engine::Point> enemyPositions;
    Engine::Group* EnemyGroup;

    ColoredRectangle background;
    void Update(float DeltaTime);
    Engine::Point WorldToMinimap(const Engine::Point& pos) const;
    PlayScene* getPlayScene() const;
};

#endif
