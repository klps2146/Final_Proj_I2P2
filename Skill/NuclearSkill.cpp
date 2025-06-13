#include "NuclearSkill.hpp"
#include "Scene/PlayScene.hpp"
#include "Character/Character.hpp"
#include "UI/Animation/Plane.hpp"

NuclearSkill::NuclearSkill()
    : SkillBase("Dash", "skill/jing.png", 0, 0, 70.0f, 3900.0) {
    upgradeExpenseMoney.push(3250); // ut 2
    MaxLevel = 1;
}

void NuclearSkill::SkillAnimation() {
    PlayScene* scene = getPlayScene();
    scene->UIGroup->AddNewObject(new Plane);
    scene->character->ChangeHP(-1990);
}
