#include <allegro5/allegro.h>
#include <allegro5/allegro_audio.h>
#include <algorithm>
#include <cmath>
#include <fstream>
#include <functional>
#include <memory>
#include <queue>
#include <string>
#include <vector>
#include "Enemy/Enemy.hpp"
#include "Enemy/PlaneEnemy.hpp"
#include "Enemy/GodEnemy.hpp"
#include "Enemy/SoldierEnemy.hpp"
#include "Enemy/TankEnemy.hpp"
#include "Turret/CoolTurret.hpp"
#include "Tool/ShovelTool.hpp"
#include "Engine/AudioHelper.hpp"
#include "Engine/GameEngine.hpp"
#include "Engine/Group.hpp"
#include "Engine/LOG.hpp"
#include "Engine/Resources.hpp"
#include "PlayScene.hpp"
#include "Turret/LaserTurret.hpp"
#include "Turret/MachineGunTurret.hpp"
#include "Turret/TurretButton.hpp"
#include "UI/Animation/DirtyEffect.hpp"
#include "UI/Animation/Plane.hpp"
#include "UI/Component/Label.hpp"

bool PlayScene::DebugMode = false;
const std::vector<Engine::Point> PlayScene::directions = { Engine::Point(-1, 0), Engine::Point(0, -1), Engine::Point(1, 0), Engine::Point(0, 1) };
const float PlayScene::DangerTime = 7.61f;
const Engine::Point PlayScene::SpawnGridPoint = Engine::Point(-1, 0);
const Engine::Point PlayScene::EndGridPoint = Engine::Point(MapWidth, MapHeight - 1);
const std::vector<int> PlayScene::code = {
    ALLEGRO_KEY_UP, ALLEGRO_KEY_UP, ALLEGRO_KEY_DOWN, ALLEGRO_KEY_DOWN,
    ALLEGRO_KEY_LEFT, ALLEGRO_KEY_RIGHT, ALLEGRO_KEY_LEFT, ALLEGRO_KEY_RIGHT,
    ALLEGRO_KEY_B, ALLEGRO_KEY_A, ALLEGRO_KEY_LSHIFT, ALLEGRO_KEY_ENTER
};

Engine::Point PlayScene::GetClientSize() const {
    return Engine::Point(MapWidth * BlockSize, MapHeight * BlockSize);
}

void PlayScene::Initialize() {
    std::cerr << "Initializing PlayScene with MapId: " << MapId << std::endl;
    if (MapId <= 0) {
        std::cerr << "Invalid MapId: " << MapId << ", setting to default 1" << std::endl;
        MapId = 1; // 預設第一關
    }
    mapState.clear();
    keyStrokes.clear();
    while (!level_req.empty()) level_req.pop();
    level_req.push(100); level_req.push(150); level_req.push(200); level_req.push(700);
    level_req.push(1500); level_req.push(3000); level_req.push(6000); level_req.push(10451);
    player_exp = 100;
    player_level = 0;
    player_skill_point = 1;
    turret_coin_mul = 1.0f;
    turret_coolDown = 1.0f;
    coin_lv = coolDown_lv = 0;

    ticks = 0;
    deathCount = -1;
    lives = 10;
    money = 666;
    SpeedMult = 1;

    // 添加群組
    AddNewObject(TileMapGroup = new Group());
    AddNewObject(GroundEffectGroup = new Group());
    AddNewObject(DebugIndicatorGroup = new Group());
    AddNewObject(TowerGroup = new Group());
    AddNewObject(EnemyGroup = new Group());
    AddNewObject(BulletGroup = new Group());
    AddNewObject(EffectGroup = new Group());
    AddNewControlObject(UIGroup = new Group());

    // 讀取地圖
    ReadMap();

    // 構建 UI
    ConstructUI();

    // 生成主角
    zoomLevel = 2.0f; // 鏡頭放大 2 倍
    player = new Engine::Sprite("play/enemy-1.png", BlockSize * 1 + BlockSize / 2, BlockSize * 1 + BlockSize / 2); // 初始在 (1,1)
    playerSpeed = 200.0f;
    cameraX = 0;
    cameraY = 0;
    AddNewObject(player);

    // 預載資源
    imgTarget = new Engine::Image("play/target.png", 0, 0);
    imgTarget->Visible = false;
    preview = nullptr;
    UIGroup->AddNewObject(imgTarget);
    deathBGMInstance = Engine::Resources::GetInstance().GetSampleInstance("astronomia.ogg");
    Engine::Resources::GetInstance().GetBitmap("lose/benjamin-happy.png");
    //bgmId = AudioHelper::PlayBGM("play.ogg");
}

void PlayScene::Terminate() {
    //AudioHelper::StopBGM(bgmId);
    AudioHelper::StopSample(deathBGMInstance);
    deathBGMInstance = std::shared_ptr<ALLEGRO_SAMPLE_INSTANCE>();
    IScene::Terminate();
}

void PlayScene::Update(float deltaTime) {
    // 使用鍵盤狀態檢查移動
    ALLEGRO_KEYBOARD_STATE keyboardState;
    al_get_keyboard_state(&keyboardState);
    if (al_key_down(&keyboardState, ALLEGRO_KEY_W)) {
        player->Position.y -= playerSpeed * deltaTime;
    }
    if (al_key_down(&keyboardState, ALLEGRO_KEY_S)) {
        player->Position.y += playerSpeed * deltaTime;
    }
    if (al_key_down(&keyboardState, ALLEGRO_KEY_A)) {
        player->Position.x -= playerSpeed * deltaTime;
    }
    if (al_key_down(&keyboardState, ALLEGRO_KEY_D)) {
        player->Position.x += playerSpeed * deltaTime;
    }

    // 限制主角移動範圍
    player->Position.x = std::max(static_cast<float>(BlockSize / 2), 
                                  std::min(player->Position.x, static_cast<float>(MapWidth * BlockSize - BlockSize / 2)));
    player->Position.y = std::max(static_cast<float>(BlockSize / 2), 
                                  std::min(player->Position.y, static_cast<float>(MapHeight * BlockSize - BlockSize / 2)));

    // 計算鏡頭偏移
    cameraX = player->Position.x - (Engine::GameEngine::GetInstance().GetScreenSize().x / 2.0f) / zoomLevel;
    cameraY = player->Position.y - (Engine::GameEngine::GetInstance().GetScreenSize().y / 2.0f) / zoomLevel;

    // 限制鏡頭範圍
    cameraX = std::max(0.0f, std::min(cameraX, static_cast<float>(MapWidth * BlockSize) - Engine::GameEngine::GetInstance().GetScreenSize().x / zoomLevel));
    cameraY = std::max(0.0f, std::min(cameraY, static_cast<float>(MapHeight * BlockSize) - Engine::GameEngine::GetInstance().GetScreenSize().y / zoomLevel));

    // 更新場景物件
    IScene::Update(deltaTime);
}

void PlayScene::Draw() const {
    ALLEGRO_TRANSFORM transform;
    al_identity_transform(&transform);
    al_scale_transform(&transform, zoomLevel, zoomLevel);
    al_translate_transform(&transform, -cameraX, -cameraY);
    al_use_transform(&transform);

    IScene::Draw();

    // 繪製 UI（不縮放）
    al_identity_transform(&transform);
    al_use_transform(&transform);
    UIGroup->Draw();
}

void PlayScene::OnMouseDown(int button, int mx, int my) {
    IScene::OnMouseDown(button, mx, my);
}

void PlayScene::OnMouseMove(int mx, int my) {
    IScene::OnMouseMove(mx, my);
}

void PlayScene::OnMouseUp(int button, int mx, int my) {
    IScene::OnMouseUp(button, mx, my);
}

void PlayScene::OnKeyDown(int keyCode) {
    IScene::OnKeyDown(keyCode);
}

void PlayScene::Hit() {
    lives--;
    if (lives <= 0) {
        Engine::GameEngine::GetInstance().ChangeScene("lose");
    }
    UILives->Text = std::string("Life ") + std::to_string(lives);
}

int PlayScene::GetMoney() const {
    return money;
}

void PlayScene::EarnMoney(int money) {
    this->money += (money > 0) ? money * turret_coin_mul : money;
    UIMoney->Text = std::string("$") + std::to_string(this->money);
    if (money == 10000) return;
    if (player_exp >= level_req.front() && player_level < 8) {
        float divid = player_exp - level_req.front();
        player_level += 1;
        player_skill_point += 1;
        player_exp = divid;
        level_req.pop();
    } else if (player_level >= 8) {
        player_exp_l->Text = std::string("EXP: MAX");
        player_level_l->Text = std::string("Level: ") + std::to_string(player_level) + "/8";
        player_skill_point_l->Text = std::string("Point: ") + std::to_string(player_skill_point);
        return;
    }
    player_exp_l->Text = std::string("EXP: ") + std::to_string(static_cast<int>(player_exp)) + "/" + std::to_string(level_req.front());
    player_level_l->Text = std::string("Level: ") + std::to_string(player_level) + "/8";
    player_skill_point_l->Text = std::string("Point: ") + std::to_string(player_skill_point);
}

void PlayScene::ReadMap() {
    std::string filename = std::string("Resource/map") + std::to_string(MapId) + ".txt";
    std::ifstream fin(filename);
    if (!fin.is_open()) {
        std::cerr << "Failed to open map file: " << filename << std::endl;
        throw std::ios_base::failure("Map file not found: " + filename);
    }

    std::vector<bool> mapData;
    std::string line;
    int rowCount = 0;
    while (std::getline(fin, line)) {
        // 移除行尾的 \r（適用於 Windows 的 \r\n）
        if (!line.empty() && line.back() == '\r') {
            line.pop_back();
        }
        if (line.length() != MapWidth) {
            std::cerr << "Invalid map row length at row " << rowCount + 1 << ": expected " << MapWidth << ", got " << line.length() << std::endl;
            fin.close();
            throw std::ios_base::failure("Map data is corrupted: invalid row length.");
        }
        for (char c : line) {
            if (c == '0') {
                mapData.push_back(false);
            } else if (c == '1') {
                mapData.push_back(true);
            } else {
                std::cerr << "Invalid character in map at row " << rowCount + 1 << ": " << c << std::endl;
                fin.close();
                throw std::ios_base::failure("Map data is corrupted: invalid character.");
            }
        }
        rowCount++;
    }
    fin.close();

    if (rowCount != MapHeight || mapData.size() != MapWidth * MapHeight) {
        std::cerr << "Invalid map dimensions: expected " << MapWidth << "x" << MapHeight << ", got " << mapData.size() << " cells, " << rowCount << " rows" << std::endl;
        throw std::ios_base::failure("Map data is corrupted: incorrect dimensions.");
    }

    mapState = std::vector<std::vector<TileType>>(MapHeight, std::vector<TileType>(MapWidth));
    for (int i = 0; i < MapHeight; i++) {
        for (int j = 0; j < MapWidth; j++) {
            const int num = mapData[i * MapWidth + j];
            mapState[i][j] = num ? TILE_FLOOR : TILE_DIRT;
            if (num)
                TileMapGroup->AddNewObject(new Engine::Image("play/floor.png", j * BlockSize, i * BlockSize, BlockSize, BlockSize));
            else
                TileMapGroup->AddNewObject(new Engine::Image("play/dirt.png", j * BlockSize, i * BlockSize, BlockSize, BlockSize));
        }
    }
}

void PlayScene::ReadEnemyWave() {
}

void PlayScene::ConstructUI() {
    UIGroup->AddNewObject(new Engine::Image("play/sand.png", 1280, 0, 320, 832));
    UIGroup->AddNewObject(new Engine::Label(std::string("Stage ") + std::to_string(MapId), "pirulen.ttf", 32, 1294, 0));
    UIGroup->AddNewObject(UIMoney = new Engine::Label(std::string("$") + std::to_string(money), "pirulen.ttf", 24, 1294, 48));
    UIGroup->AddNewObject(UILives = new Engine::Label(std::string("Life ") + std::to_string(lives), "pirulen.ttf", 24, 1294, 88));
    UIGroup->AddNewObject(player_exp_l = new Engine::Label(std::string("EXP ") + std::to_string(static_cast<int>(player_exp)) + "/" + std::to_string(level_req.front()), "pirulen.ttf", 24, 1294, 130));
    UIGroup->AddNewObject(player_level_l = new Engine::Label(std::string("Level ") + std::to_string(player_level) + "/8", "pirulen.ttf", 24, 1294, 155));
    UIGroup->AddNewObject(player_skill_point_l = new Engine::Label(std::string("Points: ") + std::to_string(player_skill_point), "pirulen.ttf", 24, 1294, 180));
    int w = Engine::GameEngine::GetInstance().GetScreenSize().x;
    int h = Engine::GameEngine::GetInstance().GetScreenSize().y;
    int shift = 135 + 25;
    dangerIndicator = new Engine::Sprite("play/benjamin.png", w - shift, h - shift);
    dangerIndicator->Tint.a = 0;
    UIGroup->AddNewObject(dangerIndicator);
}

void PlayScene::buff_add(int state) {
}

void PlayScene::UIBtnClicked(int id) {
}

bool PlayScene::CheckSpaceValid(int x, int y) {
    return true;
}

std::vector<std::vector<int>> PlayScene::CalculateBFSDistance() {
    return std::vector<std::vector<int>>(MapHeight, std::vector<int>(MapWidth, -1));
}