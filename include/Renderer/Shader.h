#pragma once

#include <unordered_map>
#include <string>

class Shader
{
public:
	Shader(const std::string& vertexShaderFilePath, const std::string& fragmentShaderFilePath);
	~Shader();

	void Bind() const;

	void Unbind() const;

	bool SetUniform1f(const char* name, float value);

	bool GetUniform1f(const char* name, float& value);

	bool SetUniform3f(const char* name, float v0, float v1, float v2);

	bool SetUniform4f(const char* name, float v0, float v1, float v2, float v3);

	bool SetUniform1i(const char* name, int value);

	bool SetUniform1b(const char* name, bool value);

	bool SetUniformMatrix4fv(const char* name, float* value);

private:

	int GetUniformLocation(const char* name);

	unsigned int m_shaderProgram;
	std::unordered_map<const char*, int> m_uniformCache;
};