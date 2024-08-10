
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

void VertexArray::addBuffer(const VertexBuffer& vbo, const VertexBufferLayout& layout)
{
	// Bind vertex array
	bind();
	// Bind vertex buffer
	vbo.bind();
	// Set layout
	const auto& elements = layout.getElements();
	unsigned int offset = 0;
	for (unsigned int i = 0; i < elements.size(); ++i)
	{
		const auto& element = elements[i];
		GLCall(glEnableVertexAttribArray(i));
		GLCall(glVertexAttribPointer(i, element.count, element.type, element.normalized, layout.getStride(), (const void*)offset));
		offset += element.count * VertexBufferElement::getSizeOfType(element.type);
	}
}
