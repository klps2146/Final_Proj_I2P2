#pragma once
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
