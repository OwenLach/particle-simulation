// Architecutre inspiration from "TheCherno"
// https://github.com/TheCherno/Architecture

#include "Application.h"

int main()
{
    ApplicationProps appProps{ "Particle Simulation", { Settings::SCR_WIDTH, Settings::SCR_HEIGHT, "Particle Simulation" } };
    Application app{ appProps };
    app.run();
}