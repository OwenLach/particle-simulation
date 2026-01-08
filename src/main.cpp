#include "Shader.h"
#include "Random.h"

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>

#include <iostream>

struct Particle
{
    glm::vec2 position;
    glm::vec2 velocity;
    float timeAliveMS;

    Particle() : position(0.0f), velocity(0.0f), timeAliveMS(0.0f) {}
};

void framebuffer_size_callback(GLFWwindow *window, int width, int height);
void processInput(GLFWwindow *window);

void initParticles();
void renderParticles(int vao);
void updateParticles(int vbo);

// settings
constexpr unsigned int SCR_WIDTH = 1600;
constexpr unsigned int SCR_HEIGHT = 1200;
constexpr int particleCount = 1000;

std::vector<Particle> particles;

int main()
{
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    GLFWwindow *window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "LearnOpenGL", NULL, NULL);
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
                 sizeof(Particle) * particleCount, // size of all particles in particle array (bytes)
                 NULL,                            // pointer to actual particle data
                 GL_STATIC_DRAW);

    // Position attribute
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(Particle), (void *)0);
    glEnableVertexAttribArray(0);

    // Use Shader
    shader.use();

    glEnable(GL_PROGRAM_POINT_SIZE);

    initParticles();

    while (!glfwWindowShouldClose(window))
    {
        processInput(window);

        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        updateParticles(vbo);
        renderParticles(vao);

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwTerminate();
    return 0;
}

void processInput(GLFWwindow *window)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
}

void framebuffer_size_callback(GLFWwindow *window, int width, int height)
{
    // make sure the viewport matches the new window dimensions; note that width and
    // height will be significantly larger than specified on retina displays.
    glViewport(0, 0, width, height);
}

void initParticles()
{
    for (int i{}; i < particleCount; ++i)
    {
        Particle p;
        glm::vec2 velocity {
            Random::get(-1.0f, 1.0f),
            Random::get(-1.0f, 1.0f)
        };
        const float speed = Random::get(0.0001f, 0.005f);
        p.velocity = glm::normalize(velocity) * speed;

        particles.push_back(p);
    }
}

void renderParticles(int vao)
{
    glBindVertexArray(vao);
    glDrawArrays(GL_POINTS, 0, static_cast<int>(particles.size()));
}

void updateParticles(int vbo)
{
    for (int i{}; i < particleCount; ++i)
    {
        Particle &p = particles[i];
        p.position.x += p.velocity.x;
        p.position.y += p.velocity.y;
    }

    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER,
                 sizeof(Particle) * particleCount, // size of all particles in particle array (bytes)
                 particles.data(),                // pointer to actual particle data
                 GL_STATIC_DRAW);
}
