// #ifndef WEAPONMANAGER_HPP
// #define WEAPONMANAGER_HPP
// #include "Weapon.hpp"
// #include <vector>
// #include <memory>

// namespace Engine {
//     class WeaponManager {
//     private:
//         std::vector<std::unique_ptr<Weapon>> weapons;
//         int currentWeaponIndex = -1;
//     public:
//         void AddWeapon(std::unique_ptr<Weapon> weapon);
//         void SwitchWeapon(int index);
//         void Update(float deltaTime);
//         void Draw() const;
//         void OnMouseMove(int mx, int my);
//         void OnMouseDown(int button, int mx, int my);
//         void OnMouseUp(int button, int mx, int my);
//         void SetCharacterPosition(Point pos);
//     };
// }
// #endif
#ifndef WEAPONMANAGER_HPP
#define WEAPONMANAGER_HPP
#include "Weapon.hpp"
#include <vector>
#include <memory>

namespace Engine {
    class WeaponManager {
    private:
        std::vector<std::unique_ptr<Weapon>> weapons;
        int currentWeaponIndex = -1;
    public:
        void AddWeapon(std::unique_ptr<Weapon> weapon);
        void SwitchWeapon(int index);
        void Update(float deltaTime);
        void Draw() const;
        void OnMouseMove(int mx, int my);
        void OnMouseDown(int button, int mx, int my);
        void OnMouseUp(int button, int mx, int my);
        void SetCharacterPosition(Point pos);
        void DrawWeaponBar(const Engine::Point& cameraPos, const Engine::Point& screenSize); // 新增方法
    };
}
#endif