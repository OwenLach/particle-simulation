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
)

target_compile_options(ParticleSimulator PRIVATE
    -Wall
    -Wextra
    -Wpedantic
    -Wshadow
    -Wconversion
)

# Include directories
target_include_directories(ParticleSimulator PRIVATE ${CMAKE_SOURCE_DIR}/include)

# Link libraries
target_link_libraries(ParticleSimulator PRIVATE OpenGL::GL glfw glad)