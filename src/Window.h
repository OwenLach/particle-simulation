// Architecutre inspiration from "TheCherno"
// https://github.com/TheCherno/Architecture/blob/50570f7908f7dff47ac7d89b3280d84133eaafce/Core/Source/Core/Window.h

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
    // Remove create() and destroy() when done Application class
    Window(const WindowProps& props = WindowProps{});
    ~Window();

    void create();
    void destroy();
    void update();

    // glm::vec2 getMousePos() const;
    glm::vec2 getFrameBufferSize() const;
    GLFWwindow* getHandle() const { return handle_; }

    bool shouldClose() const;

private:
    WindowProps props_{};
    GLFWwindow* handle_ = nullptr;
};