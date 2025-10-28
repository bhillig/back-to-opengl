#pragma once

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <Core/EventDispatcher.h>

#include <Renderer/Shader.h>
#include <Renderer/Texture.h>
#include <Renderer/VertexArray.h>

#include <memory>

class Scene;

class Window
{
public:
	Window(const char* name, int width, int height);
	~Window();

	void InitScene();

	void Run();

	GLFWwindow* GetGLFWWindow() const { return m_window; }

	EventDispatcher& GetEventDispatcher() { return m_eventDispatcher; }

	float GetWidth() const { return m_width; }

	float GetHeght() const { return m_height; }

	float m_depth = -3.0f;

private:

	void ToggleInputMode();

	void ConstructGUI();

	// Callbacks
	static void KeyCallback(GLFWwindow* window, int key, int scancode, int action, int mods);
	static void CursorPosCallback(GLFWwindow* window, double xPos, double yPos);
	static void FramebufferSizeCallback(GLFWwindow* window, int width, int height);

private:
	GLFWwindow* m_window;
	float m_width;
	float m_height;

	EventDispatcher m_eventDispatcher;
	
	float m_deltaTime;
	float m_lastTime;

	std::unique_ptr<Scene> m_scene;

};