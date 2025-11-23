#include <Application.h>

#include <Renderer/ElementBuffer.h>
#include <Renderer/Shader.h>
#include <Renderer/VertexArray.h>
#include <Renderer/VertexBuffer.h>
#include <Renderer/VertexBufferLayout.h>

#include <Scene/Scenes/CubeScene.h>
#include <Scene/Scenes/LightingDemoScene.h>
#include <Scene/Scenes/ModelScene.h>

#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

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

	// Set window hints (OpenGL version and profile)
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE); // Required for macOS
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	if (m_appSpec.WindowSpec.Title.empty())
	{
		m_appSpec.WindowSpec.Title = m_appSpec.Name;
	}

	// Create window 
	m_window = glfwCreateWindow(m_appSpec.WindowSpec.Width, m_appSpec.WindowSpec.Height, m_appSpec.WindowSpec.Title.c_str(), nullptr, nullptr);

	// Update context
	glfwMakeContextCurrent(m_window);

	// Allows callbacks to access this window class
	glfwSetWindowUserPointer(m_window, this);

	// Set GLFW callbacks
	glfwSetKeyCallback(m_window, Application::KeyCallback);
	glfwSetCursorPosCallback(m_window, Application::CursorPosCallback);
	glfwSetFramebufferSizeCallback(m_window, Application::FramebufferSizeCallback);

	// Initialize GLAD
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
		std::cout << "Failed to initialize GLAD" << std::endl;
		return;
	}

	// Capture mouse cursor
	glfwSetInputMode(m_window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

	// Initialize viewport
	glViewport(0, 0, m_appSpec.WindowSpec.Width, m_appSpec.WindowSpec.Height);

	// Enable depth testing
	glEnable(GL_DEPTH_TEST);
}

Application::~Application()
{
	// TODO: Move this to the window class
	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplGlfw_Shutdown();
	ImGui::DestroyContext();

	glfwDestroyWindow(m_window);

	m_scene.release();

	s_instance = nullptr;
}

void Application::InitScene()
{
	// Init ImGui
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO(); (void)io;
	ImGui::StyleColorsDark();

	// Setup backends
	ImGui_ImplGlfw_InitForOpenGL(m_window, true);
	ImGui_ImplOpenGL3_Init("#version 460");

	// Set scene
	m_scene = std::make_unique<LightingDemoScene>();
}

void Application::Run()
{
	m_lastTime = glfwGetTime();

	while (!glfwWindowShouldClose(m_window))
	{
		m_requestedSceneChange = false;

		// Calculate deltaTime
		const float currentTime = glfwGetTime();
		m_deltaTime = currentTime - m_lastTime;
		m_lastTime = currentTime;

		// Process events
		glfwPollEvents();

		// Process ImGui
		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplGlfw_NewFrame();
		ImGui::NewFrame();

		// Construct ImGui
		ConstructGUI();

		// Simulate the current scene
		// TODO: Hacky fix for now - should implement a more robust scene manager
		if (!m_requestedSceneChange)
		{
			m_scene->Simulate(m_deltaTime);
		}

		// Render ImGui
		ImGui::Render();
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

		// Swap front and back buffers
		glfwSwapBuffers(m_window);
	}
}

void Application::ToggleInputMode()
{
	const int cursorMode = glfwGetInputMode(m_window, GLFW_CURSOR);
	switch (cursorMode)
	{
	case GLFW_CURSOR_DISABLED:
		glfwSetInputMode(m_window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
		break;
	case GLFW_CURSOR_NORMAL:
		glfwSetInputMode(m_window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
		break;
	default:
		break;
	}
}

void Application::ConstructGUI()
{
	ImGui::Begin("Properties");
	m_scene->ConstructGUI();
	ImGui::Text("Scenes:");
	if (ImGui::Button("Lighting Scene"))
	{
		m_scene = std::make_unique<LightingDemoScene>();
		m_requestedSceneChange = true;
	}
	if (ImGui::Button("Cube Scene"))
	{
		m_scene = std::make_unique<CubeScene>();
		m_requestedSceneChange = true;
	}
	if (ImGui::Button("Model Scene"))
	{
		m_scene = std::make_unique<ModelScene>();
		m_requestedSceneChange = true;
	}
	ImGui::End();
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

} // namespace Core