#include "VertexArray.h"
#include "VertexBuffer.h"

#include <glad/glad.h>

VertexArray::VertexArray()
{
    glGenVertexArrays(1, &ID_);
    glBindVertexArray(ID_);
}

VertexArray::~VertexArray()
{
    glBindVertexArray(0);
    glDeleteVertexArrays(1, &ID_);
}

void VertexArray::setLayout(const VertexLayout& layout, const VertexBuffer& vertexBuffer)
{
    layout_ = layout;

    bind();
    vertexBuffer.bind();

    const auto& attributes{ layout_.getElements() };
    for (std::size_t i = 0; i < attributes.size(); ++i)
    {
        const VertexAttribute& attrib{ attributes[i] };
        glVertexAttribPointer(static_cast<unsigned int>(i), // vertex attribute index
                              attrib.count_,                // number of components per attribute
                              attrib.type_,                 // data type of each component
                              attrib.normalized_,           // normalized or not
                              layout_.getStride(),          // stride
                              (void*)attrib.offset_);       // offest from first vertex attribute
        glEnableVertexAttribArray(static_cast<unsigned int>(i));
    }
}

void VertexArray::bind() const
{
    glBindVertexArray(ID_);
}

void VertexArray::unbind() const
{
    glBindVertexArray(0);
}