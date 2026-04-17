#pragma once

#include "Window.h"
#include "UI.h"
#include "ParticleSystem.h"

#include <memory>
#include <string_view>

class Shader;
class VertexArray;
class VertexBuffer;
class Renderer;

class Application
{
public:
    Application(int screenWidth, int screenHeight, std::string_view title);
    ~Application();

    void run();
    void processInput(float deltaTime);

private:
    void onWindowResize(int height, int width);

private:
    Window window_;
    UI ui_;

    std::unique_ptr<Renderer> renderer_;
    std::unique_ptr<Shader> particleShader_;
    ParticleSystem particleSystem_;
    SimulationParams params_;
    DrawData drawData_;
};