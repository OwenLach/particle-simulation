#pragma once

#include <chrono>
#include <random>

namespace Random
{
    inline std::mt19937 generate()
    {
        std::random_device rd{};
        auto now = static_cast<std::mt19937::result_type>(std::chrono::steady_clock::now().time_since_epoch().count());
        std::seed_seq ss{ now, rd(), rd(), rd(), rd(), rd(), rd(), rd() };

        return std::mt19937{ ss };
    }

    inline std::mt19937 mt{ generate() };

    template <typename T>
    T get(T min, T max)
    {
        return std::uniform_int_distribution<T>{ min, max }(mt);
    }

    inline float get(float min, float max)
    {
        return std::uniform_real_distribution{ min, max }(mt);
    }
}