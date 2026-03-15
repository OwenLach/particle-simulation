#include "ParticleSystem.h"
#include "Settings.h"
#include "Random.h"

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <cmath>

ParticleSystem::ParticleSystem()
{
    particles_.reserve(Settings::maxParticles);

    for (int i = 0; i < Settings::maxParticles; ++i)
    {
        Particle p{
            .position{ static_cast<float>(Settings::SCR_WIDTH) / 2.0f, static_cast<float>(Settings::SCR_HEIGHT) / 2.0f },
            .velocity{ 0.0f },
            .color{ 1.0f, 1.0f, 1.0f, 0.0f },
            .life{ 0.0f }
        };
        particles_.push_back(p);
    }
}

void ParticleSystem::update(float dt)
{
    const float t = static_cast<float>(glfwGetTime());
    const float r = std::sin(t) / 2.0f + 0.5f;
    const float g = (std::sin(t + 2.0f)) / 2.0f + 0.5f;
    const float b = (std::sin(t + 4.0f)) / 2.0f + 0.5f;

    for (Particle& p : particles_)
    {
        if (p.life <= 0)
        {
            p.color = glm::vec4{ 1.0f, 1.0f, 1.0f, 0.0f };
            continue;
        }

        p.life -= dt;
        p.position += p.velocity * dt;

        if (p.position.x >= static_cast<float>(Settings::SCR_WIDTH) || p.position.x <= 0)
        {
            p.velocity.x *= -1;
        }

        if (p.position.y >= static_cast<float>(Settings::SCR_HEIGHT) || p.position.y <= 0)
        {
            p.velocity.y *= -1;
        }

        p.color = glm::vec4{ r, g, b, 1.0f };
    }
}

void ParticleSystem::emitParticles(int x, int y)
{
    for (int i = 0; i < params_->emissionRate; ++i)
    {
        Particle& p = particles_[particleIndex_];
        particleIndex_ = (particleIndex_ + 1) % Settings::maxParticles;

        if (p.life <= 0)
        {
            spawnParticleAt(p, x, y);
        }
    }
}

void ParticleSystem::applyModifier(int cursorX, int cursorY)
{
    switch (modifierType_)
    {
    case ParticleModifierType::None:
        break;
    case ParticleModifierType::Pull:
        pullParticlesTo(cursorX, cursorY);
        break;
    case ParticleModifierType::Circle:
        circleParticlesAround(cursorX, cursorY);
        break;
    }
}

void ParticleSystem::spawnParticleAt(Particle& p, int x, int y)
{
    glm::vec2 directionVec{
        Random::get(-1.0f, 1.0f),
        Random::get(-1.0f, 1.0f)
    };
    const float speed = Random::get(params_->particleMinSpeed, params_->particleMaxSpeed);

    p.velocity = glm::normalize(directionVec) * speed;
    p.life = Random::get(params_->particleMinLife, params_->particleMaxLife);
    p.position = glm::vec2{ x, y };
    p.color = glm::vec4{ 1.0f, 1.0f, 1.0f, 1.0f };
}

void ParticleSystem::pullParticlesTo(int x, int y)
{
    for (Particle& p : particles_)
    {
        if (p.life <= 0)
            continue;

        glm::vec2 cursorPos{ x, y };
        glm::vec2 dir{ glm::normalize(cursorPos - p.position) };

        p.velocity = dir * glm::length(p.velocity);
    }
}

void ParticleSystem::circleParticlesAround(int x, int y)
{
    glm::vec2 center{ x, y };

    for (Particle& p : particles_)
    {
        if (p.life <= 0)
            continue;

        glm::vec2 normalized{ glm::normalize(p.position - center) };
        glm::vec2 dir{ -normalized.y, normalized.x };

        p.velocity = dir * glm::length(p.velocity);
    }
}