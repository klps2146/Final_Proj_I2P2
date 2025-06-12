#ifndef ENEMYBULLET_HPP
#define ENEMYBULLET_HPP

#include "Engine/Sprite.hpp"
#include "Engine/Point.hpp"

class PlayScene;

namespace Engine {
    class EnemyBullet : public Sprite {
    protected:
        float damage;
        // Protected constructor for derived classes
        EnemyBullet(const std::string& texturePath, float x, float y, float w, float h, float anchorX, float anchorY, float damage, Point velocity);

    public:
        virtual ~EnemyBullet() = default;
        
        // Pure virtual functions for derived classes to implement
        //virtual void OnCollisionWithPlayer(Character* player, PlayScene* scene) = 0;
        virtual void Initialize() = 0;
        
        // Common update logic
        void Update(float deltaTime) override;

        // Getter for damage
        float GetDamage() const { return damage; }
    };
}

#endif // ENEMYBULLET_HPP