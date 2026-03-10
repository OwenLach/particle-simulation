#pragma once

#include <glm/glm.hpp>

struct GLFWwindow;

enum class Key
{
    // Maps to GLFW keys
    D0 = 48,
    D1 = 49,
    D2 = 50,
    D3 = 51,
    D4 = 52,
    D5 = 53,
    D6 = 54,
    D7 = 55,
    D8 = 56,
    D9 = 57,
    C = 67,
    F = 70,
    Escape = 256,
};

enum class MouseButton
{
    Left,
    Right
};

class Input
{
public:
    static bool isKeyPressed(GLFWwindow* window, Key key);
    static bool isMouseButtonPressed(GLFWwindow* window, MouseButton button);
    static glm::vec2 getMousePos(GLFWwindow* window);
};