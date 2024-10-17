#pragma once

#include "vertex_buffer.h"

class VertexBufferLayout;

class VertexArray {
public:
	VertexArray();
	~VertexArray();

	void bind() const;
	void unbind() const;

	void addBuffer(const VertexBuffer&, const VertexBufferLayout&);
	inline unsigned int getCount() const { return mCountVerticles; }

private:
	unsigned int mVAO;
	unsigned int mCountVerticles;
};
