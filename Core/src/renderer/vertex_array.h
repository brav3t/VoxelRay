#pragma once

#include "vertex_buffer.h"

class VertexBufferLayout;

class VertexArray {
private:
	unsigned int mVAO;

public:
	VertexArray();
	~VertexArray();

	void bind() const;
	void unbind() const;

	void addBuffer(const VertexBuffer& vbo, const VertexBufferLayout& layout);
};
