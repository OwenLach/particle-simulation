#pragma once

namespace Settings
{
    inline constexpr const char* glslVersion = "#version 330 core";
    inline constexpr int maxParticles = 100'000;

    // Simulation Parameters
    inline constexpr float particleSize = 1.5f;
    inline constexpr float particleMinSpeed = 10.0f;
    inline constexpr float particleMaxSpeed = 100.0f;
    inline constexpr float particleMinLife = 5.0f;
    inline constexpr float particleMaxLife = 30.0f;
    inline constexpr int emissionRate = 10;
    inline constexpr bool particlesFrozen = false;
};