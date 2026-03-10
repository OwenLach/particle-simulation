#include "Input.h"

#include <GLFW/glfw3.h>
#include <glm/glm.hpp>

bool Input::isKeyPressed(GLFWwindow* window, Key key)
{
    return glfwGetKey(window, static_cast<int>(key)) == GLFW_PRESS;
}

bool Input::isMouseButtonPressed(GLFWwindow* window, MouseButton button)
{
    return glfwGetMouseButton(window, static_cast<int>(button)) == GLFW_PRESS;
}

glm::vec2 Input::getMousePos(GLFWwindow* window)
{
    double cursorX;
    double cursorY;
    glfwGetCursorPos(window, &cursorX, &cursorY);

    return { static_cast<float>(cursorX), static_cast<float>(cursorY) };
}