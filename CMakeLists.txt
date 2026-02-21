cmake_minimum_required(VERSION 3.20)
project(ParticleSimulator)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# Find packages
find_package(OpenGL REQUIRED)
find_package(glfw3 REQUIRED)

# Create library for glad
add_library(glad src/glad.c)
target_include_directories(glad PUBLIC include)

# Add executable
add_executable(ParticleSimulator 
    src/main.cpp
    src/Shader.cpp
    src/UI.cpp
    libs/imgui/imgui.cpp
    libs/imgui/imgui_demo.cpp
    libs/imgui/imgui_draw.cpp
    libs/imgui/imgui_tables.cpp
    libs/imgui/imgui_widgets.cpp
    libs/imgui/imgui_impl_glfw.cpp
    libs/imgui/imgui_impl_opengl3.cpp
)

target_compile_options(ParticleSimulator PRIVATE
    -Wall
    -Wextra
    -Wpedantic
    -Wshadow
    -Wconversion
)

# Include directories
target_include_directories(ParticleSimulator PRIVATE 
    ${CMAKE_SOURCE_DIR}/include
    ${CMAKE_SOURCE_DIR}/libs/imgui
)

# Link libraries
target_link_libraries(ParticleSimulator PRIVATE 
    OpenGL::GL 
    glfw 
    glad
)