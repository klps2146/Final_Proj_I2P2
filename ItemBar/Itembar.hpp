#ifndef ITEMBAR_HPP
#define ITEMBAR_HPP

#include <allegro5/allegro.h>
#include "Engine/GameEngine.hpp"
#include "Engine/Collider.hpp"

#include <vector>

class SkillBase;
class PlayScene;

struct SkillSlot {
    SkillBase* skill;
    bool isAvailable = true;
    float cooldown = 0;
    float maxCooldown = 0;
};

class ItemBar {
public:
    static const int SlotAmount = 5;
    std::vector<SkillSlot> slots;
    int selectedIndex = 0;

    ItemBar();

    void Update(float deltaTime);
    void Draw(const Engine::Point& cameraPos, const Engine::Point& screenSize);
    void SelectSlot(int index);
    void OnKeyDown(int keyCode);
};

#endif