#pragma once

#include "VertexBuffer.h"

#include <glad/glad.h>

#include <cstddef>
#include <vector>
#include <string>
#include <string_view>
#include <iostream>

template <typename T>
GLenum getGLType()
{
    static_assert(false, "unsupported type");
    return 0;
}

template <>
inline GLenum getGLType<float>()
{
    return GL_FLOAT;
}

struct VertexAttribute
{
    std::string name_;
    unsigned int count_;
    GLenum type_;
    size_t offset_;
    bool normalized_;

    VertexAttribute(std::string_view name, unsigned int count, GLenum type, size_t offset, bool normalized)
        : name_{ name },
          count_{ count },
          type_{ type },
          offset_{ offset },
          normalized_{ normalized }
    {
    }
};

class VertexLayout
{
public:
    template <typename T>
    void push(std::string_view name, unsigned int count, unsigned int stride, size_t offset, bool normalized = false)
    {
        stride_ = stride;
        layout_.emplace_back(name, count, getGLType<T>(), offset, normalized);
    }

    unsigned int getStride() const { return stride_; }
    const std::vector<VertexAttribute>& getElements() const { return layout_; }

private:
    std::vector<VertexAttribute> layout_;
    unsigned int stride_ = 0;
};

class VertexArray
{
public:
    VertexArray();
    ~VertexArray();

    VertexArray(const VertexArray&) = delete;
    VertexArray& operator=(const VertexArray&) = delete;

    void setLayout(const VertexLayout& layout, const VertexBuffer& vertexBuffer);

    void bind() const;
    void unbind() const;

    unsigned int getID() const { return ID_; }

private:
    unsigned int ID_ = 0;
    VertexLayout layout_;
};