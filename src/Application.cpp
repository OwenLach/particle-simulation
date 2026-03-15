#include "Application.h"
#include "Window.h"
#include "UI.h"
#include "ParticleSystem.h"
#include "Shader.h"
#include "Settings.h"
#include "Input.h"
#include "VertexArray.h"
#include "VertexBuffer.h"

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <iostream>
#include <memory>
#include <cstddef>

Application::Application(const ApplicationProps& props)
    : props_{ props },
      window_{ props.windowProps },
      ui_{},
      particleSystem_{},
      params_{ 1.5f },
      drawData_{ "Stats", "None", 0.0f }
{
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        throw std::runtime_error("Failed to initialize GLAD\n");
    }

    ui_.init(window_.getHandle(), Settings::glslVersion);
    particleSystem_.setParams(&params_);
    particleShader_ = std::make_unique<Shader>("../shaders/vertex.vert", "../shaders/fragment.frag");

    vao_ = std::make_unique<VertexArray>();
    vbo_ = std::make_unique<VertexBuffer>(sizeof(Particle) * Settings::maxParticles);

    VertexLayout layout{};
    layout.push<float>("Position", 2, sizeof(Particle), offsetof(Particle, position));
    layout.push<float>("Color", 4, sizeof(Particle), offsetof(Particle, color));

    vao_->setLayout(layout, *vbo_);

    // Enable resizeable points through gl_PointSize in shader
    glEnable(GL_PROGRAM_POINT_SIZE);

    // Enable transparency
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    // Background color
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

    particleShader_->use();
}

Application::~Application()
{
    ui_.cleanup();
    glfwTerminate();
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
        glfwPollEvents();
        processInput();

        drawData_.fps = fps;
        drawData_.activeModifier = toString(particleSystem_.getCurrentModifier());

        ui_.newFrame();
        ui_.draw(params_, drawData_);

        // Update
        glm::vec2 framebufferSize = window_.getFrameBufferSize();
        particleShader_->setMat4("projection", glm::ortho(0.0f, static_cast<float>(framebufferSize.x), static_cast<float>(framebufferSize.y), 0.0f, -1.0f, 1.0f));
        particleShader_->setFloat("pointSize", params_.particleSize);
        particleSystem_.update(vbo_->getID(), deltaTime);

        // Clear the screen
        glClear(GL_COLOR_BUFFER_BIT);

        // Render
        particleSystem_.render(vao_->getID());
        ui_.endFrame();

        window_.update();
    }
}

void Application::processInput()
{
    GLFWwindow* window = window_.getHandle();

    if (Input::isKeyPressed(window, Key::Escape))
    {
        glfwSetWindowShouldClose(window_.getHandle(), true);
    }

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

    if (Input::isKeyPressed(window, Key::D1))
    {
        particleSystem_.setParticleModifier(ParticleModifierType::Pull);
    }

    if (Input::isKeyPressed(window, Key::D2))
    {
        particleSystem_.setParticleModifier(ParticleModifierType::Circle);
    }
}