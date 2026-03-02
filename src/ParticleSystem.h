#pragma once

#include <glm/glm.hpp>

#include <vector>

struct SimulationParams
{
    float particleSize{ 1.5f };
    float particleMinSpeed{ 10.0f };
    float particleMaxSpeed{ 100.0f };
    int emissionRate{ 10 };
};

// Emission Rate: make rate changeable with slider
// Make the color default to change color overtime but let user select color
// Make default speed random but let user change the speed

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

    void pullParticlesTo(int x, int y);

    void setParams(const SimulationParams* params) { params_ = params; }

private:
    void spawnParticleAt(Particle& p, int x, int y);

private:
    const SimulationParams* params_{};
    int particleIndex_ = 0;
    std::vector<Particle> particles_{};
};