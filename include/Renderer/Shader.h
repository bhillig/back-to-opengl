#pragma once

class Shader
{
public:
	Shader(const char* vertexShaderSource, const char* fragmentShaderSource);
	~Shader();

	void Bind() const;

	void Unbind() const;

private:
	unsigned int m_shaderProgram;
};