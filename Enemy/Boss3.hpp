#ifndef BOSS3_HPP
#define BOSS3_HPP
#include "BossEnemy.hpp"

class Boss3 : public BossEnemy {
private:
    float shootTimer;  // 計時器，用於控制射線發射間隔
    float rayTimer;    // 計時器，用於追蹤射線存在時間
    bool rayActive;    // 標記射線是否活動
    float healAmount;  // Boss回血量

    void UpdateRay(float deltaTime);  // 更新射線邏輯
    void DrawRay() const;             // 繪製射線

public:
    Boss3(float x, float y);          // 構造函數
    void Update(float deltaTime) override;  // 更新函數
    void Draw() const override;       // 繪製函數
};
#endif // BOSS3_HPP