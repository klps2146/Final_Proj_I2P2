#include <string>

#include "SoldierEnemy.hpp"

// OK TODO HACKATHON-3 (1/3): You can imitate the 2 files: 'SoldierEnemy.hpp', 'SoldierEnemy.cpp' to create a new enemy.
SoldierEnemy::SoldierEnemy(int x, int y) : Enemy("play/newenemy-1.png", x, y, 10, 80, 5, 5) {
    dmg = 6;
}
