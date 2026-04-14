#include "Application.h"
#include "Window.h"
#include "UI.h"
#include "ParticleSystem.h"
#include "Shader.h"
#include "Settings.h"
#include "Input.h"
#include "VertexArray.h"
#include "Renderer.h"

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <memory>
#include <cstddef>
#include <cstdlib>

Application::Application(int screenWidth, int screenHeight, std::string_view title)
    : window_{ screenWidth, screenHeight, title },
      ui_{},
      particleSystem_{},
      params_{
          .particleSize = Settings::particleSize,
          .particleMinSpeed = Settings::particleMinSpeed,
          .particleMaxSpeed = Settings::particleMaxSpeed,
          .particleMinLife = Settings::particleMinLife,
          .particleMaxLife = Settings::particleMaxLife,
          .emissionRate = Settings::emissionRate,
          .particlesFrozen = Settings::particlesFrozen
      },
      drawData_{ "Stats", "None", 0.0f, 0 }
{
    // clang-format off
    window_.setResizeCallback([this](int width, int height) 
    { 
        onWindowResize(width, height); 
    });

    Input::init(window_.getHandle());

    Input::registerKeyCallback(Key::F, [this](){
        particleSystem_.toggleParticleFreeze();
    });

    Input::registerKeyCallback(Key::C, [this](){
        particleSystem_.clearParticles();
    });

    Input::registerKeyCallback(Key::D1, [this](){
        particleSystem_.setParticleModifier(ParticleModifierType::Pull);
    });

    Input::registerKeyCallback(Key::D2, [this](){
        particleSystem_.setParticleModifier(ParticleModifierType::Circle);
    });

    Input::registerKeyCallback(Key::D3, [this](){
        particleSystem_.setParticleModifier(ParticleModifierType::Repel);
    });

    Input::registerKeyCallback(Key::Escape, [this](){
        glfwSetWindowShouldClose(window_.getHandle(), true);
    });
    // clang-format on

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        throw std::runtime_error("Failed to initialize GLAD\n");
    }

    ui_.init(window_.getHandle(), Settings::glslVersion);
    particleSystem_.setParams(&params_);
    particleShader_ = std::make_unique<Shader>("../shaders/vertex.vert", "../shaders/fragment.frag");

    renderer_ = std::make_unique<Renderer>();

    VertexLayout layout{};
    layout.push<float>("Position", 2, sizeof(Particle), offsetof(Particle, position));
    layout.push<float>("Color", 4, sizeof(Particle), offsetof(Particle, color));

    renderer_->init(static_cast<unsigned int>(Settings::maxParticles * sizeof(Particle)), layout);
    renderer_->setClearColor({ 0.0f, 0.0f, 0.0f, 1.0f });

    particleShader_->use();

    // Call callback manually to setup projection matrix and viewport
    onWindowResize(screenWidth, screenHeight);
}

Application::~Application()
{
    ui_.cleanup();
}

void Application::run()
{
    float fps = 0;
    int frameCount = 0;
    double lastFrameTime = glfwGetTime();
    double lastFramerateUpdate = lastFrameTime;
    double timeElapsed = 0.0;

    while (!window_.shouldClose())
    {
        frameCount++;

        // Delta Time
        const double now = glfwGetTime();
        const float deltaTime = static_cast<float>(now - lastFrameTime);
        lastFrameTime = now;

        timeElapsed = now - lastFramerateUpdate;
        // Update framerate every second
        if (timeElapsed >= 1.0)
        {
            fps = static_cast<float>(frameCount / timeElapsed);
            lastFramerateUpdate = now;
            frameCount = 0;
        }

        // Input
        processInput();

        drawData_.fps = fps;
        drawData_.activeModifier = toString(particleSystem_.getCurrentModifier());
        drawData_.activeParticles = particleSystem_.getActiveParticleCount();

        ui_.newFrame();
        ui_.draw(params_, drawData_);

        particleShader_->setFloat("pointSize", params_.particleSize);
        particleSystem_.update(deltaTime, renderer_->getFramebufferSize());
        renderer_->update(particleSystem_.getParticleRenderData(), particleSystem_.getActiveParticleCount() * sizeof(Particle));
        renderer_->clear();

        // Render
        renderer_->drawPoints(particleSystem_.getActiveParticleCount());
        ui_.endFrame();

        window_.swapBuffers();
    }
}

void Application::processInput()
{
    glfwPollEvents();

    GLFWwindow* window = window_.getHandle();

    // If UI wants the mouse don't process input
    if (ui_.wantCaptureMouse())
        return;

    glm::vec2 cursorPos{ Input::getMousePos(window) };
    int x = static_cast<int>(cursorPos.x);
    int y = static_cast<int>(cursorPos.y);

    if (Input::isMouseButtonPressed(window, MouseButton::Left))
    {
        particleSystem_.emitParticles(x, y);
    }

    if (Input::isMouseButtonPressed(window, MouseButton::Right))
    {
        particleSystem_.applyModifier(x, y);
    }
}

void Application::onWindowResize(int width, int height)
{
    renderer_->setViewport(width, height);
    renderer_->setProjectionMatrix(static_cast<float>(width), static_cast<float>(height));
    renderer_->setFramebufferSize(width, height);

    particleShader_->setMat4("projection", renderer_->getProjectionMarix());
}
