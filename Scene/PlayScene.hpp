#ifndef PLAYSCENE_HPP
#define PLAYSCENE_HPP
#include <allegro5/allegro_audio.h>
#include <list>
#include <memory>
#include <utility>
#include <vector>
#include <queue>

#include "Character/Character.hpp"
#include "Engine/IScene.hpp"
#include "Engine/Point.hpp"
#include "Weapon/Gun.hpp"
#include "Weapon/MeleeWeapon.hpp"
#include "Drop/coin.hpp" // 新增 coin.hpp 包含
#include "Store/Store.hpp"
#include "Minimap/Minimap.hpp"
#include "Enemy/Boss.hpp"

class Turret;
namespace Engine {
    class Group;
    class Image;
    class Label;
    class Sprite;
}   // namespace Engine

class PlayScene final : public Engine::IScene {
public:
    enum TileType {
        TILE_WATER,
        TILE_GRASS,
        TILE_ROCK,
        TILE_OCCUPIED,
        TILE_BRIDGE,
        TILE_HOME,
        TILE_STORE,
        TILE_FOUNTAIN,
        TILE_BOSSROOM,
    };
    ALLEGRO_SAMPLE_ID bgmId;
    std::shared_ptr<ALLEGRO_SAMPLE_INSTANCE> deathBGMInstance;
    enum class WeaponType { // Added enum for weapon switching
        GUN,
        MELEE
    };
protected:
    int lives;
    int money;
    int SpeedMult;
public:
    //// new
    int scenenum;//0 for play, 1 for home, 2 for boss
    int homeposi,homeposj;
    int homeset;
    int gohomekey = 0;

    int bossroomposi,bossroomposj;
    int bossroomset;
    int gobossroomkey = 0;

    int storeposi,storeposj;
    int storeset;
    int gostorekey = 0;
    bool buying;


    bool isBossActive;  // 新增：標誌位，表示 Boss 是否活躍
   // Boss* currentBoss;  // 新增：指向當前活躍的 Boss

    float turret_coin_mul = 1.0f; 
    float turret_coolDown_mul = 1.0f;
    int coin_lv = 0;
    int coolDown_lv = 0;
    Engine::Label* coin_lv_l;
    Engine::Label* coolDown_lv_l = 0;

    float player_exp;
    float player_level = 0;
    int player_skill_point = 1;
    Engine::Label* player_exp_l;
    Engine::Label* player_level_l;
    Engine::Label* player_skill_point_l;

    Engine::Character* character;
    Engine::Label* SkillWarn;

    MiniMap miniMap;
    Engine::Gun* gun;
    Engine::MeleeWeapon* sword;
    WeaponType currentWeapon;

    Store* store;

    Engine::Point CameraPos;

    void buff_adder(int state);

    std::queue<int> level_req;

    static bool DebugMode;
    static const std::vector<Engine::Point> directions;
    static int MapWidth, MapHeight;
    static const int BlockSize;
    static const float DangerTime;
    static const Engine::Point SpawnGridPoint;
    static const Engine::Point EndGridPoint;
    static const std::vector<int> code;
    int MapId;
    float ticks;
    float deathCountDown;

    //// new
    Group *DroneGroup;

    Group *TileMapGroup;
    Group *GroundEffectGroup;
    Group *DebugIndicatorGroup;
    Group *BulletGroup;
    Group* WeaponBulletGroup;
    Group* EnemyBulletGroup;
    Group *TowerGroup;
    Group *EnemyGroup;
    Group *EffectGroup;
    Group *UIGroup;
    Engine::Label *UIMoney;
    Engine::Label *UILives;
    Engine::Label *UIHome;
    Engine::Label *UIBossroom;
    Engine::Label *UIStore;
    Engine::Image *imgTarget;
    Engine::Sprite *dangerIndicator;
    Turret *preview;
    std::vector<std::vector<TileType>> mapState;
    std::vector<std::vector<int>> mapDistance;
    std::list<std::pair<int, float>> enemyWaveData;
    std::list<int> keyStrokes;
    static Engine::Point GetClientSize();
    explicit PlayScene() = default;
    void Initialize() override;
    void Terminate() override;
    void Update(float deltaTime) override;
    void Draw() const override;
    void OnMouseDown(int button, int mx, int my) override;
    void OnMouseMove(int mx, int my) override;
    void OnMouseUp(int button, int mx, int my) override;
    void OnKeyDown(int keyCode) override;
    void Hit(float dmg);
    int GetMoney() const;
    void EarnMoney(int money);
    void ReadMap();
    void ReadHomeMap();
    void ReadBossroomMap();
    void ReadEnemyWave();
    void ConstructUI();
    void UIBtnClicked(int id);
    void drawedgebush();
    bool CheckSpaceValid(int x, int y);
    std::vector<std::vector<int>> CalculateBFSDistance(Engine::Point start);
    std::vector<Engine::Point> FindPathAStar(Engine::Point start, Engine::Point end);
    Engine::Point GetValidSpawnPoint();
    void SpawnCoin(float x, float y, int value);
    bool tile_crossable(int t) ;
};
#endif
// PLAYSCENE_HPP
// #ifndef PLAYSCENE_HPP
// #define PLAYSCENE_HPP
// #include <allegro5/allegro_audio.h>
// #include <list>
// #include <memory>
// #include <utility>
// #include <vector>
// #include <queue>

// #include "Character/Character.hpp"
// #include "Engine/IScene.hpp"
// #include "Engine/Point.hpp"

// #include "Weapon/Gun.hpp" // 新增：引用 Gun.hpp

// class Turret;
// namespace Engine {
//     class Group;
//     class Image;
//     class Label;
//     class Sprite;
// }   // namespace Engine

// class PlayScene final : public Engine::IScene {
// public: //// new 本來是 privte
//     enum TileType {
//         TILE_WATER,
//         TILE_GRASS,
//         TILE_ROCK,
//         TILE_OCCUPIED,
//         TILE_BRIDGE,
//         TILE_HOME,
//     };
//     ALLEGRO_SAMPLE_ID bgmId;
//     std::shared_ptr<ALLEGRO_SAMPLE_INSTANCE> deathBGMInstance;

// protected:
//     int lives;
//     int money;
//     int SpeedMult;

// public:
//     //// new
//     int homeposi,homeposj;
//     int homeset = 0;
//     float turret_coin_mul = 1.0f; 
//     float turret_coolDown_mul = 1.0f; // 越小 頻率越高
    
//     int coin_lv = 0;
//     int coolDown_lv = 0;
//     Engine::Label* coin_lv_l;
//     Engine::Label* coolDown_lv_l = 0;

//     float player_exp;
//     float player_level = 0;
//     int player_skill_point = 1;
//     Engine::Label* player_exp_l;
//     Engine::Label* player_level_l;
//     Engine::Label* player_skill_point_l;

//     Engine::Character* character;
//     Engine::Gun* gun; // 新增：槍物件

//     //// new
//     Engine::Point CameraPos;


//     void buff_adder(int state);

//     std::queue<int> level_req;

//     static bool DebugMode;
//     static const std::vector<Engine::Point> directions;
//     static const int MapWidth, MapHeight;
//     static const int BlockSize;
//     static const float DangerTime;
//     static const Engine::Point SpawnGridPoint;
//     static const Engine::Point EndGridPoint;
//     static const std::vector<int> code;
//     int MapId;
//     float ticks;
//     float deathCountDown;
//     // Map tiles.
//     Group *TileMapGroup;
//     Group *GroundEffectGroup;
//     Group *DebugIndicatorGroup;
//     Group *BulletGroup;
//     Group* WeaponBulletGroup;
//     Group *TowerGroup;
//     Group *EnemyGroup;
//     Group *EffectGroup;
//     Group *UIGroup;
//     Engine::Label *UIMoney;
//     Engine::Label *UILives;
//     Engine::Image *imgTarget;
//     Engine::Sprite *dangerIndicator;
//     Turret *preview;
//     std::vector<std::vector<TileType>> mapState;
//     std::vector<std::vector<int>> mapDistance;
//     std::list<std::pair<int, float>> enemyWaveData;
//     std::list<int> keyStrokes;
//     static Engine::Point GetClientSize();
//     explicit PlayScene() = default;
//     void Initialize() override;
//     void Terminate() override;
//     void Update(float deltaTime) override;
//     void Draw() const override;
//     void OnMouseDown(int button, int mx, int my) override;
//     void OnMouseMove(int mx, int my) override;
//     void OnMouseUp(int button, int mx, int my) override;
//     void OnKeyDown(int keyCode) override;
//     void Hit();
//     int GetMoney() const;
//     void EarnMoney(int money);
//     void ReadMap();
//     void ReadEnemyWave();
//     void ConstructUI();
//     void UIBtnClicked(int id);
//     bool CheckSpaceValid(int x, int y);
//     std::vector<std::vector<int>> CalculateBFSDistance();
//     std::vector<Engine::Point> FindPathAStar(Engine::Point start, Engine::Point end);
//     // void ModifyReadMapTiles();
// };
// #endif   // PLAYSCENE_HPP
