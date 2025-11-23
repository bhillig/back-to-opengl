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

namespace Core
{

	struct WindowSpecification
	{
		std::string Title;
		int Width = 1920;
		int Height = 1080;
	};

	struct ApplicationSpecification
	{
		std::string Name;
		WindowSpecification WindowSpec;
	};

class Application
{
public:
	Application(const ApplicationSpecification& appSpec);
	~Application();

	void InitScene();

	static Application* GetApp()
	{
		assert(s_instance && "Application has not been created!");
		return s_instance;
	}

	void Run();

	GLFWwindow* GetGLFWWindow() const { return m_window; }

	EventDispatcher& GetEventDispatcher() { return m_eventDispatcher; }

	int GetWidth() const { return m_appSpec.WindowSpec.Width; }

	int GetHeight() const { return m_appSpec.WindowSpec.Height; }

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
	ApplicationSpecification m_appSpec;

	EventDispatcher m_eventDispatcher;

	float m_deltaTime;
	float m_lastTime;

	bool m_requestedSceneChange = false;

	std::unique_ptr<Scene> m_scene;

	static Application* s_instance;
};

}