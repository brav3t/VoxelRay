#pragma once

#include <glad/glad.h>

class ShaderStorageBuffer
{
public:
    ShaderStorageBuffer();
    ~ShaderStorageBuffer();

    ShaderStorageBuffer(const ShaderStorageBuffer&) = delete;
    ShaderStorageBuffer& operator=(const ShaderStorageBuffer&) = delete;

    void bind(GLuint binding) const;

    void allocate(GLsizeiptr size, const void* data = nullptr, GLenum usage = GL_DYNAMIC_DRAW);

    void update(GLintptr offset, GLsizeiptr size, const void* data) const;

    GLuint id() const { return mID; }
    GLsizeiptr size() const { return mSize; }

private:
    GLuint mID = 0;
    GLsizeiptr mSize = 0;
};
