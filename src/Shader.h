#pragma once

#include <glm/glm.hpp>

#include <string>
#include <string_view>

class Shader
{
public:
    unsigned int ID;

    Shader(const char *vertexPath, const char *fragmentPath);
    ~Shader();

    void use();

    void setBool(const std::string &name, bool value) const;
    void setInt(const std::string &name, int value) const;
    void setFloat(const std::string &name, float value) const;
    void setMat4(const std::string &name, const glm::mat4 &mat) const;

private:
    std::string loadShaderSource(const char *path);
    void checkCompileErrors(unsigned int shader, std::string_view type);
};