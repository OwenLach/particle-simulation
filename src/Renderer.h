#pragma once

#include "VertexArray.h"
#include "VertexBuffer.h"

#include <glm/glm.hpp>

#include <cstddef>
#include <memory>

class Renderer
{
public:
    void init(unsigned int bufferSize, const VertexLayout& layout);
    void drawPoints(unsigned int count) const;
    void update(const void* data, std::size_t size) const;

    void setClearColor(const glm::vec4& color) const;
    void clear() const;

    static void setViewport(int width, int height);

private:
    std::unique_ptr<VertexArray> vertexArray_;
    std::unique_ptr<VertexBuffer> vertexBuffer_;
};