#pragma once

struct GLFWwindow;
struct SimulationParams;

class UI
{
public:
    void init(GLFWwindow* window, const char* glslVersion, SimulationParams* params);
    void cleanup();

    void newFrame();
    void draw();
    void endFrame();

    bool wantCaptureMouse();

private:
    SimulationParams* params_;
};