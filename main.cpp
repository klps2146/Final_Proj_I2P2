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
// #include "AI/ChatModel.hpp"

int main(int argc, char **argv) {
    std::wstring model_path = L"Resource/model/gptneox_Opset18.onnx";
    ChatModel model(model_path);

    auto input_names = model.GetInputNames();
    for (size_t i = 0; i < input_names.size(); i++) {
        auto shape = model.GetInputShape(i);
        std::cout << "Input[" << i << "] name: " << input_names[i] << " shape: [ ";
        for (auto d : shape) {
            std::cout << d << " ";
        }
        std::cout << "]" << std::endl;
    }

    int64_t batch_size = 1;
    int64_t seq_len = 128;

    std::unordered_map<std::string, std::vector<int64_t>> inputs_int64;
    std::unordered_map<std::string, std::vector<int64_t>> input_shapes_map;

    // 建立示範輸入資料，這裡全部用 0 和 1 填充
    inputs_int64["input_ids"] = std::vector<int64_t>(batch_size * seq_len, 0);
    input_shapes_map["input_ids"] = {batch_size, seq_len};

    inputs_int64["attention_mask"] = std::vector<int64_t>(batch_size * seq_len, 1);
    input_shapes_map["attention_mask"] = {batch_size, seq_len};

    try {
        auto output = model.Infer(inputs_int64, input_shapes_map);

        std::cout << "Output size: " << output.size() << std::endl;
        std::cout << "First 10 values: ";
        for (size_t i = 0; i < std::min<size_t>(10, output.size()); i++) {
            std::cout << output[i] << " ";
        }
        std::cout << std::endl;
    }
    catch (const std::exception& ex) {
        std::cerr << "Error during inference: " << ex.what() << std::endl;
    }




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

