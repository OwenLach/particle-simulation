#include "Shader.h"
#include "Random.h"

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <iostream>

void framebuffer_size_callback(GLFWwindow *window, int width, int height);
void processInput(GLFWwindow *window);

struct Particle;

void initParticles();
void renderParticles(unsigned int vao);
void updateParticles(unsigned int vbo, float dt);
void emitParticles(int x, int y);
void spawnParticleAt(Particle &p, int x, int y);

// settings
constexpr unsigned int SCR_WIDTH{1400};
constexpr unsigned int SCR_HEIGHT{1200};

constexpr int maxParticles{10'000};
constexpr int particlesPerFrame{4};
int particleIndex{0};

// Creates a projection matrix that makes (0, 0) the top-left of the screen, y goes down
const glm::mat4 projection{glm::ortho(0.0f, static_cast<float>(SCR_WIDTH),  // Left to right
                                      static_cast<float>(SCR_HEIGHT), 0.0f, // Bottom to top
                                      -1.0f, 1.0f)};                        // Near to far

struct Particle
{
    glm::vec2 position;
    glm::vec2 velocity;
    float life;

    Particle() : position({static_cast<float>(SCR_WIDTH) / 2.0f, static_cast<float>(SCR_HEIGHT) / 2.0f}), velocity(0.0f), life(0.0f) {}
};
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
                 sizeof(Particle) * maxParticles, // size of all particles in particle array (bytes)
                 NULL,                            // pointer to actual particle data
                 GL_STATIC_DRAW);

    // Position attribute
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(Particle), (void *)0);
    glEnableVertexAttribArray(0);

    // Use Shader
    shader.use();
    shader.setMat4("projection", projection);

    glEnable(GL_PROGRAM_POINT_SIZE);

    initParticles();

    float lastFrame = static_cast<float>(glfwGetTime());

    while (!glfwWindowShouldClose(window))
    {
        float currentFrame = static_cast<float>(glfwGetTime());
        float deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

        processInput(window);

        updateParticles(vbo, deltaTime);
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
    {
        glfwSetWindowShouldClose(window, true);
    }

    if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT))
    {
        double cursorX{};
        double cursorY{};
        glfwGetCursorPos(window, &cursorX, &cursorY);
        // std::cout << "(" << cursorX << ", " << cursorY << ")\n";
        emitParticles(static_cast<int>(cursorX), static_cast<int>(cursorY));
    }
}

void framebuffer_size_callback([[maybe_unused]] GLFWwindow *window, int width, int height)
{
    (void)window;
    glViewport(0, 0, width, height);
}

void initParticles()
{
    particles.reserve(maxParticles);

    for (int i{}; i < maxParticles; ++i)
    {
        Particle p;
        particles.push_back(p);
    }
}

void renderParticles(unsigned int vao)
{
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    glBindVertexArray(vao);
    glDrawArrays(GL_POINTS, 0, static_cast<int>(particles.size()));
}

void updateParticles(unsigned int vbo, float dt)
{
    for (int i{}; i < maxParticles; ++i)
    {
        Particle &p = particles[i];
        if (p.life > 0)
        {
            p.life -= dt;
            p.position += p.velocity * dt;
        }
    }

    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER,
                 sizeof(Particle) * maxParticles, // size of all particles in particle array (bytes)
                 particles.data(),                // pointer to actual particle data
                 GL_STATIC_DRAW);
}

void emitParticles(int x, int y)
{
    for (int i{0}; i < particlesPerFrame; ++i)
    {
        Particle &p = particles[particleIndex];
        particleIndex = (particleIndex + 1) % maxParticles;

        if (p.life <= 0)
        {
            spawnParticleAt(p, x, y);
        }
    }
}

void spawnParticleAt(Particle &p, int x, int y)
{
    glm::vec2 directionVec{
        Random::get(-1.0f, 1.0f),
        Random::get(-1.0f, 1.0f)};
    const float speed = Random::get(10.0f, 100.0f);

    p.velocity = glm::normalize(directionVec) * speed;
    p.life = Random::get(5.0f, 15.0f);
    p.position = glm::vec2{x, y};
}