#include "ParticleSystem.h"
#include "Settings.h"
#include "Random.h"

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <cmath>

ParticleSystem::ParticleSystem()
    : particles_(Settings::maxParticles),
      modifierType_{ ParticleModifierType::None }
{
}

void ParticleSystem::update(float dt, const glm::ivec2& bounds)
{
    if (params_->particlesFrozen)
        return;

    const float t = static_cast<float>(glfwGetTime());
    const float r = std::sin(t) / 2.0f + 0.5f;
    const float g = (std::sin(t + 2.0f)) / 2.0f + 0.5f;
    const float b = (std::sin(t + 4.0f)) / 2.0f + 0.5f;

    // Only loop through active count since alive particles are packed together

    for (int i = 0; i < activeCount_;)
    {
        Particle& p = particles_[i];

        // Check if particle has died
        if (p.life <= 0)
        {
            p.color = glm::vec4{ 1.0f, 1.0f, 1.0f, 0.0f };

            // Swap particle with last alive particle
            // Could potentially be swapping with a dead particle but that will be handled next iteration
            std::swap(particles_[i], particles_[activeCount_ - 1]);
            activeCount_--;

            continue;
        }

        p.life -= dt;
        p.position += p.velocity * dt;

        if (p.position.x >= static_cast<float>(bounds.x))
        {
            p.velocity.x *= -1;
            p.position.x = static_cast<float>(bounds.x);
        }

        if (p.position.x <= 0)
        {
            p.velocity.x *= -1;
            p.position.x = 0;
        }

        if (p.position.y >= static_cast<float>(bounds.y))
        {

            p.velocity.y *= -1;
            p.position.y = static_cast<float>(bounds.y);
        }

        if (p.position.y <= 0)
        {
            p.velocity.y *= -1;
            p.position.y = 0;
        }

        p.color = glm::vec4{ r, g, b, 1.0f };

        i++;
    }
}

void ParticleSystem::emitParticles(int x, int y)
{
    for (int i = 0; i < params_->emissionRate; ++i)
    {
        if (activeCount_ == Settings::maxParticles)
            return;

        Particle& p = particles_[activeCount_];
        spawnParticleAt(p, x, y);
        activeCount_++;
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
    case ParticleModifierType::Repel:
        repelParticles(cursorX, cursorY);
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

void ParticleSystem::toggleParticleFreeze()
{
    params_->particlesFrozen = !params_->particlesFrozen;
}

void ParticleSystem::clearParticles()
{
    if (!params_->particlesFrozen)
    {
        for (Particle& p : particles_)
            p.life = 0.0f;
    }
}

void ParticleSystem::repelParticles(int x, int y)
{
    for (Particle& p : particles_)
    {
        if (p.life <= 0)
            continue;

        glm::vec2 cursorPos{ x, y };
        glm::vec2 dir{ glm::normalize(cursorPos - p.position) };

        p.velocity = -dir * glm::length(p.velocity);
    }
}