#include <Renderer/ElementBuffer.h>

ElementBuffer::ElementBuffer(const void* data, GLsizeiptr dataSize)
	: ElementBuffer(data, dataSize, GL_STATIC_DRAW)
{
}

ElementBuffer::ElementBuffer(const void* data, GLsizeiptr dataSize, GLenum usage)
{
	// Generate an element buffer
	glGenBuffers(1, &m_rendererID);

	// Bind to the element buffer and pass in our indices
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_rendererID);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, dataSize, data, usage);

	// Unbind the element buffer
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

	// Store the element count
	m_count = dataSize / sizeof(unsigned int);
}

ElementBuffer::~ElementBuffer()
{
	glDeleteBuffers(1, &m_rendererID);
}

void ElementBuffer::Bind() const
{
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_rendererID);
}

void ElementBuffer::Unbind() const
{
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}
