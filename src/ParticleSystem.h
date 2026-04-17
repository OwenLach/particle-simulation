#pragma once

#include <glm/glm.hpp>

#include <vector>
#include <array>
#include <string_view>

struct SimulationParams
{
    float particleSize;
    float particleMinSpeed;
    float particleMaxSpeed;
    float particleMinLife;
    float particleMaxLife;
    int emissionRate;
    bool particlesFrozen;
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
    Circle,
    Repel,
};

inline std::string_view toString(ParticleModifierType type)
{
    constexpr static auto modifierNames{ std::to_array<std::string_view>({ "None", "Pull", "Circle", "Repel" }) };
    return modifierNames[static_cast<std::size_t>(type)];
}

class Shader;

class ParticleSystem
{
public:
    ParticleSystem();

    void update(float dt, const glm::ivec2& bounds);
    void emitParticles(int x, int y, float deltaTime);

    void applyModifier(int cursorX, int cursorY);
    void pullParticlesTo(int x, int y);
    void circleParticlesAround(int x, int y);
    void toggleParticleFreeze();
    void clearParticles();
    void repelParticles(int x, int y);

    void setParams(SimulationParams* params) { params_ = params; }
    void setParticleModifier(ParticleModifierType type) { modifierType_ = type; }

    ParticleModifierType getCurrentModifier() const { return modifierType_; }
    const Particle* getParticleRenderData() const { return particles_.data(); }
    int getActiveParticleCount() const { return activeCount_; }

private:
    void spawnParticleAt(Particle& p, int x, int y);

private:
    // probably use std::array<Particle, Settings::maxParticles>
    std::vector<Particle> particles_;
    ParticleModifierType modifierType_;
    SimulationParams* params_{ nullptr };

    int activeCount_ = 0;
};