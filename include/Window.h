#pragma once

#include <GLFW/glfw3.h>

class Window
{
public:
	Window(const char* name, int width, int height);
	~Window();

	void Run();

	GLFWwindow* GetGLFWWindow() const { return m_window; }

private:
	GLFWwindow* m_window;
};