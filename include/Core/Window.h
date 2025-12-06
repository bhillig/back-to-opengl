#pragma once

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <string>

namespace Core
{

struct WindowSpecification
{
	std::string Title;
	int Width = 1920;
	int Height = 1080;
};

class Window
{
public:
	Window(const WindowSpecification& windowSpec = WindowSpecification());
	~Window();

	bool Create();

	void SetupGUIForFrame();

	void RenderGUI();

	void Update();

	int GetWidth() const { return m_windowSpecification.Width; }

	int GetHeight() const { return m_windowSpecification.Height; }

	bool ShouldClose() const { return glfwWindowShouldClose(m_handle); }

	GLFWwindow* GetHandle() const { return m_handle; }

private:
	WindowSpecification m_windowSpecification;
	GLFWwindow* m_handle;
};

} // namespace Core