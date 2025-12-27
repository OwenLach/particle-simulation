#include "Shader.h"

#include <glad/glad.h>

#include <filesystem>
#include <fstream>
#include <iostream>
#include <string>

Shader::Shader(const char *vertexPath, const char *fragmentPath)
{
    std::string vertexStr = loadShaderSource(vertexPath);
    std::string fragmentStr = loadShaderSource(fragmentPath);

    const char *vertexSource = vertexStr.c_str();
    const char *fragmentSource = fragmentStr.c_str();

    // Vertex shader
    unsigned int vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1, &vertexSource, NULL);
    glCompileShader(vertexShader);
    checkCompileErrors(vertexShader, "VERTEX");

    // Fragment shader
    unsigned int fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &fragmentSource, NULL);
    glCompileShader(fragmentShader);
    checkCompileErrors(fragmentShader, "FRAGMENT");

    // Linking shaders
    this->ID = glCreateProgram();
    glAttachShader(this->ID, vertexShader);
    glAttachShader(this->ID, fragmentShader);
    glLinkProgram(this->ID);
    checkCompileErrors(this->ID, "PROGRAM");

    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);
}

Shader::~Shader()
{
    if (this->ID != 0)
    {
        glDeleteProgram(this->ID);
    }
}

void Shader::use()
{
    glUseProgram(ID);
}

std::string Shader::loadShaderSource(const char *path)
{
    std::ifstream file(path);

    if (!file.is_open())
    {
        std::cerr << "ERROR::SHADER::FAILED_TO_LOAD_SHADER" << std::endl;
        return "";
    }

    return std::string((std::istreambuf_iterator<char>(file)),
                       (std::istreambuf_iterator<char>()));
}

void Shader::checkCompileErrors(unsigned int shader, std::string_view type)
{
    int success;
    char log[1024];

    if (type != "PROGRAM")
    {
        glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
        if (!success)
        {
            glGetShaderInfoLog(shader, sizeof(log), NULL, log);
            std::cerr << "ERROR::SHADER_COMPILATION_ERROR of type: " << type << "\n"
                      << log
                      << "\n -- --------------------------------------------------- -- "
                      << std::endl;
        }
    }
    else
    {
        glGetProgramiv(shader, GL_LINK_STATUS, &success);
        if (!success)
        {
            glGetProgramInfoLog(shader, sizeof(log), NULL, log);
            std::cerr << "ERROR::PROGRAM_LINKING_ERROR of type: " << type << "\n"
                      << log
                      << "\n -- --------------------------------------------------- -- "
                      << std::endl;
        }
    }
}