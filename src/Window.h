#pragma once

#include "Settings.h"

#include <glm/glm.hpp>

#include <string>
#include <functional>

struct GLFWwindow;

struct WindowProps
{
    int width = 0;
    int height = 0;
    std::string title{};
};

class Window
{
public:
    Window(int screenWidth, int screenHeight, std::string_view title);
    ~Window();

    void swapBuffers();

    void setResizeCallback(std::function<void(int, int)> callback) { resizeCallback_ = callback; }

    GLFWwindow* getHandle() const { return handle_; }

    bool shouldClose() const;

private:
    WindowProps props_{};
    GLFWwindow* handle_ = nullptr;
    std::function<void(int, int)> resizeCallback_{};
};