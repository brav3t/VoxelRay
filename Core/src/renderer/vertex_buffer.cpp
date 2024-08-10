
#include "vertex_buffer.h"

#include "gl_utils.h"

VertexBuffer::VertexBuffer(const void* dataVertices, unsigned int dataSize) {
    GLCall(glGenBuffers(1, &mVBO));
    GLCall(glBindBuffer(GL_ARRAY_BUFFER, mVBO));
    GLCall(glBufferData(GL_ARRAY_BUFFER, dataSize, dataVertices, GL_STATIC_DRAW));
}

VertexBuffer::~VertexBuffer() {
    GLCall(glDeleteBuffers(1, &mVBO));
}

void VertexBuffer::bind() const {
    GLCall(glBindBuffer(GL_ARRAY_BUFFER, mVBO));
}

void VertexBuffer::unbind() const {
    GLCall(glBindBuffer(GL_ARRAY_BUFFER, 0));
}
