#pragma once
#include <glad/glad.h>
#include <GLFW/glfw3.h>

class Window
{
public:
	Window(const char* name, int width, int height);
	~Window();

	void InitScene();

	void Run();

	GLFWwindow* GetGLFWWindow() const { return m_window; }

private:
	GLFWwindow* m_window;

	unsigned int m_vao;
	unsigned int m_vao2;
	unsigned int m_shaderProgram;
	unsigned int m_shaderProgram2;
};