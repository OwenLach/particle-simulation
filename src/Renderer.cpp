#include "Renderer.h"
#include "VertexArray.h"
#include "VertexBuffer.h"

#include <glad/glad.h>
#include <glm/glm.hpp>

void Renderer::init(unsigned int bufferSize, const VertexLayout& layout)
{
    // Enable resizeable points through gl_PointSize in shader
    glEnable(GL_PROGRAM_POINT_SIZE);

    // Enable transparency
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    vertexArray_ = std::make_unique<VertexArray>();
    vertexBuffer_ = std::make_unique<VertexBuffer>(bufferSize);

    vertexArray_->setLayout(layout, *vertexBuffer_);
}

void Renderer::drawPoints(unsigned int count) const
{
    vertexArray_->bind();
    glDrawArrays(GL_POINTS, 0, count);
}

void Renderer::update(const void* data, std::size_t size) const
{
    vertexBuffer_->bind();
    glBufferSubData(GL_ARRAY_BUFFER, 0, size, data);
}

void Renderer::setClearColor(const glm::vec4& color) const
{
    glClearColor(color.r, color.g, color.b, color.a);
}

void Renderer::clear() const
{
    glClear(GL_COLOR_BUFFER_BIT);
}

void Renderer::setViewport(int width, int height)
{
    glViewport(0, 0, width, height);
}