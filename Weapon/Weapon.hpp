#ifndef WEAPON_HPP
#define WEAPON_HPP
#include "Engine/Sprite.hpp"
#include "Engine/IControl.hpp"

namespace Engine {
    class Weapon : public Sprite, public IControl {
    public:
        Weapon(std::string img, float x, float y, float w = 0, float h = 0,
               float anchorX = 0.5f, float anchorY = 0.5f);
        virtual void Update(float deltaTime) override = 0;
        virtual void OnMouseMove(int mx, int my) override = 0;
        virtual void OnMouseDown(int button, int mx, int my) override = 0;
        virtual void OnMouseUp(int button, int mx, int my) override = 0;
        virtual void SetCharacterPosition(Point pos) = 0;
        virtual void Draw() const override = 0;
        bool isActive = false;
    };
}
#endif