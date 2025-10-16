#pragma once
#include <string>

#include <glad/glad.h>

class Texture
{
public:
	Texture(const std::string& filepath, unsigned int slot = 0);
	~Texture();

	void Bind() const;

	void Unbind() const;

private:
	unsigned int m_rendererID;
	unsigned int m_slot;
	int m_width;
	int m_height;
	int m_numberOfColorChannels;
	unsigned char* m_buffer;
};