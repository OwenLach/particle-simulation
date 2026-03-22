#pragma once

#include <string>

struct GLFWwindow;
struct SimulationParams;

struct DrawData
{
    std::string title{};
    std::string activeModifier{};
    float fps = 0;
    int activeParticles = 0;
};

class UI
{
public:
    void init(GLFWwindow* window, const char* glslVersion);
    void cleanup();

    void newFrame();
    void draw(SimulationParams& params, DrawData& drawData);
    void endFrame();

    // put in InputManager class
    bool wantCaptureMouse();
};