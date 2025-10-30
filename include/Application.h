#pragma once

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <Core/EventDispatcher.h>

#include <Renderer/Shader.h>
#include <Renderer/Texture.h>
#include <Renderer/VertexArray.h>

#include <memory>

#include <cassert>

class Scene;

class Application
{
public:
	Application(const char* name, int width, int height);
	~Application();

	void InitScene();

	static void Init(const char* name, int width, int height)
	{
		assert(!s_instance && "Application is already created!");
		s_instance = new Application(name, width, height);
	}

	static Application* GetApp()
	{
		assert(s_instance && "Application has not been created!");
		return s_instance;
	}

	static void Shutdown()
	{
		delete s_instance;
		s_instance = nullptr;
	}

	void Run();

	GLFWwindow* GetGLFWWindow() const { return m_window; }

	EventDispatcher& GetEventDispatcher() { return m_eventDispatcher; }

	float GetWidth() const { return m_width; }

	float GetHeight() const { return m_height; }

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

	bool m_requestedSceneChange = false;

	std::unique_ptr<Scene> m_scene;

	static Application* s_instance;
};