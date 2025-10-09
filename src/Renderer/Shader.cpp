#include <Renderer/Shader.h>

#include <glad/glad.h>

#include <iostream>

unsigned int CompileShader(const char* shaderSource, int shaderType)
{
	int  success;
	char infoLog[512];

	unsigned int shader = glCreateShader(shaderType);
	glShaderSource(shader, 1, &shaderSource, nullptr);

	// Compile the shader
	glCompileShader(shader);
	glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		// Report shader compilation errors
		const std::string shaderTypeStr = shaderType == GL_VERTEX_SHADER ? "VERTEX" : "FRAGMENT";
		glGetShaderInfoLog(shader, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::" << shaderTypeStr << "::COMPILATION_FAILED\n" << infoLog << std::endl;
	}
	return shader;
}

void LinkShaderProgram(unsigned int shaderProgram)
{
	int  success;
	char infoLog[512];

	// Link the shader program
	glLinkProgram(shaderProgram);
	glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
	if (!success) {
		glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::LINKING::FAILED\n" << infoLog << std::endl;
	}
}

Shader::Shader(const char* vertexShaderSource, const char* fragmentShaderSource)
{
	unsigned int vertexShader = CompileShader(vertexShaderSource, GL_VERTEX_SHADER);
	unsigned int fragmentShader = CompileShader(fragmentShaderSource, GL_FRAGMENT_SHADER);

	m_shaderProgram = glCreateProgram();

	glAttachShader(m_shaderProgram, vertexShader);
	glAttachShader(m_shaderProgram, fragmentShader);

	LinkShaderProgram(m_shaderProgram);

	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);
}

Shader::~Shader()
{
	glDeleteProgram(m_shaderProgram);
}

void Shader::Bind() const
{
	glUseProgram(m_shaderProgram);
}

void Shader::Unbind() const
{
	glUseProgram(0);
}
