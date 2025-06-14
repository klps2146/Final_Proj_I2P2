#ifndef SIMPLE_BOMB_HPP
#define SIMPLE_BOMB_HPP
#include "Bomb.hpp"

class SimpleBomb : public Bomb {
public:
    SimpleBomb(std::string img, float x, float y, float radius, float damage, float delay, Engine::Point velocity = Engine::Point(0, 0));
    void Explode() override;
};
#endif // SIMPLE_BOMB_HPP