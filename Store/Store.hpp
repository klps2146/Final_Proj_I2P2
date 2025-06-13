#pragma once
#include <vector>
#include "Engine/Point.hpp"
#include "UI/Component/ColoredRectangle.hpp"
#include "UI/Component/Image.hpp"
#include "UI/Component/Label.hpp"

class SkillBase;

class PlayScene;

class StoreItem : public Engine::IObject{
public:
    PlayScene* getPlayScene() const;
    StoreItem(const std::string& imagePath, int price, float x, float y);
    void Update(float deltaTime) override;
    void Draw() const override;
    int GetPrice() const { return price; }
    bool IsClicked(float mouseX, float mouseY) const;
    void SetPosition(float x, float y);
    void SetSize(float w, float h);

    Engine::Image* iconImage ;
    SkillBase* skillHere;
    int price;
};



class Store : public Engine::IObject {
public:
    Store();
    void Update(float deltaTime) override;
    void Draw() const override;
    void OnMouseDown(int button, int mx, int my);
    float slotSize = 80;
    float spacing = 20;
    ColoredRectangle background;
    std::vector<StoreItem*> items;
    PlayScene* getPlayScene()const;
};
