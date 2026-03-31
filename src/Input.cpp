#include "Input.h"

#include <GLFW/glfw3.h>
#include <glm/glm.hpp>

std::unordered_map<int, std::function<void()>> Input::keyCallbacks_{};

void Input::init(GLFWwindow* window)
{
    glfwSetKeyCallback(window, keyCallback);
}

void Input::registerKeyCallback(Key key, std::function<void()> fn)
{
    Input::keyCallbacks_[static_cast<int>(key)] = fn;
}

void Input::keyCallback(GLFWwindow*, int key, int, int action, int)
{
    if (Input::keyCallbacks_.find(key) != Input::keyCallbacks_.end() && action == GLFW_PRESS)
        (Input::keyCallbacks_[key])();
}

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