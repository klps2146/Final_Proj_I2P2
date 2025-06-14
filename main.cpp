// [main.cpp]
// This is the entry point of your game.
// You can register your scenes here, and start the game.
#include "Engine/GameEngine.hpp"
#include "Engine/LOG.hpp"
#include "Scene/LoseScene.hpp"
#include "Scene/PlayScene.hpp"
#include "Scene/StageSelectScene.hpp"
#include "Scene/WinScene.hpp"
#include "Scene/StartScene.h"
#include "Scene/SettingsScene.hpp"
#include "Scene/ScoreboardScene.hpp"
#include "Scene/ChatScene.hpp"
#include <ctime> // for time()
#include <cstdlib> // for srand()
#include <iostream>

int main(int argc, char **argv) {
	srand(time(0)); // 初始化隨機數種子
	Engine::LOG::SetConfig(true);
	Engine::GameEngine& game = Engine::GameEngine::GetInstance();

    // OK TODO HACKATHON-2 (2/3): Register Scenes here
	game.AddNewScene("start", new StartScene());
	game.AddNewScene("settings", new SettingsScene());
    game.AddNewScene("stage-select", new StageSelectScene());
    // game.AddNewScene("stage-select", new StageSelectScene());
    game.AddNewScene("chat-scene", new ChatScene());
	game.AddNewScene("play", new PlayScene());
	game.AddNewScene("lose", new LoseScene());
	game.AddNewScene("win", new WinScene());

    // OK TODO HACKATHON-1 (1/1): Change the start scene
	game.Start("start", 60, 1600, 832);

	return 	0;
}

