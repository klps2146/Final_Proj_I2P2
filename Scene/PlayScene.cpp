#include <algorithm>
#include <allegro5/allegro.h>
#include <cmath>
#include <fstream>
#include <iostream>
#include <functional>
#include <memory>
#include <queue>
#include <string>
#include <vector>
#include <utility>
#include <random>
#include "Enemy/Enemy.hpp"
#include "Enemy/PlaneEnemy.hpp"
#include "Enemy/GodEnemy.hpp"

#include "Enemy/EnemyBullet.hpp"
#include "Enemy/Bomb.hpp"
#include "Enemy/BombThrowerEnemy.hpp"

#include "Turret/CoolTurret.hpp"
#include "Tool/ShovelTool.hpp"
#include "Character/Character.hpp"
#include "Engine/IScene.hpp"
#include "Enemy/SoldierEnemy.hpp"
#include "Enemy/TankEnemy.hpp"
#include "Enemy/ShootEnemy.hpp"
#include "Engine/AudioHelper.hpp"
#include "Engine/GameEngine.hpp"
#include "Engine/Group.hpp"
#include "Engine/LOG.hpp"
#include "Engine/Resources.hpp"
#include "Engine/SpriteFixed.hpp"
#include "PlayScene.hpp"
#include "Turret/LaserTurret.hpp"
#include "Turret/MachineGunTurret.hpp"
#include "Turret/TurretButton.hpp"
#include "UI/Animation/DirtyEffect.hpp"
#include "UI/Animation/Plane.hpp"
#include "UI/Component/Label.hpp"
#include "UI/Component/ColoredRectangle.hpp"

#include "Skill/SkillBase.hpp"
#include "Skill/DashSkill.hpp"
#include "Skill/AreaSkill.hpp"
#include "Skill/SummonDroneSkill.hpp"
#include "Skill/NuclearSkill.hpp"
#include "Skill/ShieldSkill.hpp"
#include "Skill/OrbitBulletSkill.hpp"

#include "Minimap/Minimap.hpp"

#include "Drop/coin.hpp"

#include "Enemy/BossEnemy.hpp"
#include "Enemy/Boss1.hpp"
#include "Enemy/Boss2.hpp"
#include "Enemy/Boss3.hpp"
#include "Store/Store.hpp"

bool PlayScene::DebugMode = false;
const std::vector<Engine::Point> PlayScene::directions = { Engine::Point(-1, 0), Engine::Point(0, -1), Engine::Point(1, 0), Engine::Point(0, 1) };
const int PlayScene::BlockSize = 64;
const float PlayScene::DangerTime = 7.61;
int PlayScene::MapWidth = 40, PlayScene::MapHeight = 30;
const Engine::Point PlayScene::SpawnGridPoint = Engine::Point(-1, 0);
const Engine::Point PlayScene::EndGridPoint = Engine::Point(MapWidth, MapHeight - 1);
const std::vector<int> PlayScene::code = {
    ALLEGRO_KEY_UP, ALLEGRO_KEY_UP, ALLEGRO_KEY_DOWN, ALLEGRO_KEY_DOWN,
    ALLEGRO_KEY_LEFT, ALLEGRO_KEY_RIGHT, ALLEGRO_KEY_LEFT, ALLEGRO_KEY_RIGHT,
    ALLEGRO_KEY_B, ALLEGRO_KEY_A, ALLEGRO_KEY_LSHIFT, ALLEGRO_KEY_ENTER
};
Engine::Point PlayScene::GetClientSize() {
    return Engine::Point(MapWidth * BlockSize, MapHeight * BlockSize);
}
void PlayScene::Initialize() {
    SetBackgroundImage("map/mapBG.png");
    Engine::Resources::GetInstance().GetBitmap("play/bomb.png");

    mapState.clear();
    keyStrokes.clear();
    while (!level_req.empty()) level_req.pop();
    level_req.push(100);
    level_req.push(150);
    level_req.push(200);
    level_req.push(700);
    level_req.push(1500);
    level_req.push(3000);
    level_req.push(6000);
    level_req.push(10451);
    player_exp = player_level = 0;
    player_skill_point = 1;
    turret_coin_mul = turret_coolDown_mul = 1.0f;
    coin_lv = coolDown_lv = 0;

    ticks = 0;
    deathCountDown = -1;
    lives = 5000;
    money = 666;
    SpeedMult = 1;
    homeset=0;
    storeset=0;
    bossroomset=0;
    fountainset=0;
    buying=false;


    bossSpawned = false; // 初始化Boss生成標記
    bossAlive = false;   // 初始化Boss存活標記


    /*if(scenenum==1){
        MapHeight = 18;
    }*/
    

    //// chracter
    
    character = new Engine::Character("character/moving.png", 500, 500, 0, 0, 0.5f, 0.5f, 200, 32);
    character->SetSpriteSource(0, 0, 96, 96);
    character->SetSize(70, 70);

    character->AddSkill(new DashSkill());
    character->AddSkill(new ShieldSkill());
    character->AddSkill(new AreaSkill("MagicCircle", "skill/trump.png", 140, 8.0f, 5.0f));
    character->AddSkill(new SummonDroneSkill(2, 25.0f, 500.0f, 0.9f));
    character->AddSkill(new OrbitSkill(0, 0));
    character->AddSkill(new NuclearSkill());
    character->VisableLevel = 2;
    AddNewControlObject(character);

    CameraPos = Engine::Point(0, 0);

    store = new Store();

    std::string prevscene;

    //紀錄資訊
    std::ifstream fin("../Resource/whichscene.txt");
    char whichscene = '0';
    if (fin.is_open()) {
        const int skillnum = 6;
        std::string money_record, hp_record, shield_record, power_record, speed_record, level_record;
        std::string  skill_level[skillnum];
        std::string  skill_unlock[skillnum];
        fin >> whichscene >> prevscene >> money_record >> hp_record >> shield_record >> power_record >> speed_record >> level_record;
        for(int i=0;i<skillnum;i++)fin >> skill_level[i];
        for(int i=0;i<skillnum;i++)fin >> skill_unlock[i];
        fin.close();
        money=std::stoi(money_record);
        character->HP=std::stoi(hp_record);
        character->shield=std::stoi(shield_record);
        character->POWER=std::stoi(power_record);
        character->speed=std::stoi(speed_record);
        player_level=std::stoi(level_record);
        for(int i=0;i<skillnum;i++)character->itemBar_.slots[i]->level=std::stoi(skill_level[i]);
        for(int i=0;i<skillnum;i++)character->itemBar_.slots[i]->isUnlocked=(std::stoi(skill_unlock[i]) == 1 ? 1 : 0);
    } else {
        std::cerr << "Failed to read whichscene.txt" << std::endl;
    }

    // 設置場景編號
    if(whichscene == '0')scenenum = 0;
    if(whichscene == '1')scenenum = 1;
    if(whichscene == '2')scenenum = 2;

    if(scenenum == 1){
        if(prevscene == "0"){
            character->Position.x+=160;
            character->Position.y +=700;
        }
        else if(prevscene == "2"){
            character->Position.x +=800;
            character->Position.y -=200;
        }
    }

    //if(scenenum == 1)character->Position.y+=500;

    //// 小地圖
    miniMap = MiniMap(
        MapWidth, MapHeight,
        1295.f, 5.f,// 座標
        300.f, 300.f // 大小 
    ),

    //gun = new Engine::Gun(this, character->Position);
    //AddNewControlObject(gun);

    //sword = new Engine::MeleeWeapon(this, character->Position);
    //AddNewControlObject(sword);


    // currentWeapon = WeaponType::GUN; // Default to gun
    // gun->isActive = true;
    // sword->isActive = false;

    character->AddWeapon(std::make_unique<Engine::Gun>(this, character->Position));
    character->AddWeapon(std::make_unique<Engine::MeleeWeapon>(this, character->Position));

    AddNewObject(EnemyBulletGroup = new Group());
    AddNewObject(DroneGroup = new Group());
    
    AddNewObject(TileMapGroup = new Group());
    AddNewObject(GroundEffectGroup = new Group());
    AddNewObject(DebugIndicatorGroup = new Group());
    AddNewObject(TowerGroup = new Group());
    AddNewObject(EnemyGroup = new Group());
    AddNewObject(BulletGroup = new Group());

    AddNewObject(DroneBulletGroup = new Group());

    AddNewObject(WeaponBulletGroup = new Group());
    AddNewObject(EffectGroup = new Group());
    AddNewControlObject(UIGroup = new Group());
    if(scenenum==0||scenenum==2) ReadMap();
    else if(scenenum==1) ReadHomeMap();

    ReadEnemyWave();
    ConstructUI();
    imgTarget = new Engine::Image("play/target.png", 0, 0); // pkboie is handsome
    imgTarget->Visible = false;
    preview = nullptr;
    UIGroup->AddNewObject(imgTarget);
    deathBGMInstance = Engine::Resources::GetInstance().GetSampleInstance("astronomia.ogg");
    Engine::Resources::GetInstance().GetBitmap("lose/benjamin-happy.png");
    bgmId = AudioHelper::PlayBGM("play.ogg");
    std::cout << "clear 1\n";
}
void PlayScene::Terminate() {
    AudioHelper::StopBGM(bgmId);
    AudioHelper::StopSample(deathBGMInstance);
    deathBGMInstance = std::shared_ptr<ALLEGRO_SAMPLE_INSTANCE>();

    IScene::Terminate();
}
void PlayScene::Update(float deltaTime) {

    WeaponBulletGroup->Update(deltaTime);
    EnemyBulletGroup->Update(deltaTime);
    DroneGroup->Update(deltaTime);

    // miniMap
    miniMap.SetEnemyPositions(EnemyGroup);
    miniMap.SetPlayer(character->Position, character->GetRotation());


    //go home
    if(gohomekey==0){
        UIHome->Text = std::string("");
        if(character->Position.x>homeposj*BlockSize-40 && character->Position.x<(homeposj+4)*BlockSize+40
            &&character->Position.y>homeposi*BlockSize-40 && character->Position.y<(homeposi+4)*BlockSize+40) gohomekey=1;
    }
    if(gohomekey==1){
        if(scenenum!=1)UIHome->Text = std::string("Press F to go home");
        else UIHome->Text = std::string("Press F to go adventure");
        if(!(character->Position.x>homeposj*BlockSize-40 && character->Position.x<(homeposj+4)*BlockSize+40
            &&character->Position.y>homeposi*BlockSize-40 && character->Position.y<(homeposi+4)*BlockSize+40)) gohomekey=0;
    }

    //go bossroom
    if(gobossroomkey==0){
        UIBossroom->Text = std::string("");
        if(character->Position.x>bossroomposj*BlockSize-40 && character->Position.x<(bossroomposj+4)*BlockSize+40
            &&character->Position.y>bossroomposi*BlockSize-40 && character->Position.y<(bossroomposi+4)*BlockSize+40) gobossroomkey=1;
    }
    if(gobossroomkey==1 && scenenum==1){
        UIBossroom->Text = std::string("Press F to go bossroom");
        if(!(character->Position.x>bossroomposj*BlockSize-40 && character->Position.x<(bossroomposj+4)*BlockSize+40
            &&character->Position.y>bossroomposi*BlockSize-40 && character->Position.y<(bossroomposi+4)*BlockSize+40)) gobossroomkey=0;
    }

    //go fountain
    if(fountainkey==0){
        
        UIFountain->Text = std::string("");
        UIShit->Text = std::string("");
        if(character->Position.x>fountainposj*BlockSize-40 && character->Position.x<(fountainposj+4)*BlockSize+40
            &&character->Position.y>fountainposi*BlockSize-40 && character->Position.y<(fountainposi+4)*BlockSize+40) fountainkey=1;
    }
    if(fountainkey==1 && scenenum==1){
        UIFountain->Text = std::string("Press F to drink fountain water");
        UIShit->Text = std::string("Press G to shit in the fountain");
        if(!(character->Position.x>fountainposj*BlockSize-40 && character->Position.x<(fountainposj+4)*BlockSize+40
            &&character->Position.y>fountainposi*BlockSize-40 && character->Position.y<(fountainposi+4)*BlockSize+40)) fountainkey=0;
    }
    
    //go store
    if(gostorekey==0){
        UIStore->Text = std::string("");
        if(character->Position.x>storeposj*BlockSize-40 && character->Position.x<(storeposj+4)*BlockSize+40
            &&character->Position.y>storeposi*BlockSize-40 && character->Position.y<(storeposi+4)*BlockSize+40) gostorekey=1;
    }
    if(gostorekey==1  && scenenum==1){
        UIStore->Text = std::string("Press F to go store");
        if(!(character->Position.x>storeposj*BlockSize-40 && character->Position.x<(storeposj+4)*BlockSize+40
            &&character->Position.y>storeposi*BlockSize-40 && character->Position.y<(storeposi+4)*BlockSize+40)) gostorekey=0;
    }
    // std::cout << "clear bef2\n";
    if (store)
    store->Update(deltaTime);
    else std::cout << "NO store INITIALIZE\n";
    // std::cout << "clear 2\n";
    
    if (SpeedMult == 0)
        deathCountDown = -1;
    else if (deathCountDown != -1)
        SpeedMult = 1;

    // Compute distance map from player's position
    int playerX = static_cast<int>(floor(character->Position.x / BlockSize));
    int playerY = static_cast<int>(floor(character->Position.y / BlockSize));
    if (playerX < 0) playerX = 0;
    if (playerX >= MapWidth) playerX = MapWidth - 1;
    if (playerY < 0) playerY = 0;
    if (playerY >= MapHeight) playerY = MapHeight - 1;
    Engine::Point playerGrid(playerX, playerY);
    mapDistance = CalculateBFSDistance(playerGrid);
    
    
    if (!bossSpawned && money >= 6667 && scenenum!=1 && scenenum!=0) { 
        bossSpawned = true;
        bossAlive = true;
        SpawnBosses();
    }
    
    for (int i = 0; i < SpeedMult; i++) {
        if(!buying) IScene::Update(deltaTime);
        ticks += deltaTime;
        const float spawnInterval = 2.0f;
        static float spawnTimer = 0.0f;
        spawnTimer += deltaTime;
        if (spawnTimer >= spawnInterval && scenenum!=1 && scenenum!=2) {
            spawnTimer -= spawnInterval;
            int type = rand() % 6 + 1;
            Engine::Point spawnPos = GetValidSpawnPoint();
            Enemy* enemy;
            switch (type) {
                case 1:
                    EnemyGroup->AddNewObject(enemy = new SoldierEnemy(spawnPos.x, spawnPos.y));
                    break;
                case 2:
                    EnemyGroup->AddNewObject(enemy = new PlaneEnemy(spawnPos.x, spawnPos.y));
                    break;
                case 3:
                    EnemyGroup->AddNewObject(enemy = new TankEnemy(spawnPos.x, spawnPos.y));
                    break;
                case 4:
                    EnemyGroup->AddNewObject(enemy = new GodEnemy(spawnPos.x, spawnPos.y));
                    break;
                case 5:
                    EnemyGroup->AddNewObject(enemy = new ShootEnemy(spawnPos.x, spawnPos.y));
                    break;
                case 6:
                    EnemyGroup->AddNewObject(enemy = new BombThrowerEnemy(spawnPos.x, spawnPos.y));
                    break;
                default:
                    continue;
            }
            std::cout << "Enemy" << type << " spawned at position: (" 
                        << spawnPos.x << ", " << spawnPos.y << ")\n";
            enemy->Update(ticks);  
            
        }
    }
    if (preview) {
        preview->Position = Engine::GameEngine::GetInstance().GetMousePosition();
        preview->Update(deltaTime);
    }
}


void PlayScene::SpawnBosses() {
    Engine::Point spawnPos1 = GetValidSpawnPoint();
    Engine::Point spawnPos2 = GetValidSpawnPoint();
    Engine::Point spawnPos3 = GetValidSpawnPoint();

    Boss1* boss1 = new Boss1(spawnPos1.x, spawnPos1.y);
    Boss2* boss2 = new Boss2(spawnPos2.x, spawnPos2.y);
    Boss3* boss3 = new Boss3(spawnPos3.x, spawnPos3.y);
    
    EnemyGroup->AddNewObject(boss1);
    EnemyGroup->AddNewObject(boss2);
    EnemyGroup->AddNewObject(boss3);
}

void PlayScene::Draw() const {
    al_clear_to_color(al_map_rgb(135, 206, 235));
    IScene::Draw();
    //// new
    character->Draw();


    if (character->IsAlive()) {
        character->DrawBars();
        // Draw only the active weapon
        // if (currentWeapon == WeaponType::GUN) {
        //     gun->Draw();
        // } else if (currentWeapon == WeaponType::MELEE) {
        //     sword->Draw();
        // }
    }
    WeaponBulletGroup->Draw();
    EnemyBulletGroup->Draw();
    
    DroneGroup->Draw();


    if(buying)store->Draw();

    /// 小地圖
    miniMap.Draw();

    if (DebugMode) {
        for (int i = 0; i < MapHeight; i++) {
            for (int j = 0; j < MapWidth; j++) {
                if (mapDistance[i][j] != -1) {
                    Engine::Label label(std::to_string(mapDistance[i][j]), "pirulen.ttf", 32, (j + 0.5) * BlockSize, (i + 0.5) * BlockSize);
                    label.Anchor = Engine::Point(0.5, 0.5);
                    label.Draw();
                }
            }
        }
    }
}
void PlayScene::OnMouseDown(int button, int mx, int my) {
    if ((button & 1) && !imgTarget->Visible && preview) {
        UIGroup->RemoveObject(preview->GetObjectIterator());
        preview = nullptr;
    }
    IScene::OnMouseDown(button, mx, my);
    if (buying) {
        store->OnMouseDown(button, mx, my);
    }
}
void PlayScene::OnMouseMove(int mx, int my) {
    IScene::OnMouseMove(mx, my);
    const int x = mx / BlockSize;
    const int y = my / BlockSize;
    if (!preview || x < 0 || x >= MapWidth || y < 0 || y >= MapHeight) {
        imgTarget->Visible = false;
        return;
    }
    imgTarget->Visible = true;
    imgTarget->Position.x = x * BlockSize;
    imgTarget->Position.y = y * BlockSize;
}
void PlayScene::OnMouseUp(int button, int mx, int my) {
    IScene::OnMouseUp(button, mx, my);
    if (!imgTarget->Visible)
        return;
    const int x = mx / BlockSize;
    const int y = my / BlockSize;
    if (button & 1) {
        if (mapState[y][x] == TILE_OCCUPIED) {
            if (dynamic_cast<ShovelTool*>(preview) != nullptr) {
                dynamic_cast<ShovelTool*>(preview)->OnClick(x, y);
                UIGroup->RemoveObject(preview->GetObjectIterator());
                preview = nullptr;
                return;
            }
        }
        if (mapState[y][x] != TILE_OCCUPIED) {
            if (!preview)
                return;
            if (dynamic_cast<ShovelTool*>(preview) != nullptr) {
                UIGroup->RemoveObject(preview->GetObjectIterator());
                preview = nullptr;
                return;
            }
            if (!CheckSpaceValid(x, y)) {
                Engine::Sprite *sprite;
                GroundEffectGroup->AddNewObject(sprite = new DirtyEffect("play/target-invalid.png", 1, x * BlockSize + BlockSize / 2, y * BlockSize + BlockSize / 2));
                sprite->Rotation = 0;
                return;
            }
            EarnMoney(-preview->GetPrice());
            preview->GetObjectIterator()->first = false;
            UIGroup->RemoveObject(preview->GetObjectIterator());
            preview->Position.x = x * BlockSize + BlockSize / 2;
            preview->Position.y = y * BlockSize + BlockSize / 2;
            preview->coolDown = preview->coolDown * turret_coolDown_mul;
            preview->Enabled = true;
            preview->Preview = false;
            preview->Tint = al_map_rgba(255, 255, 255, 255);
            TowerGroup->AddNewObject(preview);
            preview->Update(0);
            preview = nullptr;
            mapState[y][x] = TILE_OCCUPIED;
            OnMouseMove(mx, my);
        }
    }
}
void PlayScene::OnKeyDown(int keyCode) {
    IScene::OnKeyDown(keyCode);
    static int idx = 0;
    if (PlayScene::code[idx] == keyCode) {
        idx++;
    } else {
        idx = 0;
    }
    if (idx >= PlayScene::code.size()) {
        // UIGroup->AddNewObject(new Plane);
        money += 100000;
        idx = 0;
    }
    if (keyCode == ALLEGRO_KEY_TAB) {
        DebugMode = !DebugMode;
    } else {
        keyStrokes.push_back(keyCode);
        if (keyStrokes.size() > code.size())
            keyStrokes.pop_front();
    }


    if (keyCode == ALLEGRO_KEY_Q) {
        // currentWeapon = WeaponType::GUN;
        // gun->isActive = true;
        // sword->isActive = false;
    }else if (keyCode == ALLEGRO_KEY_E){
        // currentWeapon = WeaponType::MELEE;
        // gun->isActive = false;
        // sword->isActive = true;
    }
                    
    else if (keyCode >= ALLEGRO_KEY_0 && keyCode <= ALLEGRO_KEY_9) {
        // Hotkey for Speed up.
        // SpeedMult = keyCode - ALLEGRO_KEY_0;
    }

    if(fountainkey && keyCode == ALLEGRO_KEY_G){
        shit=1;
        TileMapGroup->AddNewObject(new Engine::Image("play/shitfountain.png", fountainposj * BlockSize, fountainposi * BlockSize, 4 * BlockSize, 4 * BlockSize));

    }

    if(fountainkey && keyCode == ALLEGRO_KEY_F){
        //gain power
        character->ChangePOWER(100);
        if(shit)character->ChangeHP(-100);
    }

    if((gohomekey||(gobossroomkey && scenenum==1)) && keyCode == ALLEGRO_KEY_F){
        char newScene='1';
        if(gohomekey)newScene = (scenenum == 1) ? '0' : '1';
        else if(gobossroomkey) newScene =  '2';
        else std::cout<<"errorrr\n";
        
        // 寫入新的值到 whichscene.txt
        std::ofstream fout("../Resource/whichscene.txt", std::ios::trunc);
        if (fout.is_open()) {
            fout << newScene << " "  // 切換場景編號
                << scenenum << " "
                << money << " "                          // 金幣
                << character->HP << " "                  // 血量
                << character->shield << " "              // 護盾
                << character->POWER << " "               // 能量
                << character->speed << " "               // 速度
                << player_level<<"\n";                    // 等級
            int skillnum=character->itemBar_.slots.size();  //error, unkown reason       
            skillnum=6;             
            for(int i=0;i<skillnum-1;i++)fout <<character->itemBar_.slots[i]->level<<" ";
            fout <<character->itemBar_.slots[skillnum-1]->level<<"\n";
            for(int i=0;i<skillnum-1;i++)fout <<(character->itemBar_.slots[i]->getUnlock()? 1 : 0 )<<" ";
            fout <<(character->itemBar_.slots[skillnum-1]->getUnlock()? 1 : 0 );
            fout.flush();
        } else {
            std::cerr << "Failed to save game data!" << std::endl;
            return;
        }
        // 切換場景
        Engine::GameEngine::GetInstance().ChangeScene("play");
    }

    //send keycode to store
    /*if (buying) {
        store->OnKeyDown(keyCode);
    }*/

    //bulid store
    if(scenenum==1 && gostorekey && keyCode == ALLEGRO_KEY_F) buying =! buying;
}
void PlayScene::Hit(float damage) {
    lives--;
    character->ChangeHP(-1 * damage);
    AudioHelper::PlaySample("hurt.wav", false, 5.0);
    if (lives <= 0) {
        Engine::GameEngine::GetInstance().ChangeScene("lose");
    }
}
int PlayScene::GetMoney() const {
    return money;
}
void PlayScene::EarnMoney(int money) {
    this->money += (money > 0) ? money * turret_coin_mul : money;
    UIMoney->Text = std::string("$") + std::to_string(this->money);
    //this->money += money;
}
void PlayScene::ReadMap() {
    std::string filename;
    //
    std::string whichfile = std::string("../Resource/whichscene.txt");
    std::ifstream f(whichfile);
    char scenenum;
    f >> scenenum;
    //
    if(scenenum=='0') filename = std::string("Resource/map") + std::to_string(MapId) + ".txt";
    else if(scenenum=='1') filename = std::string("Resource/homemap.txt");
    else if(scenenum=='2') filename = std::string("Resource/bossmap.txt");
    // Read map file.
    char c;
    std::vector<int> mapData;
    std::ifstream fin(filename);
    while (fin >> c) {
        switch (c) {
            case '0': mapData.push_back(0); break;
            case '1': mapData.push_back(1); break;
            case '2': mapData.push_back(2); break;
            case '3': mapData.push_back(3); break;
            case '4': mapData.push_back(4); break;
            case '\n':
            case '\r':
                if (static_cast<int>(mapData.size()) / MapWidth != 0)
                    throw std::ios_base::failure("Map data is corrupted.4");
                break;
            default: throw std::ios_base::failure("Map data is corrupted.5");
        }
    }
    fin.close();
    /*if (static_cast<int>(mapData.size()) != MapWidth * MapHeight)
        throw std::ios_base::failure("Map data is corrupted.6");*/
    mapState = std::vector<std::vector<TileType>>(MapHeight, std::vector<TileType>(MapWidth));
    std::string spriteSheetFile = "map/Overworld.png";
    static const std::pair<std::pair<int, int>, std::pair<int, int>> item_bias[10] = {
        {{3, 3}, {1, 1}},
        {{0, 0}, {1, 1}},
        {{4, 1}, {2, 2}},
        {{5, 6}, {3, 3}}
    };
    int height = mapState.size();      // 地圖實際行數
    int width  = mapState[0].size();   // 地圖實際列數
    for (int i = 0; i < MapHeight; i++) {
        for (int j = 0; j < MapWidth; j++) {
            const int num = mapData[i * MapWidth + j];
            if (num == 0) {
                mapState[i][j] = TILE_WATER;
            } else if (num == 1) {
                mapState[i][j] = TILE_GRASS;
            } else if (num == 2) {
                mapState[i][j] = TILE_ROCK;
            } else if (num == 3) {
                mapState[i][j] = TILE_BRIDGE;
            } else if (num == 4) {
                mapState[i][j] = TILE_HOME;
                if (!homeset) {
                    homeposi = i;
                    homeposj = j;
                    homeset = 1;
                }
            }
        }
    }
    for (int i = 0; i < MapHeight; i++) {
        for (int j = 0; j < MapWidth; j++) {
            const int num = mapData[i * MapWidth + j];
            if (num == 0) {
                TileMapGroup->AddNewObject(new Engine::Image("play/water.png", j * BlockSize, i * BlockSize, BlockSize, BlockSize));
            }
            if (num == 1 || num == 2 || num == 4) {
                int beyondwater = 0, belowwater = 0, rightwater = 0, leftwater = 0;
                TileMapGroup->AddNewObject(new Engine::Image("play/grass.png", j * BlockSize, i * BlockSize, BlockSize, BlockSize));
                if (i + 1 < MapHeight && (mapState[i + 1][j] == TILE_WATER || mapState[i + 1][j] == TILE_BRIDGE)) beyondwater = 1;
                if (i - 1 >= 0 && (mapState[i - 1][j] == TILE_WATER || mapState[i - 1][j] == TILE_BRIDGE)) belowwater = 1;
                if (j - 1 >= 0 && (mapState[i][j - 1] == TILE_WATER || mapState[i][j - 1] == TILE_BRIDGE)) rightwater = 1;
                if (j + 1 < MapWidth && (mapState[i][j + 1] == TILE_WATER || mapState[i][j + 1] == TILE_BRIDGE)) leftwater = 1;
                if (beyondwater) TileMapGroup->AddNewObject(new Engine::Image("play/grassbeyondwater.png", j * BlockSize, i * BlockSize, BlockSize, BlockSize, 0, 0));
                if (belowwater) TileMapGroup->AddNewObject(new Engine::Image("play/grassbelowwater.png", j * BlockSize, i * BlockSize, BlockSize, BlockSize, 0, 0));
                if (rightwater) TileMapGroup->AddNewObject(new Engine::Image("play/grassrightwater.png", j * BlockSize, i * BlockSize, BlockSize, BlockSize, 0, 0));
                if (leftwater) TileMapGroup->AddNewObject(new Engine::Image("play/grassleftwater.png", j * BlockSize, i * BlockSize, BlockSize, BlockSize, 0, 0));
                if (belowwater && rightwater) TileMapGroup->AddNewObject(new Engine::Image("play/grassbelowrightwater.png", j * BlockSize, i * BlockSize, BlockSize, BlockSize, 0, 0));
                if (belowwater && leftwater) TileMapGroup->AddNewObject(new Engine::Image("play/grassbelowleftwater.png", j * BlockSize, i * BlockSize, BlockSize, BlockSize, 0, 0));
                if (beyondwater && rightwater) TileMapGroup->AddNewObject(new Engine::Image("play/grassbeyondrightwater.png", j * BlockSize, i * BlockSize, BlockSize, BlockSize, 0, 0));
                if (beyondwater && leftwater) TileMapGroup->AddNewObject(new Engine::Image("play/grassbeyondleftwater.png", j * BlockSize, i * BlockSize, BlockSize, BlockSize, 0, 0));
            }
            if (num == 2) {
                int beyondwater = 0, belowwater = 0, rightwater = 0, leftwater = 0;
                TileMapGroup->AddNewObject(new Engine::Image("play/grass.png", j * BlockSize, i * BlockSize, BlockSize, BlockSize));
                if (i + 1 < MapHeight && (mapState[i + 1][j] == TILE_WATER || mapState[i + 1][j] == TILE_BRIDGE)) beyondwater = 1;
                if (i - 1 >= 0 && (mapState[i - 1][j] == TILE_WATER || mapState[i - 1][j] == TILE_BRIDGE)) belowwater = 1;
                if (j - 1 >= 0 && (mapState[i][j - 1] == TILE_WATER || mapState[i][j - 1] == TILE_BRIDGE)) rightwater = 1;
                if (j + 1 < MapWidth && (mapState[i][j + 1] == TILE_WATER || mapState[i][j + 1] == TILE_BRIDGE)) leftwater = 1;
                if (beyondwater) TileMapGroup->AddNewObject(new Engine::Image("play/grassbeyondwater.png", j * BlockSize, i * BlockSize, BlockSize, BlockSize, 0, 0));
                if (belowwater) TileMapGroup->AddNewObject(new Engine::Image("play/grassbelowwater.png", j * BlockSize, i * BlockSize, BlockSize, BlockSize, 0, 0));
                if (rightwater) TileMapGroup->AddNewObject(new Engine::Image("play/grassrightwater.png", j * BlockSize, i * BlockSize, BlockSize, BlockSize, 0, 0));
                if (leftwater) TileMapGroup->AddNewObject(new Engine::Image("play/grassleftwater.png", j * BlockSize, i * BlockSize, BlockSize, BlockSize, 0, 0));
                if (belowwater && rightwater) TileMapGroup->AddNewObject(new Engine::Image("play/grassbelowrightwater.png", j * BlockSize, i * BlockSize, BlockSize, BlockSize, 0, 0));
                if (belowwater && leftwater) TileMapGroup->AddNewObject(new Engine::Image("play/grassbelowleftwater.png", j * BlockSize, i * BlockSize, BlockSize, BlockSize, 0, 0));
                if (beyondwater && rightwater) TileMapGroup->AddNewObject(new Engine::Image("play/grassbeyondrightwater.png", j * BlockSize, i * BlockSize, BlockSize, BlockSize, 0, 0));
                if (beyondwater && leftwater) TileMapGroup->AddNewObject(new Engine::Image("play/grassbeyondleftwater.png", j * BlockSize, i * BlockSize, BlockSize, BlockSize, 0, 0));
                TileMapGroup->AddNewObject(new Engine::Image("play/rock.png", j * BlockSize, i * BlockSize, BlockSize, BlockSize));
            }
            if (num == 3) {
                TileMapGroup->AddNewObject(new Engine::Image("play/water.png", j * BlockSize, i * BlockSize, BlockSize, BlockSize));
                TileMapGroup->AddNewObject(new Engine::Image("play/stonebridge.png", j * BlockSize, i * BlockSize, BlockSize, BlockSize));
            }
        }
    }
    if (homeset) TileMapGroup->AddNewObject(new Engine::Image("play/home.png", homeposj * BlockSize, homeposi * BlockSize, 4 * BlockSize, 4 * BlockSize));
    drawedgebush();
}

void PlayScene::ReadHomeMap() {
    std::string filename;
    //
    std::string whichfile = std::string("../Resource/whichscene.txt");
    std::ifstream f(whichfile);
    char scenenum;
    f >> scenenum;
    //
    if(scenenum=='0') filename = std::string("Resource/map") + std::to_string(MapId) + ".txt";
    else if(scenenum=='1') filename = std::string("Resource/homemap.txt");
    // Read map file.
    char c;
    std::vector<int> mapData;
    std::ifstream fin(filename);
    while (fin >> c) {
        switch (c) {
            case '0': mapData.push_back(0); break;
            case '1': mapData.push_back(1); break;
            case '2': mapData.push_back(2); break;
            case '3': mapData.push_back(3); break;
            case '4': mapData.push_back(4); break;
            case '5': mapData.push_back(5); break;
            case '6': mapData.push_back(6); break;
            case '7': mapData.push_back(7); break;
            case '8': mapData.push_back(8); break;
            case '9': mapData.push_back(9); break;
            case '\n':
            case '\r':
                if (static_cast<int>(mapData.size()) / MapWidth != 0)
                    throw std::ios_base::failure("Map data is corrupted.4");
                break;
            default: throw std::ios_base::failure("Map data is corrupted.5");
        }
    }
    fin.close();
    if (static_cast<int>(mapData.size()) != MapWidth * MapHeight)
        throw std::ios_base::failure("Map data is corrupted.6");
    mapState = std::vector<std::vector<TileType>>(MapHeight, std::vector<TileType>(MapWidth));
    std::string spriteSheetFile = "map/Overworld.png";
    static const std::pair<std::pair<int, int>, std::pair<int, int>> item_bias[10] = {
        {{3, 3}, {1, 1}},
        {{0, 0}, {1, 1}},
        {{4, 1}, {2, 2}},
        {{5, 6}, {3, 3}}
    };
    for (int i = 0; i < MapHeight; i++) {
        for (int j = 0; j < MapWidth; j++) {
            const int num = mapData[i * MapWidth + j];
            if (num == 0) {
                mapState[i][j] = TILE_GRASS;
            } else if (num == 1 || num==9 ) {
                mapState[i][j] = TILE_GRASS;
            } else if (num == 2 || num==7) {
                mapState[i][j] = TILE_ROCK;
            } else if (num == 3) {
                mapState[i][j] = TILE_BRIDGE;
            } else if (num == 4) {
                mapState[i][j] = TILE_HOME;
                if (!homeset) {
                    homeposi = i;
                    homeposj = j;
                    homeset = 1;
                }
            } else if (num == 5) {
                mapState[i][j] = TILE_STORE;
                if (!storeset) {
                    storeposi = i;
                    storeposj = j;
                    storeset = 1;
                }
            }
            else if (num == 6) {
                mapState[i][j] = TILE_FOUNTAIN;
                if (!fountainset) {
                    fountainposi = i;
                    fountainposj = j;
                    fountainset = 1;
                }
            }else if (num == 8) {
                mapState[i][j] = TILE_BOSSROOM;
                if (!bossroomset) {
                    bossroomposi = i;
                    bossroomposj = j;
                    bossroomset = 1;
                }
            }
        }
    }
    for (int i = 0; i < MapHeight; i++) {
        for (int j = 0; j < MapWidth; j++) {
            const int num = mapData[i * MapWidth + j];
            if (num == 0) {
                TileMapGroup->AddNewObject(new Engine::Image("play/grass.png", j * BlockSize, i * BlockSize, BlockSize, BlockSize));
            }
            if (num == 1 || num == 4 || num == 5) {
                TileMapGroup->AddNewObject(new Engine::Image("play/floor.png", j * BlockSize, i * BlockSize, BlockSize, BlockSize));
            }
            if (num == 2) {
                TileMapGroup->AddNewObject(new Engine::Image("play/grass.png", j * BlockSize, i * BlockSize, BlockSize, BlockSize));
                TileMapGroup->AddNewObject(new Engine::Image("play/rock.png", j * BlockSize, i * BlockSize, BlockSize, BlockSize));
            }
            if (num == 3) {
                TileMapGroup->AddNewObject(new Engine::Image("play/water.png", j * BlockSize, i * BlockSize, BlockSize, BlockSize));
                TileMapGroup->AddNewObject(new Engine::Image("play/stonebridge.png", j * BlockSize, i * BlockSize, BlockSize, BlockSize));
            }if (num == 6) {
                TileMapGroup->AddNewObject(new Engine::Image("play/grass.png", j * BlockSize, i * BlockSize, BlockSize, BlockSize));
                //TileMapGroup->AddNewObject(new Engine::Image("play/fountain.png", (j-3) * BlockSize, (i-3) * BlockSize, 4*BlockSize, 4*BlockSize));
            }if (num == 7) {
                TileMapGroup->AddNewObject(new Engine::Image("play/floor.png", j * BlockSize, i * BlockSize, BlockSize, BlockSize));
                TileMapGroup->AddNewObject(new Engine::Image("play/plant.png", j * BlockSize, (i-1) * BlockSize, BlockSize, 2*BlockSize));
            }if (num == 8) {
                TileMapGroup->AddNewObject(new Engine::Image("play/floor.png", j * BlockSize, i * BlockSize, BlockSize, BlockSize));
            }if (num == 9) {
                TileMapGroup->AddNewObject(new Engine::Image("play/floor.png", j * BlockSize, i * BlockSize, BlockSize, BlockSize));
                TileMapGroup->AddNewObject(new Engine::Image("play/skull.png", j * BlockSize, i * BlockSize, BlockSize, BlockSize));
            }
        }
    }
    if (homeset && scenenum!='1') TileMapGroup->AddNewObject(new Engine::Image("play/home.png", homeposj * BlockSize, homeposi * BlockSize, 4 * BlockSize, 4 * BlockSize));
    if (homeset && scenenum =='1') TileMapGroup->AddNewObject(new Engine::Image("play/battlefield.png", homeposj * BlockSize, homeposi * BlockSize, 4 * BlockSize, 4 * BlockSize));
    if (storeset) TileMapGroup->AddNewObject(new Engine::Image("play/store.png", storeposj * BlockSize, storeposi * BlockSize, 4 * BlockSize, 4 * BlockSize));
    if (bossroomset) TileMapGroup->AddNewObject(new Engine::Image("play/bossroom.png", bossroomposj * BlockSize, bossroomposi * BlockSize, 4.9 * BlockSize, 4 * BlockSize));
    if (fountainset) TileMapGroup->AddNewObject(new Engine::Image("play/fountain.png", fountainposj * BlockSize, fountainposi * BlockSize, 4 * BlockSize, 4 * BlockSize));
    drawedgebush();
}

void PlayScene::drawedgebush(){
    for (int layer = 0; layer < 1; layer++) {
        // Vertical edges (left and right)
        for (int i =   -layer; i < MapHeight + layer; i++) {
            int j = -1 - layer; // Left edge
            TileMapGroup->AddNewObject(new Engine::Image("play/grass.png", j * BlockSize, i * BlockSize, BlockSize, BlockSize));
            TileMapGroup->AddNewObject(new Engine::Image("play/verti_bush.png", j * BlockSize, i * BlockSize, BlockSize, BlockSize));
            j = MapWidth + layer; // Right edge
            TileMapGroup->AddNewObject(new Engine::Image("play/grass.png", j * BlockSize, i * BlockSize, BlockSize, BlockSize));
            TileMapGroup->AddNewObject(new Engine::Image("play/verti_bush.png", j * BlockSize, i * BlockSize, BlockSize, BlockSize));
        }
        // Horizontal edges (top and bottom)
        for (int j = -layer; j < MapWidth + layer; j++) {
            int i = -1 - layer; // Top edge
            TileMapGroup->AddNewObject(new Engine::Image("play/grass.png", j * BlockSize, i * BlockSize, BlockSize, BlockSize));
            TileMapGroup->AddNewObject(new Engine::Image("play/hori_bush.png", j * BlockSize, i * BlockSize, BlockSize, BlockSize));
            i = MapHeight + layer; // Bottom edge
            TileMapGroup->AddNewObject(new Engine::Image("play/grass.png", j * BlockSize, i * BlockSize, BlockSize, BlockSize));
            TileMapGroup->AddNewObject(new Engine::Image("play/hori_bush.png", j * BlockSize, i * BlockSize, BlockSize, BlockSize));
        }
        // Corners
        TileMapGroup->AddNewObject(new Engine::Image("play/grass.png", (-1 - layer) * BlockSize, (-1 - layer) * BlockSize, BlockSize, BlockSize));
        TileMapGroup->AddNewObject(new Engine::Image("play/upleft_bush.png", (-1 - layer) * BlockSize, (-1 - layer) * BlockSize, BlockSize, BlockSize));
        TileMapGroup->AddNewObject(new Engine::Image("play/grass.png", (MapWidth + layer) * BlockSize, (-1 - layer) * BlockSize, BlockSize, BlockSize));
        TileMapGroup->AddNewObject(new Engine::Image("play/upright_bush.png", (MapWidth + layer) * BlockSize, (-1 - layer) * BlockSize, BlockSize, BlockSize));
        TileMapGroup->AddNewObject(new Engine::Image("play/grass.png", (-1 - layer) * BlockSize, (MapHeight + layer) * BlockSize, BlockSize, BlockSize));
        TileMapGroup->AddNewObject(new Engine::Image("play/downleft_bush.png", (-1 - layer) * BlockSize, (MapHeight + layer) * BlockSize, BlockSize, BlockSize));
        TileMapGroup->AddNewObject(new Engine::Image("play/grass.png", (MapWidth + layer) * BlockSize, (MapHeight + layer) * BlockSize, BlockSize, BlockSize));
        TileMapGroup->AddNewObject(new Engine::Image("play/downright_bush.png", (MapWidth + layer) * BlockSize, (MapHeight + layer) * BlockSize, BlockSize, BlockSize));
    }


    // Add five layers of edge forest
    for (int layer = 1; layer < 15; layer++) {
        // Vertical edges (left and right)
        for (int i = -layer; i < MapHeight + layer; i++) {
            int j = -1 - layer; // Left edge
            TileMapGroup->AddNewObject(new Engine::Image("play/grass.png", j * BlockSize, i * BlockSize, BlockSize, BlockSize));
            TileMapGroup->AddNewObject(new Engine::Image("play/forest.png", j * BlockSize, i * BlockSize, BlockSize, BlockSize));
            j = MapWidth + layer; // Right edge
            TileMapGroup->AddNewObject(new Engine::Image("play/grass.png", j * BlockSize, i * BlockSize, BlockSize, BlockSize));
            TileMapGroup->AddNewObject(new Engine::Image("play/forest.png", j * BlockSize, i * BlockSize, BlockSize, BlockSize));
        }
        // Horizontal edges (top and bottom)
        for (int j = -layer; j < MapWidth + layer; j++) {
            int i = -1 - layer; // Top edge
            TileMapGroup->AddNewObject(new Engine::Image("play/grass.png", j * BlockSize, i * BlockSize, BlockSize, BlockSize));
            TileMapGroup->AddNewObject(new Engine::Image("play/forest.png", j * BlockSize, i * BlockSize, BlockSize, BlockSize));
            i = MapHeight + layer; // Bottom edge
            TileMapGroup->AddNewObject(new Engine::Image("play/grass.png", j * BlockSize, i * BlockSize, BlockSize, BlockSize));
            TileMapGroup->AddNewObject(new Engine::Image("play/forest.png", j * BlockSize, i * BlockSize, BlockSize, BlockSize));
        }
        // Corners
        TileMapGroup->AddNewObject(new Engine::Image("play/grass.png", (-1 - layer) * BlockSize, (-1 - layer) * BlockSize, BlockSize, BlockSize));
        TileMapGroup->AddNewObject(new Engine::Image("play/forest.png", (-1 - layer) * BlockSize, (-1 - layer) * BlockSize, BlockSize, BlockSize));
        TileMapGroup->AddNewObject(new Engine::Image("play/grass.png", (MapWidth + layer) * BlockSize, (-1 - layer) * BlockSize, BlockSize, BlockSize));
        TileMapGroup->AddNewObject(new Engine::Image("play/forest.png", (MapWidth + layer) * BlockSize, (-1 - layer) * BlockSize, BlockSize, BlockSize));
        TileMapGroup->AddNewObject(new Engine::Image("play/grass.png", (-1 - layer) * BlockSize, (MapHeight + layer) * BlockSize, BlockSize, BlockSize));
        TileMapGroup->AddNewObject(new Engine::Image("play/forest.png", (-1 - layer) * BlockSize, (MapHeight + layer) * BlockSize, BlockSize, BlockSize));
        TileMapGroup->AddNewObject(new Engine::Image("play/grass.png", (MapWidth + layer) * BlockSize, (MapHeight + layer) * BlockSize, BlockSize, BlockSize));
        TileMapGroup->AddNewObject(new Engine::Image("play/forest.png", (MapWidth + layer) * BlockSize, (MapHeight + layer) * BlockSize, BlockSize, BlockSize));
    }

}
/*
void PlayScene::drawedgebush(){
    // Add five layers of edge bushes
    for (int layer = 0; layer < 15; layer++) {
        // Vertical edges (left and right)
        for (int i = -layer; i < MapHeight + layer; i++) {
            int j = -1 - layer; // Left edge
            TileMapGroup->AddNewObject(new Engine::Image("play/grass.png", j * BlockSize, i * BlockSize, BlockSize, BlockSize));
            TileMapGroup->AddNewObject(new Engine::Image("play/verti_bush.png", j * BlockSize, i * BlockSize, BlockSize, BlockSize));
            j = MapWidth + layer; // Right edge
            TileMapGroup->AddNewObject(new Engine::Image("play/grass.png", j * BlockSize, i * BlockSize, BlockSize, BlockSize));
            TileMapGroup->AddNewObject(new Engine::Image("play/verti_bush.png", j * BlockSize, i * BlockSize, BlockSize, BlockSize));
        }
        // Horizontal edges (top and bottom)
        for (int j = -layer; j < MapWidth + layer; j++) {
            int i = -1 - layer; // Top edge
            TileMapGroup->AddNewObject(new Engine::Image("play/grass.png", j * BlockSize, i * BlockSize, BlockSize, BlockSize));
            TileMapGroup->AddNewObject(new Engine::Image("play/hori_bush.png", j * BlockSize, i * BlockSize, BlockSize, BlockSize));
            i = MapHeight + layer; // Bottom edge
            TileMapGroup->AddNewObject(new Engine::Image("play/grass.png", j * BlockSize, i * BlockSize, BlockSize, BlockSize));
            TileMapGroup->AddNewObject(new Engine::Image("play/hori_bush.png", j * BlockSize, i * BlockSize, BlockSize, BlockSize));
        }
        // Corners
        TileMapGroup->AddNewObject(new Engine::Image("play/grass.png", (-1 - layer) * BlockSize, (-1 - layer) * BlockSize, BlockSize, BlockSize));
        TileMapGroup->AddNewObject(new Engine::Image("play/upleft_bush.png", (-1 - layer) * BlockSize, (-1 - layer) * BlockSize, BlockSize, BlockSize));
        TileMapGroup->AddNewObject(new Engine::Image("play/grass.png", (MapWidth + layer) * BlockSize, (-1 - layer) * BlockSize, BlockSize, BlockSize));
        TileMapGroup->AddNewObject(new Engine::Image("play/upright_bush.png", (MapWidth + layer) * BlockSize, (-1 - layer) * BlockSize, BlockSize, BlockSize));
        TileMapGroup->AddNewObject(new Engine::Image("play/grass.png", (-1 - layer) * BlockSize, (MapHeight + layer) * BlockSize, BlockSize, BlockSize));
        TileMapGroup->AddNewObject(new Engine::Image("play/downleft_bush.png", (-1 - layer) * BlockSize, (MapHeight + layer) * BlockSize, BlockSize, BlockSize));
        TileMapGroup->AddNewObject(new Engine::Image("play/grass.png", (MapWidth + layer) * BlockSize, (MapHeight + layer) * BlockSize, BlockSize, BlockSize));
        TileMapGroup->AddNewObject(new Engine::Image("play/downright_bush.png", (MapWidth + layer) * BlockSize, (MapHeight + layer) * BlockSize, BlockSize, BlockSize));
    }
}*/
void PlayScene::ReadEnemyWave() {
    std::string filename = std::string("Resource/enemy") + std::to_string(MapId) + ".txt";
    float type, wait, repeat;
    enemyWaveData.clear();
    std::ifstream fin(filename);
    while (fin >> type && fin >> wait && fin >> repeat) {
        for (int i = 0; i < repeat; i++)
            enemyWaveData.emplace_back(type, wait);
    }
    fin.close();
}

void PlayScene::ConstructUI() {

    UIGroup->AddNewObject(new ColoredRectangle(0, 0, 200, 220, al_map_rgba(255, 255, 255, 70),2.0f, al_map_rgba(255, 255, 255, 1)));


    UIGroup->AddNewObject(new Engine::Label(std::string("Stage ") + std::to_string(MapId), "pirulen.ttf", 32, 12, 0));
    UIGroup->AddNewObject(SkillWarn = new Engine::Label(std::string("") , "pirulen.ttf", 19, Engine::GameEngine::GetInstance().GetScreenSize().x / 2, Engine::GameEngine::GetInstance().GetScreenSize().y - 17, 255, 10, 10, 220, 0.5f, 0.5f));

    UIGroup->AddNewObject(new Engine::SpriteFixed("play/coin.png", 22, 62, 34, 34, 0.5, 0.5));
    UIGroup->AddNewObject(UIMoney = new Engine::Label(std::to_string(money), "pirulen.ttf", 24, 46, 48));
    /*if(gohomekey==1)*/ UIGroup->AddNewObject(UIHome = new Engine::Label(std::string(""), "pirulen.ttf", 24,  600, 600));
    UIGroup->AddNewObject(UIStore = new Engine::Label(std::string(""), "pirulen.ttf", 24,  600, 600));
    UIGroup->AddNewObject(UIBossroom = new Engine::Label(std::string(""), "pirulen.ttf", 24,  600, 600));
    UIGroup->AddNewObject(UIFountain = new Engine::Label(std::string(""), "pirulen.ttf", 24,  500, 600));
    UIGroup->AddNewObject(UIShit = new Engine::Label(std::string(""), "pirulen.ttf", 24,  500, 650));

    //// new
    UIGroup->AddNewObject(player_exp_l = new Engine::Label(std::string("EXP ") + std::to_string((int)player_exp) + "/" + std::to_string((int)level_req.front()), "pirulen.ttf", 24, 12, 130));
    UIGroup->AddNewObject(player_level_l = new Engine::Label(std::string("Level ") + std::to_string((int)player_level) + "/8", "pirulen.ttf", 24, 12, 155));
    int w = Engine::GameEngine::GetInstance().GetScreenSize().x;
    int h = Engine::GameEngine::GetInstance().GetScreenSize().y;
    int shift = 135 + 25;
    dangerIndicator = new Engine::Sprite("play/benjamin.png", w - shift, h - shift);
    dangerIndicator->Tint.a = 0;
    UIGroup->AddNewObject(dangerIndicator);

}
void PlayScene::buff_adder(int state) {
}
void PlayScene::UIBtnClicked(int id) {
}
bool PlayScene::CheckSpaceValid(int x, int y) {
    if (x < 0 || x >= MapWidth || y < 0 || y >= MapHeight)
        return false;
    if (mapState[y][x] == TILE_GRASS && mapState[y][x] != TILE_OCCUPIED)
        return true;
    return false;
}
std::vector<std::vector<int>> PlayScene::CalculateBFSDistance(Engine::Point start) {
    std::vector<std::vector<int>> map(MapHeight, std::vector<int>(MapWidth, -1));
    std::queue<Engine::Point> que;
    if (start.x < 0 || start.x >= MapWidth || start.y < 0 || start.y >= MapHeight || mapState[start.y][start.x] == TILE_OCCUPIED)
        return map;
    que.push(start);
    map[start.y][start.x] = 0;
    while (!que.empty()) {
        Engine::Point p = que.front();
        que.pop();
        const int dx[4] = {1, -1, 0, 0};
        const int dy[4] = {0, 0, 1, -1};
        for (int i = 0; i < 4; i++) {
            int nx = p.x + dx[i];
            int ny = p.y + dy[i];
            if (nx >= 0 && nx < MapWidth && ny >= 0 && ny < MapHeight &&
                (mapState[ny][nx] == TILE_GRASS || mapState[ny][nx] == TILE_BRIDGE) &&
                map[ny][nx] == -1) {
                map[ny][nx] = map[p.y][p.x] + 1;
                que.push(Engine::Point(nx, ny));
            }
        }
    }
    return map;
}
// Engine::Point PlayScene::GetValidSpawnPoint() {
//     std::vector<Engine::Point> validPoints;
//     for (int y = 0; y < MapHeight; y++) {
//         for (int x = 0; x < MapWidth; x++) {
//             if ((x > 0 && y > 0) &&
//                 ((mapState[y][x] == TILE_GRASS || mapState[y][x] == TILE_BRIDGE )&& (mapState[y][x] != TILE_WATER))) {
//                 validPoints.emplace_back(x, y);
//             }
//         }
//     }
//     if (validPoints.empty()) {
//         return Engine::Point(BlockSize / 2, BlockSize / 2); // 預設也要符合大於 0 的規則
//     }
//     std::random_device rd;
//     std::mt19937 gen(rd());
//     std::uniform_int_distribution<> dis(0, validPoints.size() - 1);
//     Engine::Point gridPos = validPoints[dis(gen)];

//     return Engine::Point(gridPos.x * BlockSize + BlockSize / 2, gridPos.y * BlockSize + BlockSize / 2);
// }

// 修正
Engine::Point PlayScene::GetValidSpawnPoint() {
    std::vector<Engine::Point> validPoints;
    for (int y = 1; y < MapHeight - 1; y++) {
        for (int x = 1; x < MapWidth - 1; x++) {
            // 只要是 grass 或 bridge 就當合法點
            if (mapState[y][x] == TILE_GRASS || mapState[y][x] == TILE_BRIDGE) {
                validPoints.emplace_back(x, y);
            }
        }
    }

    // 如果沒找到合法點，就回傳地圖中心
    if (validPoints.empty()) {
        return Engine::Point(MapWidth * BlockSize / 2, MapHeight * BlockSize / 2);
    }

    // 隨機挑一個格子位置
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dis(0, validPoints.size() - 1);
    Engine::Point gridPos = validPoints[dis(gen)];

    // 回傳該格子的中心像素座標
    return Engine::Point(
        gridPos.x * BlockSize + BlockSize / 2,
        gridPos.y * BlockSize + BlockSize / 2
    );
}


void PlayScene::SpawnCoin(float x, float y, int value) {
    GroundEffectGroup->AddNewObject(new Coin(this, x, y, value));
}
bool PlayScene::tile_crossable(int t) {
    if(t==0||t==2||t==5||t==6||t==8)return 0;
    return 1;
}
void PlayScene::drawskillicon(int x,int y) {
    GroundEffectGroup->AddNewObject(new DirtyEffect("play/noparking.png", 1000, x, y));
}

