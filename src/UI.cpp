#include "UI.h"
#include "ParticleSystem.h"

#include <GLFW/glfw3.h>

#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>

#include <iostream>

void UI::init(GLFWwindow* window, const char* glslVersion, SimulationParams* params)
{
    params_ = params;

    // Setup ImGui context
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO();
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard; // Enable Keyboard Controls
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;  // Enable Gamepad Controls
    io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;

    // Setup Platform/Renderer backends
    ImGui_ImplGlfw_InitForOpenGL(window, true); // Second param install_callback=true will install GLFW callbacks and chain to existing ones.
    ImGui_ImplOpenGL3_Init(glslVersion);
}

void UI::cleanup()
{
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();
}

void UI::newFrame()
{
    // Start ImGui frame
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();
}

void UI::draw(float fps)
{
    ImGui::Begin("Stats");
    ImGui::Text("FPS: %.2f", fps);

    ImGui::SliderFloat("Particle Size", &params_->particleSize, 0.1f, 5.0f);

    ImGui::SliderInt("Emission Rate", &params_->emissionRate, 1, 20);

    ImGui::SliderFloat("Particle Min Speed", &params_->particleMinSpeed, 1.0f, 100.0f);
    ImGui::SliderFloat("Particle Max Speed", &params_->particleMaxSpeed, 100.0f, 200.0f);

    ImGui::SliderFloat("Particle Min Life", &params_->particleMinLife, 1.0f, 15.0f);
    ImGui::SliderFloat("Particle Max Life", &params_->particleMaxLife, 15.0f, 30.0f);
    // ImGui::ShowDemoWindow();
    ImGui::End();
}

void UI::endFrame()
{
    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}

bool UI::wantCaptureMouse()
{
    return ImGui::GetIO().WantCaptureMouse;
}