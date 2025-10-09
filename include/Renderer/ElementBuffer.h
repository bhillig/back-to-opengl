#pragma once

#include <glad/glad.h>

class ElementBuffer
{
public:
	ElementBuffer(const void* data, GLsizeiptr dataSize);
	ElementBuffer(const void* data, GLsizeiptr dataSize, GLenum usage);
	~ElementBuffer();

	void Bind() const;

	void Unbind() const;

	unsigned int Count() const { return m_count; }

private:
	unsigned int m_rendererID;
	unsigned int m_count;
};