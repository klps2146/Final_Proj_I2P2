

#ifndef GUN_HPP
#define GUN_HPP
#include "Weapon.hpp"
#include "Engine/IScene.hpp"
#include "Engine/Point.hpp"

namespace Engine {
    class Gun : public Weapon {
    private:
        IScene* scene;
        float fireCooldown;
        float cooldownTimer;
        Point characterPosition;
    public:
        explicit Gun(IScene* scene, Point characterPos);
        void Update(float deltaTime) override;
        void OnMouseMove(int mx, int my) override;
        void OnMouseDown(int button, int mx, int my) override;
        void OnMouseUp(int button, int mx, int my) override;
        void SetCharacterPosition(Point pos) override;
        void Draw() const override;
    };
}
#endif