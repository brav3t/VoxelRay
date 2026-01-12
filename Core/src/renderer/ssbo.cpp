#include "ssbo.h"

#include "gl_utils.h"
#include <glad/glad.h>

ShaderStorageBuffer::ShaderStorageBuffer()
{
    GLCall(glGenBuffers(1, &mID));
}

ShaderStorageBuffer::~ShaderStorageBuffer()
{
    if (mID)
        glDeleteBuffers(1, &mID);
}

void ShaderStorageBuffer::bind(GLuint binding) const
{
    GLCall(glBindBufferBase(GL_SHADER_STORAGE_BUFFER, binding, mID));
}

void ShaderStorageBuffer::allocate(GLsizeiptr size, const void* data, GLenum usage)
{
    mSize = size;
    GLCall(glBindBuffer(GL_SHADER_STORAGE_BUFFER, mID));
    GLCall(glBufferData(GL_SHADER_STORAGE_BUFFER, size, data, usage));
    GLCall(glBindBuffer(GL_SHADER_STORAGE_BUFFER, 0));
}

void ShaderStorageBuffer::update(GLintptr offset, GLsizeiptr size, const void* data) const
{
    GLCall(glBindBuffer(GL_SHADER_STORAGE_BUFFER, mID));
    GLCall(glBufferSubData(GL_SHADER_STORAGE_BUFFER, offset, size, data));
    GLCall(glBindBuffer(GL_SHADER_STORAGE_BUFFER, 0));
}
