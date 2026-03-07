#pragma once

#include "Window.h"
#include "UI.h"
#include "ParticleSystem.h"
#include <glm/glm.hpp>

#include <memory>
#include <string>

class Shader;

struct ApplicationProps
{
    std::string title = "Application";
    WindowProps windowProps{};
};

class Application
{

public:
    unsigned int vao_ = 0;
    unsigned int vbo_ = 0;

public:
    Application(const ApplicationProps& props = ApplicationProps{});
    ~Application();

    void run();
    void processInput();

private:
    ApplicationProps props_;
    Window window_;
    UI ui_;

    std::unique_ptr<Shader> particleShader_;
    ParticleSystem particleSystem_;
    SimulationParams params_;
    DrawData drawData_;
    glm::mat4 projection_;
};