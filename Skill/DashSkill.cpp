#include "DashSkill.hpp"
#include "Scene/PlayScene.hpp"
#include "Character/Character.hpp"

DashSkill::DashSkill()
    : SkillBase("Dash", "skill/yuan.png", 0, 0, 5.0f) { // 冷卻 5
    Unlock();
}

void DashSkill::SkillAnimation() {
    PlayScene* scene = getPlayScene();
    if (scene && scene->character) {
        int res = level * 2;
        scene->character->speed *= res;
        float dura = (float)level * 3.0f;
        scene->character->setTimer(dura);
    }
}
