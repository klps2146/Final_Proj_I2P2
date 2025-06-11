#include "Particle.hpp"

Particle::Particle(const Engine::Point& pos, const Engine::Point& vel, float life, ALLEGRO_COLOR color)
    : position(pos), velocity(vel), life(life), color(color) {}

void Particle::Update(float deltaTime) {
    position.x += velocity.x * deltaTime;
    position.y += velocity.y * deltaTime;
    life -= deltaTime;
}

void Particle::Draw() const {
    if (life > 0)
        al_draw_filled_circle(position.x, position.y, 3, color);
}

bool Particle::IsAlive() const {
    return life > 0;
}
