
class VertexBuffer {
public:
	VertexBuffer(const void* dataVertices, unsigned int dataSize);
	~VertexBuffer();

	void bind() const;
	void unbind() const;

private:
	unsigned int mVBO;
};
