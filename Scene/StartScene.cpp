//
// Created by Hsuan on 2024/4/10.
//

#include "StartScene.h"
#include <allegro5/allegro_audio.h>
#include <functional>
#include <iostream>
#include <memory>
#include <string>

#include "Engine/AudioHelper.hpp"
#include "Engine/GameEngine.hpp"
#include "Engine/Point.hpp"
#include "Engine/Resources.hpp"
#include "PlayScene.hpp"
#include "Scene/StartScene.h"
#include "UI/Component/ImageButton.hpp"
#include "UI/Component/Label.hpp"
#include "UI/Component/Slider.hpp"

// TODO HACKATHON-2 (1/3): You can imitate the 2 files: 'StartScene.hpp', 'StartScene.cpp' to implement your SettingsScene.
void StartScene::Initialize() {
    SetBackgroundImage("stage-select/yuanSuang.png");

    int w = Engine::GameEngine::GetInstance().GetScreenSize().x;
    int h = Engine::GameEngine::GetInstance().GetScreenSize().y;
    int halfW = w / 2;
    int halfH = h / 2;
    Engine::ImageButton *btn;

    AddNewObject(new Engine::Label("Genshin Dener", "pirulen.ttf", 120, halfW, halfH / 3 + 50, 10, 10, 11, 255, 0.5, 0.5));

    btn = new Engine::ImageButton("stage-select/dirt.png", "stage-select/floor.png", halfW - 200, halfH / 2 + 200, 400, 100);
    btn->SetOnClickCallback(std::bind(&StartScene::PlayOnClick, this, 1));
    AddNewControlObject(btn);
    AddNewObject(new Engine::Label("Play", "pirulen.ttf", 48, halfW, halfH / 2 + 250, 0, 0, 0, 255, 0.5, 0.5));

    btn = new Engine::ImageButton("stage-select/dirt.png", "stage-select/floor.png", halfW - 200, halfH * 3 / 2 - 50, 400, 100);
    btn->SetOnClickCallback(std::bind(&StartScene::SettingsOnClick, this, 2));
    AddNewControlObject(btn);
    AddNewObject(new Engine::Label("Settings", "pirulen.ttf", 48, halfW, halfH * 3 / 2, 0, 0, 0, 255, 0.5, 0.5));
}
void StartScene::Terminate() {
    IScene::Terminate();
}
void StartScene::PlayOnClick(int stage) {
    Engine::GameEngine::GetInstance().ChangeScene("stage-select");
}
void StartScene::SettingsOnClick(int stage) {
    Engine::GameEngine::GetInstance().ChangeScene("settings");
}

void StartScene::Draw() const {
    IScene::Draw();
}