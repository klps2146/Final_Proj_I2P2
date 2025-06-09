#ifndef PLAYSCENE_HPP
#define PLAYSCENE_HPP
#include <list>
#include <queue>
#include <string>
#include <vector>
#include <allegro5/allegro_audio.h>
#include <Engine/IScene.hpp>

class Enemy;
class Turret;
namespace Engine {
    class Group;
    class Image;
    class Label;
    class Sprite;
}

class PlayScene : public Engine::IScene {
public:
    static bool DebugMode;
    static const std::vector<Engine::Point> directions;
    static const Engine::Point SpawnGridPoint;
    static const Engine::Point EndGridPoint;
    static const std::vector<int> code;
    static const float DangerTime;
    static const int MapWidth = 20, MapHeight = 13;
    static const int BlockSize = 64;
    enum TileType {
        TILE_DIRT,
        TILE_FLOOR,
        TILE_OCCUPIED
    };
    std::vector<std::vector<TileType>> mapState;
    std::vector<std::vector<int>> mapDistance;
    Engine::Group* TileMapGroup;
    Engine::Group* GroundEffectGroup;
    Engine::Group* DebugIndicatorGroup;
    Engine::Group* TowerGroup;
    Engine::Group* EnemyGroup;
    Engine::Group* BulletGroup;
    Engine::Group* EffectGroup;
    Engine::Group* UIGroup;
    Engine::Image* imgTarget;
    Engine::Label* UIMoney;
    Engine::Label* UILives;
    Engine::Label* player_exp_l;
    Engine::Label* player_level_l;
    Engine::Label* player_skill_point_l;
    Engine::Label* coin_lv_l;
    Engine::Label* coolDown_lv_l;
    Engine::Sprite* dangerIndicator;
    float ticks;
    int lives;
    int money;
    int MapId;
    float player_exp;
    std::list<std::pair<int, float>> enemyWaveData;
    Turret* preview;
    int SpeedMult;
    float deathCount;
    std::shared_ptr<ALLEGRO_SAMPLE_INSTANCE> deathBGMInstance;
    int bgmId;
    std::queue<int> level_req;
    float player_skill_point;
    float turret_coin_mul;
    float turret_coolDown;
    int coin_lv;
    int player_level;
    int coolDown_lv;
    // 新增功能相關
    float zoomLevel;
    Engine::Sprite* player;
    float playerSpeed;
    float cameraX, cameraY;

    explicit PlayScene() = default;
    void Initialize() override;
    void Terminate() override;
    void Update(float deltaTime) override;
    void Draw() const override;
    void OnMouseDown(int button, int mx, int my) override;
    void OnMouseMove(int mx, int my) override;
    void OnMouseUp(int button, int mx, int my) override;
    void OnKeyDown(int keyCode) override;
    void Hit();
    int GetMoney() const;
    void EarnMoney(int money);
    Engine::Point GetClientSize() const;
protected:
    void ConstructUI();
    void UIBtnClicked(int id);
    bool CheckSpaceValid(int x, int y);
    std::vector<std::vector<int>> CalculateBFSDistance();
    void ReadMap();
    void ReadEnemyWave();
    void buff_add(int state);
private:
    std::list<int> keyStrokes;
};

#endif // PLAYSCENE_HPP