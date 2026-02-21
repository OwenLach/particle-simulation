#include "Particle.h"
#include "ParticleSystem.h"
#include "Random.h"
#include "Settings.h"
#include "Shader.h"
#include "SimulationParams.h"
#include "UI.h"

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <iostream>

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow* window);

// Creates a projection matrix that makes (0, 0) the top-left of the screen, y goes down
const glm::mat4 projection{ glm::ortho(0.0f, static_cast<float>(Settings::SCR_WIDTH),  // Left to right
                                       static_cast<float>(Settings::SCR_HEIGHT), 0.0f, // Bottom to top
                                       -1.0f, 1.0f) };                                 // Near to far

ParticleSystem particleSystem{};
SimulationParams params{ 1.5f };
UI ui{};

int main()
{
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    GLFWwindow* window = glfwCreateWindow(Settings::SCR_WIDTH, Settings::SCR_HEIGHT, "C++ Particle Simulation", NULL, NULL);
    if (window == NULL)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }

    ui.init(window, Settings::glslVersion, &params);

    Shader shader("../shaders/vertex.vert", "../shaders/fragment.frag");

    // VAO
    unsigned int vao;
    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);

    // Create vertex buffer object to store vertices in video memory
    unsigned int vbo;
    glGenBuffers(1, &vbo);

    // Bind VBO and copy vertex data into buffer memory
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER,
                 sizeof(Particle) * Settings::maxParticles, // size of all particles in particle array (bytes)
                 NULL,                                      // pointer to actual particle data
                 GL_DYNAMIC_DRAW);

    // Position attribute
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE,
                          sizeof(Particle),
                          (void*)0);
    glEnableVertexAttribArray(0);

    // Color attribute
    glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE,
                          sizeof(Particle),
                          (void*)(4 * sizeof(float)));
    glEnableVertexAttribArray(1);

    glEnable(GL_PROGRAM_POINT_SIZE);

    shader.use();
    shader.setMat4("projection", projection);
    shader.setFloat("pointSize", params.particleSize);

    float fps = 0;
    int frameCount = 0;

    double lastFrameTime = glfwGetTime();
    double lastFramerateUpdate = lastFrameTime;
    double timeElapsed = 0.0;

    while (!glfwWindowShouldClose(window))
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
        processInput(window);

        ui.newFrame();
        ui.draw(fps);

        // Update
        shader.setFloat("pointSize", params.particleSize);
        particleSystem.update(vbo, deltaTime);

        // Background color
        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        // Enable transparency
        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

        // Render
        particleSystem.render(vao);
        ui.endFrame();

        glfwSwapBuffers(window);
    }

    ui.cleanup();
    glfwTerminate();

    return 0;
}

void processInput(GLFWwindow* window)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
    {
        glfwSetWindowShouldClose(window, true);
    }

    // If UI wants the mouse don't process input
    if (ui.wantCaptureMouse())
    {
        return;
    }

    double cursorX;
    double cursorY;
    glfwGetCursorPos(window, &cursorX, &cursorY);

    int x = static_cast<int>(cursorX);
    int y = static_cast<int>(cursorY);

    if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT))
    {
        particleSystem.emitParticles(x, y);
    }

    if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_RIGHT))
    {
        particleSystem.pullParticlesTo(x, y);
    }
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    (void)window;
    glViewport(0, 0, width, height);
}