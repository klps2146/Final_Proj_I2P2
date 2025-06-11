#ifndef PARTIClE_HPP
#define PARTIClE_HPP

#include <allegro5/allegro.h>
#include <allegro5/allegro_primitives.h>
#include "Engine/Point.hpp"

class Particle {
public:
    Engine::Point position;
    Engine::Point velocity;
    float life;
    ALLEGRO_COLOR color;

    Particle(const Engine::Point& pos, const Engine::Point& vel, float life, ALLEGRO_COLOR color);
    void Update(float deltaTime);
    void Draw(const Engine::Point& cameraPos) const;
    bool IsAlive() const;
};

#endif