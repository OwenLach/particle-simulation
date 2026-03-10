#pragma once

#include "Settings.h"

#include <glm/glm.hpp>

#include <string>

struct GLFWwindow;

struct WindowProps
{
    int width = Settings::SCR_WIDTH;
    int height = Settings::SCR_HEIGHT;
    std::string title = "Window";
};

class Window
{
public:
    Window(const WindowProps& props = WindowProps{});
    ~Window();

    void update();

    glm::vec2 getFrameBufferSize() const;
    GLFWwindow* getHandle() const { return handle_; }

    bool shouldClose() const;

private:
    WindowProps props_{};
    GLFWwindow* handle_ = nullptr;
};