#include "Window.h"
#include "Settings.h"
#include "Renderer.h"

#include <GLFW/glfw3.h>

#include <glm/glm.hpp>

#include <iostream>
#include <string>

Window::Window(int screenWidth, int screenHeight, std::string_view title)
    : props_{ screenWidth, screenHeight, std::string{ title } }
{
    glfwInit();

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    handle_ = glfwCreateWindow(props_.width, props_.height, props_.title.c_str(), NULL, NULL);
    if (!handle_)
    {
        glfwTerminate();
        throw std::runtime_error("Failed to create GLFW window");
    }

    glfwMakeContextCurrent(handle_);
    glfwSetWindowUserPointer(handle_, this);

    // clang-format off
    glfwSetWindowSizeCallback(handle_, [](GLFWwindow* window, int width, int height)
    {
        Window* handle = static_cast<Window*>(glfwGetWindowUserPointer(window));
        handle->props_.height = height;
        handle->props_.width = width; 
    });

    glfwSetFramebufferSizeCallback(handle_, [](GLFWwindow* window, int width, int height)
    {
        Window* handle = static_cast<Window*>(glfwGetWindowUserPointer(window));
        if (handle->resizeCallback_)
            handle->resizeCallback_(width, height);
    });
    // clang-format on
}

Window::~Window()
{
    if (handle_)
        glfwDestroyWindow(handle_);

    glfwTerminate();
}

void Window::swapBuffers()
{
    glfwSwapBuffers(handle_);
}

bool Window::shouldClose() const
{
    return glfwWindowShouldClose(handle_);
}
