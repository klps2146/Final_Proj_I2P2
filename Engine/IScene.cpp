#include "IScene.hpp"
#include "Engine/GameEngine.hpp"
#include <allegro5/allegro.h>

namespace Engine {

    void IScene::Terminate() {
        Clear();
    }
    void IScene::Draw() const {
        if (!backgroundImagePath.empty()) {
            backgroundImage->Draw();
        }
        else {
            al_clear_to_color(backgroundColor);
        }
        Group::Draw();
    }

    //// 魔改
    void IScene::SetBackgroundColor(ALLEGRO_COLOR color) {
        backgroundColor = color;
        if (!backgroundImagePath.empty()){
            delete backgroundImage;
            backgroundImagePath = "";
        }
    }
    void IScene::SetBackgroundImage(const std::string& path) {
        backgroundImagePath = path;
        int screenW = GameEngine::GetInstance().GetScreenSize().x;
        int screenH = GameEngine::GetInstance().GetScreenSize().y;
        backgroundImage = new Image(backgroundImagePath, 0, 0, screenW, screenH, 0, 0);
    }
}


