#include "ParticleSystemManager.hpp"
#include <cstdlib>


void ParticleSystemManager::Emit(const Engine::Point& pos, int count, ALLEGRO_COLOR color) {
    for (int i = 0; i < count; ++i) {
        float angle = (rand() % 360) * 3.14159f / 180;
        float speed = (rand() % 50) / 10.0f;
        Engine::Point vel = Engine::Point(cos(angle) * speed, sin(angle) * speed);
        particles.emplace_back(pos, vel, 0.5f, color);
    }
}

void ParticleSystemManager::Update(float deltaTime) {
    for (auto& p : particles)
        p.Update(deltaTime);
    particles.erase(std::remove_if(particles.begin(), particles.end(), [](const Particle& p) {
        return !p.IsAlive();
    }), particles.end());
}

void ParticleSystemManager::Draw(const Engine::Point& cameraPos) const {
    for (const auto& p : particles)
        p.Draw(cameraPos);
}