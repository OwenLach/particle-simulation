#pragma once

struct GLFWwindow;
struct SimulationParams;

class UI
{
public:
    void init(GLFWwindow* window, const char* glslVersion, SimulationParams* params);
    void cleanup();

    void newFrame();
    void draw(float fps);
    void endFrame();

    // put in InputManager class
    bool wantCaptureMouse();

private:
    SimulationParams* params_{};
};