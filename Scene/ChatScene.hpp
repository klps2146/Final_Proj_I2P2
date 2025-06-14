#ifndef CHATSCENE_HPP
#define CHATSCENE_HPP
#include "Engine/IScene.hpp"
#include "UI/Component/Label.hpp"
#include <allegro5/allegro_audio.h>

class ChatScene final : public Engine::IScene {
private:
    float ticks;
    ALLEGRO_SAMPLE_ID bgmId;
    std::string user_name = "";
    Engine::Label *name_label;
    std::string score;
    void save_date();
public:
    explicit ChatScene() = default;
    void Initialize() override;
    void Terminate() override;
    void Update(float deltaTime) override;
    void BackOnClick(int stage);
    void OnKeyDown(int keyCode) override;
};

#endif   // WINSCENE_HPP
