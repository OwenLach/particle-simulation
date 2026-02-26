// Architecutre inspiration from "TheCherno"
// https://github.com/TheCherno/Architecture/blob/50570f7908f7dff47ac7d89b3280d84133eaafce/Core/Source/Core/Window.h

#pragma once

#include <glm/glm.hpp>

#include <string>

struct GLFWwindow;

class Window
{
    struct WindowProps
    {
        int width = 1200;
        int height = 1400;
        std::string title = "Window";
    };

public:
    // Remove create() and destroy() when done Application class
    Window(const WindowProps& props);
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