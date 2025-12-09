#include <Renderer/VertexBuffer.h>

VertexBuffer::VertexBuffer(const void* data, GLsizeiptr dataSize)
	: VertexBuffer(data, dataSize, GL_STATIC_DRAW)
{
}

VertexBuffer::VertexBuffer(const void* data, GLsizeiptr dataSize, GLenum usage)
{
	// Generate a vertex buffer
	glGenBuffers(1, &m_rendererID);

	// Bind to the vertex buffer and pass in our vertices
	glBindBuffer(GL_ARRAY_BUFFER, m_rendererID);
	glBufferData(GL_ARRAY_BUFFER, dataSize, data, usage);

	// Unbind the vertex buffer
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	// Store the element count
	m_count = dataSize / sizeof(float);
}

VertexBuffer::~VertexBuffer()
{
	glDeleteBuffers(1, &m_rendererID);
}

void VertexBuffer::Bind() const
{
	glBindBuffer(GL_ARRAY_BUFFER, m_rendererID);
}

void VertexBuffer::Unbind() const
{
	glBindBuffer(GL_ARRAY_BUFFER, 0);
}
