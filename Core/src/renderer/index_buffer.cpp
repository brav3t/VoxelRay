
#include "index_buffer.h"

#include "gl_utils.h"

IndexBuffer::IndexBuffer(const unsigned int* dataIndices, unsigned int countIndices)
    : mCountIndices(countIndices) {
    ASSERT(sizeof(unsigned int) == sizeof(GLuint));

    GLCall(glGenBuffers(1, &mIBO));
    GLCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mIBO));
    GLCall(glBufferData(GL_ELEMENT_ARRAY_BUFFER, countIndices * sizeof(unsigned int), dataIndices, GL_STATIC_DRAW));
}

IndexBuffer::~IndexBuffer() {
    GLCall(glDeleteBuffers(1, &mIBO));
}

void IndexBuffer::bind() const {
    GLCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mIBO));
}

void IndexBuffer::unbind() const {
    GLCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0));
}
