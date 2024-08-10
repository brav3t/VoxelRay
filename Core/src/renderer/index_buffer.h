#pragma once

class IndexBuffer {
private:
	unsigned int mIBO;
	unsigned int mCountIndices;

public:
	IndexBuffer(const unsigned int* dataIndices, unsigned int countIndices);
	~IndexBuffer();

	void bind() const;
	void unbind() const;

	inline unsigned int getCount() const { return mCountIndices; }
};
