#include "AreaSkill.hpp"
#include "Scene/PlayScene.hpp"
#include "Character/Character.hpp"
#include <Enemy/Enemy.hpp>

#include <iostream>

AreaSkill::AreaSkill(const std::string& name, const std::string& img, float radius, float maxCD, float duration)
    : SkillBase(name, img, 0, 0, maxCD, 245.0), radius(radius * level), effectDuration(duration), effectTimer(0), showingEffect(false) {
    // Unlock();
    upgradeExpenseMoney.push(650); // unlock
    upgradeExpenseMoney.push(850); // ut 2
    upgradeExpenseMoney.push(1050); // ut 3
    upgradeExpenseMoney.push(1600);
    upgradeExpenseMoney.push(2850);


}

void AreaSkill::SkillAnimation() {
    auto scene = getPlayScene();
    if (!scene) return;

    auto character = scene->character;
    if (!character) return;

    effectPosition = character->Position;
    effectTimer = effectDuration;
    showingEffect = true;

    // particleSystem.Emit(effectPosition, 50, al_map_rgba(0, 0, 0, 255));
}

void AreaSkill::Update(float deltaTime) {
    SkillBase::Update(deltaTime);
    particleSystem.Update(deltaTime);

    if (showingEffect) {
        effectTimer -= deltaTime;
        if (effectTimer <= 0) {
            effectTimer = 0;
            showingEffect = false;
        } else {
            // 檢查所有敵人是否在範圍內
            auto scene = getPlayScene();
            if (scene) {
                for (auto* enemy : scene->EnemyGroup->GetObjects()) {
                    Engine::Point ePos = enemy->Position;
                    float dx = ePos.x - effectPosition.x;
                    float dy = ePos.y - effectPosition.y;
                    float dist = std::sqrt(dx * dx + dy * dy);
                    Enemy* emy = dynamic_cast<Enemy*>(enemy);
                    if (dist < radius) {
                        if (emy)
                        emy->Hit(14, true);
                    }
                }
            }
        }
    }
}

void AreaSkill::Draw() const {
    SkillBase::Draw();

    if (showingEffect) {
        // std::cout << "DRAW AREA\n" ;
        
        int nx = effectPosition.x - getPlayScene()->CameraPos.x;
        int ny = effectPosition.y - getPlayScene()->CameraPos.y;
        //getPlayScene()->drawskillicon(effectPosition.x, effectPosition.y);
        al_draw_filled_circle(nx, ny, radius, al_map_rgba(255, 0, 0, 150));
        al_draw_circle(nx, ny, radius, al_map_rgb(255, 0, 0), 3);
    }

}
