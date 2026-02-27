#pragma once

#include <glm/glm.hpp>

#include <vector>

struct Particle
{
    glm::vec2 position;
    glm::vec2 velocity;
    glm::vec4 color;
    float life;
};

class Shader;

class ParticleSystem
{
public:
    ParticleSystem();

    void update(unsigned int vbo, float dt);
    // render should turn into ParticleRenderer class later
    void render(unsigned int vao) const;
    // probably Emitter class later
    void emitParticles(int x, int y);
    void spawnParticleAt(Particle& p, int x, int y);
    void pullParticlesTo(int x, int y);

private:
    int particleIndex_ = 0;
    std::vector<Particle> particles_{};
};