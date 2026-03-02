#include "ParticleSystem.h"
#include "Settings.h"
#include "Random.h"

#include <glad/glad.h>
#include <GLFW/glfw3.h>

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

void ParticleSystem::update(unsigned int vbo, float dt)
{
    const float t = static_cast<float>(glfwGetTime());
    const float r = std::sin(t) / 2.0f + 0.5f;
    const float g = (std::sin(t + 2.0f)) / 2.0f + 0.5f;
    const float b = (std::sin(t + 4.0f)) / 2.0f + 0.5f;

    for (int i = 0; i < Settings::maxParticles; ++i)
    {
        Particle& p = particles_[i];

        // Set dead particles to be transparent
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

    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferSubData(GL_ARRAY_BUFFER,
                    0,
                    sizeof(Particle) * Settings::maxParticles,
                    particles_.data());
}

void ParticleSystem::render(unsigned int vao) const
{
    glBindVertexArray(vao);
    glDrawArrays(GL_POINTS, 0, static_cast<int>(particles_.size()));
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

void ParticleSystem::spawnParticleAt(Particle& p, int x, int y)
{
    glm::vec2 directionVec{
        Random::get(-1.0f, 1.0f),
        Random::get(-1.0f, 1.0f)
    };
    const float speed = Random::get(params_->particleMinSpeed, params_->particleMaxSpeed);

    p.velocity = glm::normalize(directionVec) * speed;
    p.life = Random::get(5.0f, 15.0f);
    p.position = glm::vec2{ x, y };
    p.color = glm::vec4{ 1.0f, 1.0f, 1.0f, 1.0f };
}

void ParticleSystem::pullParticlesTo(int x, int y)
{
    for (int i = 0; i < Settings::maxParticles; ++i)
    {
        Particle& p = particles_[i];
        if (p.life > 0)
        {
            glm::vec2 cursorVec{ x, y };
            glm::vec2 dir{ glm::normalize(cursorVec - p.position) };

            const float speed = Random::get(params_->particleMinSpeed, params_->particleMaxSpeed);
            p.velocity = dir * speed;
        }
    }
}