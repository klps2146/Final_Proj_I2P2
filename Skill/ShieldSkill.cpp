#include "ShieldSkill.hpp"
#include "Scene/PlayScene.hpp"
#include "Character/Character.hpp"
#include "UI/Animation/Plane.hpp"

ShieldSkill::ShieldSkill()
    : SkillBase("Dash", "skill/shield2.png", 0, 0, 70.0f, 150.0) {
    Unlock();
    upgradeExpenseMoney.push(250); // ut 2
    upgradeExpenseMoney.push(400); // ut 3
    upgradeExpenseMoney.push(850); // ut 4
    upgradeExpenseMoney.push(1400);
}

void ShieldSkill::SkillAnimation() {
    PlayScene* scene = getPlayScene();
    scene->character->changeShield(1000 + level*200);
}
