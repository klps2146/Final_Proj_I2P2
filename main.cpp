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
#include <ctime> // for time()
#include <cstdlib> // for srand()

#include <iostream>
//#include "AI/ChatModel.hpp"

int main(int argc, char **argv) {
    // ChatModel model(L"Resource/model/gpt2-10.onnx");

    // std::string input = "hello world";
    // auto tokens = model.Tokenize(input);

    // std::cout << "Input tokens: ";
    // for (auto t : tokens) std::cout << t << " ";
    // std::cout << std::endl;

    // auto output_tokens = model.Infer(tokens);

    // std::cout << "Output tokens: ";
    // for (auto t : output_tokens) std::cout << t << " ";
    // std::cout << std::endl;

    // std::string output_text = model.Detokenize(output_tokens);
    // std::cout << "Output text: " << output_text << std::endl;


	srand(time(0)); // 初始化隨機數種子
	Engine::LOG::SetConfig(true);
	Engine::GameEngine& game = Engine::GameEngine::GetInstance();

    // OK TODO HACKATHON-2 (2/3): Register Scenes here
	game.AddNewScene("start", new StartScene());
	game.AddNewScene("settings", new SettingsScene());
    game.AddNewScene("stage-select", new StageSelectScene());
    game.AddNewScene("scoreboard-scene", new ScoreboardScene());
	game.AddNewScene("play", new PlayScene());
	game.AddNewScene("lose", new LoseScene());
	game.AddNewScene("win", new WinScene());

    // OK TODO HACKATHON-1 (1/1): Change the start scene
	game.Start("start", 60, 1600, 832);

	return 	0;
}

