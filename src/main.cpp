// Architecture inspiration from "TheCherno"
// https://github.com/TheCherno/Architecture

#include "Application.h"

int main()
{
    constexpr int screenHeight = 2200;
    constexpr int screenWidth = 1200;

    Application app{ screenHeight, screenWidth, "Particle Simulation" };
    app.run();
}