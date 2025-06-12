#include "SummonDroneSkill.hpp"
#include "Partner/Drone.hpp"
#include "Scene/PlayScene.hpp"
#include "Partner/Drone.hpp"
#include <cmath>
#include <cstdlib>

SummonDroneSkill::SummonDroneSkill(int count, float lifetime, float radius, float cooldown)
    : SkillBase("Summon Drone", "skill/Drone.png", 0, 0, 10.0f, 590.0f),
      droneCount(count), droneLife(lifetime * level), droneRadius(radius), droneCooldown(cooldown) {
    Unlock();
}

void SummonDroneSkill::SkillAnimation() {
    PlayScene* scene = getPlayScene();
    if (!scene) return;

    Engine::Point center = scene->character->Position;

    for (int i = 0; i < droneCount * level; ++i) {
        float angle = 2 * ALLEGRO_PI * i / droneCount;
        float offsetX = cos(angle) * 40 + (rand() % 20 - 10); // 附近隨機生成
        float offsetY = sin(angle) * 40 + (rand() % 20 - 10);
         
        Engine::Point followOffset(30, 30);
        Drone* drone = new Drone(center.x + offsetX, center.y + offsetY, droneRadius, droneCooldown, droneLife, followOffset);
        getPlayScene()->DroneGroup->AddNewObject(drone);
    }

    // scene->ParticleSystemManager->Add("drone_spawn", center.x, center.y);
}
