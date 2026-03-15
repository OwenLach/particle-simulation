#include "VertexBuffer.h"

#include <glad/glad.h>

VertexBuffer::VertexBuffer(unsigned int size)
    : ID_{ 0 }
{
    glGenBuffers(1, &ID_);
    glBindBuffer(GL_ARRAY_BUFFER, ID_);
    glBufferData(GL_ARRAY_BUFFER, size, nullptr, GL_DYNAMIC_DRAW);
}

VertexBuffer::~VertexBuffer()
{
    glDeleteBuffers(1, &ID_);
}

void VertexBuffer::bind() const
{
    glBindBuffer(GL_ARRAY_BUFFER, ID_);
}

void VertexBuffer::unbind() const
{
    glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void VertexBuffer::setData(unsigned int size, const void* data)
{
    bind();
    glBufferSubData(GL_ARRAY_BUFFER, 0, size, data);
}
