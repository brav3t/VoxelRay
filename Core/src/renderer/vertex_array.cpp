
#include "vertex_array.h"

#include "vertex_buffer_layout.h"
#include "gl_utils.h"

VertexArray::VertexArray()
{
	GLCall(glGenVertexArrays(1, &mVAO));
}

VertexArray::~VertexArray()
{
	GLCall(glDeleteVertexArrays(1, &mVAO));
}

void VertexArray::bind() const
{
	GLCall(glBindVertexArray(mVAO));
}

void VertexArray::unbind() const
{
	GLCall(glBindVertexArray(0));
}

void VertexArray::addBuffer(const VertexBuffer& vbo, const VertexBufferLayout& layoutVertex)
{
	// Bind vertex array
	bind();
	// Bind vertex buffer
	vbo.bind();
	mCountVerticles = vbo.getCount();
	// Set vertex layout
	const auto& elements = layoutVertex.getElements();
	unsigned int offset = 0;
	for (unsigned int i = 0; i < elements.size(); ++i)
	{
		const auto& element = elements[i];
		GLCall(glEnableVertexAttribArray(i));
		GLCall(glVertexAttribPointer(i, element.count, element.type, element.normalized, layoutVertex.getStride(), (const void*)offset));
		offset += element.count * VertexBufferElement::getSizeOfType(element.type);
	}
}
