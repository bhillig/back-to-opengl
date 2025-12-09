#include <Application.h>

#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>

#include <algorithm>
#include <iostream>
#include <ranges>

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

	m_appSpec.WindowSpec.Callback = [this](Event& event) {
		RaiseEvent(event);
	};

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

void Application::RaiseEvent(Event& event)
{
	for (auto& layer : std::views::reverse(m_layerStack))
	{
		layer->OnEvent(event);
	}
}

Application::~Application()
{
	s_instance = nullptr;
}

} // namespace Core