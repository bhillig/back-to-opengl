#pragma once
#include <string>

#include <glad/glad.h>

class Texture
{
public:
	Texture(const std::string& filepath, int slot = 0);
	~Texture();

	void Bind(int slot = 0) const;

	void Unbind() const;

private:
	unsigned int m_rendererID;
	int m_width;
	int m_height;
	int m_numberOfColorChannels;
	unsigned char* m_buffer;
};