#pragma once

#include <glm/glm.hpp>

#include <vector>
#include <array>
#include <string_view>

struct SimulationParams
{
    float particleSize = 1.5f;
    float particleMinSpeed = 10.0f;
    float particleMaxSpeed = 100.0f;
    float particleMinLife = 5.0f;
    float particleMaxLife = 30.0f;
    int emissionRate = 10;
};

struct Particle
{
    glm::vec2 position;
    glm::vec2 velocity;
    glm::vec4 color;
    float life;
};

enum class ParticleModifierType
{
    None,
    Pull,
    Circle
};

constexpr auto modifierNames{ std::to_array<std::string_view>({ "None", "Pull", "Circle" }) };

inline std::string_view toString(ParticleModifierType type)
{
    return modifierNames[static_cast<std::size_t>(type)];
}

class Shader;

class ParticleSystem
{
public:
    ParticleSystem();

    void update(float dt);

    void emitParticles(int x, int y);

    void applyModifier(int cursorX, int cursorY);
    void pullParticlesTo(int x, int y);
    void circleParticlesAround(int x, int y);

    void setParams(const SimulationParams* params) { params_ = params; }
    void setParticleModifier(ParticleModifierType type) { modifierType_ = type; }

    ParticleModifierType getCurrentModifier() const { return modifierType_; }
    const Particle* getParticleRenderData() const { return particles_.data(); }
    int getParticleCount() const { return static_cast<int>(particles_.size()); }

private:
    void spawnParticleAt(Particle& p, int x, int y);

private:
    std::vector<Particle> particles_{};
    const SimulationParams* params_{};
    ParticleModifierType modifierType_{ ParticleModifierType::None };
    int particleIndex_ = 0;
};