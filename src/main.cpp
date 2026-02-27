#include "Application.h"

int main()
{
    ApplicationProps appProps{ "Particle Simulation", { Settings::SCR_WIDTH, Settings::SCR_HEIGHT } };
    Application app{ appProps };
    app.run();
}