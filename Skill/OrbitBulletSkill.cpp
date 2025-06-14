#include "OrbitBulletSkill.hpp"
#include "Bullet/OrbitingBullet.hpp"
#include "Scene/PlayScene.hpp"
#include "Character/Character.hpp"

OrbitSkill::OrbitSkill(float x, float y)
    : SkillBase("Orbit Shot", "play/visable.png", x, y, 50, 800) {
    // Unlock();
    upgradeExpenseMoney.push(6); // unlock
    upgradeExpenseMoney.push(8); // ut 2
    upgradeExpenseMoney.push(10); // ut 3
    upgradeExpenseMoney.push(10);
    upgradeExpenseMoney.push(20);
}

void OrbitSkill::SkillAnimation() {
    auto* scene = getPlayScene();
    Engine::Point pos = scene->character->Position;

    const int bulletCount = 2 + level * 2;
    float radius = 150;
    float angularSpeed = (1 + level / 10) * ALLEGRO_PI;
    float lifeTime = 30.0 + level * 4;
    float damage = 10 + level * 2;
    

    for (int i = 0; i < bulletCount; ++i) {
        float angle = i * (2 * ALLEGRO_PI / bulletCount);
        scene->BulletGroup->AddNewObject(new OrbitingBullet(pos, angle, radius, angularSpeed, damage, lifeTime));
    }
}
