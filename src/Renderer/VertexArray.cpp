#include <Renderer/VertexArray.h>

#include <Renderer/VertexBuffer.h>
#include <Renderer/VertexBufferLayout.h>

#include <glad/glad.h>

VertexArray::VertexArray()
{
	glGenVertexArrays(1, &m_rendererID);
}

VertexArray::~VertexArray()
{
	glDeleteVertexArrays(1, &m_rendererID);
}

void VertexArray::Add(const VertexBuffer& buffer, const VertexBufferLayout& layout)
{
	buffer.Bind();
	const std::vector<VertexBufferElement>& elements = layout.GetElements();
	unsigned int offset = 0;
	for (int i = 0; i < elements.size(); ++i)
	{
		const VertexBufferElement& element = elements[i];
		glVertexAttribPointer(i, element.count, element.type, element.normalized, layout.GetStride(), (void*)offset);
		glEnableVertexAttribArray(i);
		offset += element.count * VertexBufferElement::GetSizeOfType(element.type);
	}
}

void VertexArray::Bind() const
{
	glBindVertexArray(m_rendererID);
}

void VertexArray::Unbind() const
{
	glBindVertexArray(0);
}
