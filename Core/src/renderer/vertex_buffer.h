
#pragma once

class VertexBuffer
{
public:
	VertexBuffer(const void* dataVertices, unsigned int countVerticles);
	~VertexBuffer();

	void bind() const;
	void unbind() const;

	inline unsigned int getCount() const { return mCountVerticles; }

private:
	unsigned int mVBO;
	unsigned int mCountVerticles;
};
