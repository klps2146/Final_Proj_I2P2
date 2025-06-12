#pragma once
#include <vector>
#include "Engine/Point.hpp"
#include "Engine/Group.hpp"
class PlayScene;
namespace Engine {
    class Group;
    class Image;
    class Label;
    class Sprite;
}   // namespace Engine
class Store  : public Engine::IObject{
public:
    static const int SlotAmount = 4; // 商店槽位數量
    //std::vector<StoreItem*> items;
    int selectedIndex = 0;
    Engine::Group *UIGroup;
    Store();
    void Update(float deltaTime);
    void Draw(const Engine::Point& cameraPos, const Engine::Point& screenSize);
    void SelectSlot(int index);
    void OnKeyDown(int keyCode);
    //StoreItem* GetSelectedItem() const;
    PlayScene *getPlayScene();
};
/*#pragma once
#include <vector>
#include "Engine/Point.hpp"

class SkillBase;

class ItemBar {
public:
    static const int SlotAmount = 5;
    std::vector<SkillBase*> slots;
    int selectedIndex = 0;

    ItemBar();
    void Update(float deltaTime);
    void Draw(const Engine::Point& cameraPos, const Engine::Point& screenSize);
    void SelectSlot(int index);
    void OnKeyDown(int keyCode);
    SkillBase* GetSelectedSkill() const;
};
*/