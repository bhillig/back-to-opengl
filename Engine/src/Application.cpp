#include <Application.h>

#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>

#include <algorithm>
#include <iostream>

namespace Core
{

Application* Application::s_instance = nullptr;

Application::Application(const ApplicationSpecification& appSpec)
	: m_appSpec(appSpec)
	, m_deltaTime(0.f)
	, m_lastTime(0.f)
{
	assert(!s_instance && "Application is already instantiated!");
	s_instance = this;

	if (m_appSpec.WindowSpec.Title.empty())
	{
		m_appSpec.WindowSpec.Title = m_appSpec.Name;
	}

	m_window = std::make_unique<Window>(m_appSpec.WindowSpec);
}

bool Application::Init()
{
	// TODO: Add glfw error callback
	if (!glfwInit())
	{
		std::cerr << "Application failed to initialize: Failed to init glfw!\n";
		return false;
	}

	if (!m_window->Create())
	{
		std::cerr << "Application failed to initialize: Failed to create window!\n";
		return false;
	}

	// TODO: Have the window handle the callbacks and then pass it to the application

	// Allows callbacks to access this application class
	glfwSetWindowUserPointer(m_window->GetHandle(), this);

	// Set GLFW callbacks
	glfwSetKeyCallback(m_window->GetHandle(), Application::KeyCallback);
	glfwSetCursorPosCallback(m_window->GetHandle(), Application::CursorPosCallback);
	glfwSetFramebufferSizeCallback(m_window->GetHandle(), Application::FramebufferSizeCallback);

	// Init ImGui
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO(); (void)io;
	ImGui::StyleColorsDark();

	// Setup backends
	ImGui_ImplGlfw_InitForOpenGL(m_window->GetHandle(), true);
	ImGui_ImplOpenGL3_Init("#version 460");
	return true;
}

void Application::Run()
{
	m_lastTime = glfwGetTime();

	while (!m_window->ShouldClose())
	{
		// Calculate deltaTime
		const float currentTime = glfwGetTime();
		m_deltaTime = currentTime - m_lastTime;
		m_lastTime = currentTime;

		// Process events
		glfwPollEvents();

		m_window->SetupGUIForFrame();

		// Update layers
		for (const auto& layer : m_layerStack)
		{
			layer->OnUpdate(m_deltaTime);
		}

		// Render layers
		for (const auto& layer : m_layerStack)
		{
			layer->OnRender();
		}

		m_window->RenderGUI();
		m_window->Update();
	}
}

GLFWwindow* Application::GetGLFWWindow() const
{
	assert(m_window && "Window not initialized!");
	return m_window->GetHandle();
}

void Application::ToggleInputMode()
{
	const int cursorMode = glfwGetInputMode(m_window->GetHandle(), GLFW_CURSOR);
	switch (cursorMode)
	{
	case GLFW_CURSOR_DISABLED:
		glfwSetInputMode(m_window->GetHandle(), GLFW_CURSOR, GLFW_CURSOR_NORMAL);
		break;
	case GLFW_CURSOR_NORMAL:
		glfwSetInputMode(m_window->GetHandle(), GLFW_CURSOR, GLFW_CURSOR_DISABLED);
		break;
	default:
		break;
	}
}

void Application::KeyCallback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	// Retrieve the pointer to the Window instance
	Application* self = static_cast<Application*>(glfwGetWindowUserPointer(window));
	if (!self) return;

	// TODO: Implement an input layer system to properly handle ImGui + 3D scene mouse events
	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS) {
		self->ToggleInputMode();
	}

	if (action == GLFW_PRESS)
	{
		const Event e(EventType::KeyPressed, 0.0, 0.0, key);
		self->m_eventDispatcher.Dispatch(e);
	}
	else if (action == GLFW_RELEASE)
	{
		const Event e(EventType::KeyReleased, 0.0, 0.0, key);
		self->m_eventDispatcher.Dispatch(e);
	}
}

void Application::CursorPosCallback(GLFWwindow* window, double xPos, double yPos)
{
	Application* self = static_cast<Application*>(glfwGetWindowUserPointer(window));
	assert(self);

	const Event e(EventType::MouseMove, xPos, yPos);
	self->m_eventDispatcher.Dispatch(e);
}

void Application::FramebufferSizeCallback(GLFWwindow* window, int width, int height)
{
	Application* self = static_cast<Application*>(glfwGetWindowUserPointer(window));
	assert(self);

	self->m_appSpec.WindowSpec.Width = width;
	self->m_appSpec.WindowSpec.Height = height;
	glViewport(0, 0, width, height);
}

Application::~Application()
{
	s_instance = nullptr;
	m_layerStack.clear();
}

} // namespace Core