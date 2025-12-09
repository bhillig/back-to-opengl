#pragma once

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <Window.h>
#include <EventDispatcher.h>
#include <Layer.h>

#include <Renderer/Shader.h>
#include <Renderer/Texture.h>
#include <Renderer/VertexArray.h>

#include <memory>

#include <cassert>

class Layer;

namespace Core
{
	struct ApplicationSpecification
	{
		std::string Name = "Application";
		WindowSpecification WindowSpec;
	};

class Application
{
public:
	Application(const ApplicationSpecification& appSpec);
	~Application();

	bool Init();

	static Application* GetApp()
	{
		assert(s_instance && "Application has not been created!");
		return s_instance;
	}

	void Run();

	GLFWwindow* GetGLFWWindow() const;

	EventDispatcher& GetEventDispatcher() { return m_eventDispatcher; }

	int GetWidth() const { return m_appSpec.WindowSpec.Width; }

	int GetHeight() const { return m_appSpec.WindowSpec.Height; }

	float m_depth = -3.0f;

	template<typename T>
	void AddLayer()
	{
		m_layerStack.emplace_back(std::make_unique<T>());
	}

private:

	void ToggleInputMode();

	// Callbacks
	static void KeyCallback(GLFWwindow* window, int key, int scancode, int action, int mods);
	static void CursorPosCallback(GLFWwindow* window, double xPos, double yPos);
	static void FramebufferSizeCallback(GLFWwindow* window, int width, int height);

private:
	ApplicationSpecification m_appSpec;
	std::unique_ptr<Window> m_window;

	EventDispatcher m_eventDispatcher;

	float m_deltaTime;
	float m_lastTime;

	bool m_requestedSceneChange = false;

	std::vector<std::unique_ptr<Layer>> m_layerStack;

	static Application* s_instance;
};

}