#include "Window.h"
#include "Settings.h"

#include <glad/glad.h> // Remove when take out glViewport()
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>

#include <iostream>
#include <string>

Window::Window(const WindowProps& props)
    : props_{ props }
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
        (void)window;
        Settings::SCR_HEIGHT = height;
        Settings::SCR_WIDTH = width;

        glViewport(0, 0, static_cast<GLsizei>(width), static_cast<GLsizei>(height));
    });
    // clang-format on
}

Window::~Window()
{
    if (handle_)
        glfwDestroyWindow(handle_);

    glfwTerminate();
}

void Window::update()
{
    glfwSwapBuffers(handle_);
}

glm::vec2 Window::getFrameBufferSize() const
{
    int width;
    int height;
    glfwGetFramebufferSize(handle_, &width, &height);

    return { width, height };
}

bool Window::shouldClose() const
{
    return glfwWindowShouldClose(handle_);
}