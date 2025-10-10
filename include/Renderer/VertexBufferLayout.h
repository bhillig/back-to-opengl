#pragma once

#include <cassert>
#include <vector>
#include <glad/glad.h>

struct VertexBufferElement
{
	unsigned int type;
	unsigned int count;
	unsigned char normalized;

	static unsigned int GetSizeOfType(unsigned int type)
	{
		switch (type)
		{
			case GL_FLOAT: return sizeof(float);
			case GL_UNSIGNED_INT: return sizeof(unsigned int);
			case GL_UNSIGNED_BYTE: return sizeof(char);
		}
		assert(false && "Attempting to get size of unknown type");
		return 0;
	}
};

class VertexBufferLayout
{
public:
	VertexBufferLayout();
	~VertexBufferLayout();

	template<typename T>
	void Push(unsigned int count)
	{
		assert(false && "Attempted to push an unknown type");
	}

	template<>
	void Push<float>(unsigned int count)
	{
		m_vertexBufferElements.emplace_back(VertexBufferElement(GL_FLOAT, count, GL_FALSE));
		m_stride += count * sizeof(float);
	}

	unsigned int GetStride() const { return m_stride; }

	const std::vector<VertexBufferElement>& GetElements() const { return m_vertexBufferElements; }

private:
	std::vector<VertexBufferElement> m_vertexBufferElements;
	unsigned int m_stride;
};