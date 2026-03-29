#pragma once

#include "VertexArray.h"
#include "VertexBuffer.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <cstddef>
#include <memory>

struct Framebuffer
{
    unsigned int width = 0;
    unsigned int height = 0;
};

class Renderer
{
public:
    void init(unsigned int bufferSize, const VertexLayout& layout);

    void drawPoints(unsigned int count) const;

    void update(const void* data, std::size_t size) const;

    void setProjectionMatrix(float width, float height);
    const glm::mat4& getProjectionMarix() const { return projectionMatrix_; }

    void setFramebufferSize(int width, int height);
    glm::ivec2 getFramebufferSize() const { return { framebuffer_.width, framebuffer_.height }; }

    void setClearColor(const glm::vec4& color) const;
    void clear() const;

    static void setViewport(int width, int height);

private:
    Framebuffer framebuffer_{};

    std::unique_ptr<VertexArray> vertexArray_;
    std::unique_ptr<VertexBuffer> vertexBuffer_;

    glm::mat4 projectionMatrix_;
};
