#pragma once
#include <vector>
#include "Engine/Point.hpp"

class SkillBase;
class PlayScene;

class ItemBar {
public:
    static const int SlotAmount = 6;
    std::vector<SkillBase*> slots;
    int selectedIndex = 0;

    ItemBar();
    void Update(float deltaTime);
    void Draw(const Engine::Point& cameraPos, const Engine::Point& screenSize);
    void SelectSlot(int index);
    void OnKeyDown(int keyCode);
    SkillBase* GetSelectedSkill() const;
    PlayScene* getPlayScene();
};
