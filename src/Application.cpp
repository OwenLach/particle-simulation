#include "Application.h"
#include "Window.h"
#include "UI.h"
#include "ParticleSystem.h"
#include "Shader.h"
#include "Settings.h"

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <iostream>
#include <memory>

Application::Application(const ApplicationProps& props)
    : props_{ props },
      window_{ props.windowProps },
      ui_{},
      particleSystem_{},
      params_{ 1.5f },
      drawData_{ "Stats", "None", 0.0f },
      projection_{ glm::ortho(0.0f, static_cast<float>(Settings::SCR_WIDTH),  // Left to right
                              static_cast<float>(Settings::SCR_HEIGHT), 0.0f, // Bottom to top
                              -1.0f, 1.0f) }
{
    glfwInit();

    window_.create();

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        throw std::runtime_error("Failed to initialize GLAD\n");
    }

    ui_.init(window_.getHandle(), Settings::glslVersion);

    particleSystem_.setParams(&params_);

    particleShader_ = std::make_unique<Shader>("../shaders/vertex.vert", "../shaders/fragment.frag");

    // VAO
    glGenVertexArrays(1, &vao_);
    glBindVertexArray(vao_);

    // Create vertex buffer object to store vertices in video memory
    glGenBuffers(1, &vbo_);

    // Bind VBO and copy vertex data into buffer memory
    glBindBuffer(GL_ARRAY_BUFFER, vbo_);
    glBufferData(GL_ARRAY_BUFFER,
                 sizeof(Particle) * Settings::maxParticles, // size of all particles in particle array (bytes)
                 NULL,                                      // pointer to actual particle data
                 GL_DYNAMIC_DRAW);

    // Position attribute
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(Particle), (void*)0);
    glEnableVertexAttribArray(0);

    // Color attribute
    glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, sizeof(Particle), (void*)(4 * sizeof(float)));
    glEnableVertexAttribArray(1);

    // Enable resizeable points through gl_PointSize in shader
    glEnable(GL_PROGRAM_POINT_SIZE);

    // Enable transparency
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    // Background color
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

    // glm::vec2 framebufferSize{ window.getFrameBufferSize() };

    particleShader_->use();
}

Application::~Application()
{
    window_.destroy();
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
        particleSystem_.update(vbo_, deltaTime);

        // Clear the screen
        glClear(GL_COLOR_BUFFER_BIT);

        // Render
        particleSystem_.render(vao_);
        ui_.endFrame();

        window_.update();
    }
}

void Application::processInput()
{
    if (glfwGetKey(window_.getHandle(), GLFW_KEY_ESCAPE) == GLFW_PRESS)
    {
        glfwSetWindowShouldClose(window_.getHandle(), true);
    }

    // If UI wants the mouse don't process input
    if (ui_.wantCaptureMouse())
    {
        return;
    }

    double cursorX;
    double cursorY;
    glfwGetCursorPos(window_.getHandle(), &cursorX, &cursorY);

    int x = static_cast<int>(cursorX);
    int y = static_cast<int>(cursorY);

    if (glfwGetMouseButton(window_.getHandle(), GLFW_MOUSE_BUTTON_LEFT))
    {
        particleSystem_.emitParticles(x, y);
    }

    if (glfwGetMouseButton(window_.getHandle(), GLFW_MOUSE_BUTTON_RIGHT))
    {
        particleSystem_.applyModifier(x, y);
    }

    if (glfwGetKey(window_.getHandle(), GLFW_KEY_1))
    {
        particleSystem_.setParticleModifier(ParticleModifierType::Pull);
    }

    if (glfwGetKey(window_.getHandle(), GLFW_KEY_2))
    {
        particleSystem_.setParticleModifier(ParticleModifierType::Circle);
    }
}