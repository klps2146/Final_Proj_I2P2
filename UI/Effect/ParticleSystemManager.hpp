#ifndef PARTICLESYSTEMMANAGER_HPP
#define PARTICLESYSTEMMANAGER_HPP
#include <vector>
#include "Particle.hpp"

class ParticleSystemManager {
private:
    std::vector<Particle> particles;
public:
    void Emit(const Engine::Point& pos, int count, ALLEGRO_COLOR color);
    void Update(float deltaTime);
    void Draw(const Engine::Point& cameraPos) const;
};

#endif