#pragma once

class VertexBuffer
{
public:
    VertexBuffer(unsigned int size);
    ~VertexBuffer();

    VertexBuffer(const VertexBuffer&) = delete;
    VertexBuffer& operator=(const VertexBuffer&) = delete;

    void bind() const;
    void unbind() const;

    void setData(unsigned int size, const void* data);

    unsigned int getID() const { return ID_; }

private:
    unsigned int ID_;
};
