#pragma once

#include <string_view>

class Shader
{
public:
    unsigned int ID;

    Shader(const char *vertexPath, const char *fragmentPath);
    ~Shader();
    
    void use();

private:
    std::string loadShaderSource(const char *path);
    void checkCompileErrors(unsigned int shader, std::string_view type);
};