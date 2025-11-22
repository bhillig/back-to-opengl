#pragma once

#include <glad/glad.h>

class VertexBuffer
{
public:
	VertexBuffer() = default;
	VertexBuffer(const void* data, GLsizeiptr dataSize);
	VertexBuffer(const void* data, GLsizeiptr dataSize, GLenum usage);
	~VertexBuffer();

	void Bind() const;

	void Unbind() const;

	unsigned int Count() const { return m_count; }

private:
	unsigned int m_rendererID;
	unsigned int m_count;
};