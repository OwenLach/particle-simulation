#include "UI.h"
#include "ParticleSystem.h"

#include <GLFW/glfw3.h>

#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>

#include <iostream>

void UI::init(GLFWwindow* window, const char* glslVersion)
{
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

void UI::draw(SimulationParams& params, DrawData& drawData)
{
    // ImGui::Begin("Stats");
    ImGui::Begin(drawData.title.c_str());
    ImGui::TextColored({ 1.0f, 0.0f, 0.0f, 1.0f }, "FPS: %.2f", drawData.fps);
    ImGui::Text("Current Modifier: %s", drawData.activeModifier.c_str());

    ImGui::SliderFloat("Particle Size", &params.particleSize, 0.1f, 5.0f);

    ImGui::SliderInt("Emission Rate", &params.emissionRate, 1, 40);

    ImGui::SliderFloat("Particle Min Speed", &params.particleMinSpeed, 1.0f, 100.0f);
    ImGui::SliderFloat("Particle Max Speed", &params.particleMaxSpeed, 100.0f, 200.0f);

    ImGui::SliderFloat("Particle Min Life", &params.particleMinLife, 1.0f, 15.0f);
    ImGui::SliderFloat("Particle Max Life", &params.particleMaxLife, 15.0f, 30.0f);

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