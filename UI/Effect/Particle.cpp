// Particle.cpp
#include "Particle.hpp"

Particle::Particle(const Engine::Point& pos, const Engine::Point& vel, float life, ALLEGRO_COLOR color)
    : position(pos), velocity(vel), life(life), color(color) {}

void Particle::Update(float deltaTime) {
    position.x += velocity.x * deltaTime;
    position.y += velocity.y * deltaTime;
    
    life -= deltaTime;
}
#include <iostream>
void Particle::Draw(const Engine::Point& cameraPos) const {
    if (life > 0) {
        float drawX = position.x - cameraPos.x;
        float drawY = position.y - cameraPos.y;
        // std::cout << "Draw particle (" << drawX << ", " << drawY << ")" << std::endl;
        al_draw_filled_circle(drawX, drawY, 15, color);
    }
}

bool Particle::IsAlive() const {
    return life > 0;
}
