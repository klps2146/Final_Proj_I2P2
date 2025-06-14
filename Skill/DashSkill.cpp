#include "DashSkill.hpp"
#include "Scene/PlayScene.hpp"
#include "Character/Character.hpp"

DashSkill::DashSkill()
    : SkillBase("Dash", "skill/yuan.png", 0, 0, 5.0f, 30.0) { // 冷卻 5
    Unlock();
    upgradeExpenseMoney.push(250); // ut 2
    upgradeExpenseMoney.push(400); // ut 3
    upgradeExpenseMoney.push(850); // ut 4
    upgradeExpenseMoney.push(1400);
}

void DashSkill::SkillAnimation() {
    PlayScene* scene = getPlayScene();
    if (scene && scene->character) {
        int res = 1 + level * 0.7;
        scene->character->speed *= res;
        float dura = (float)level*0.2f +3.0f;
        scene->character->setTimer(dura, res);
    }
}
