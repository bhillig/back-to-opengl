#include <Window/Window.h>

#include <Renderer/ElementBuffer.h>
#include <Renderer/Shader.h>
#include <Renderer/VertexArray.h>
#include <Renderer/VertexBuffer.h>
#include <Renderer/VertexBufferLayout.h>

#include <Scene/CubeScene.h>

#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <algorithm>
#include <iostream>


Window::Window(const char* name, int width, int height)
	: m_width(width), m_height(height)
{
	// Set window hints (OpenGL version and profile)
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE); // Required for macOS
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	// Create window with (width, height, name)
	m_window = glfwCreateWindow(m_width, m_height, name, nullptr, nullptr);

	// Update context
	glfwMakeContextCurrent(m_window);

	// Allows callbacks to access this window class
	glfwSetWindowUserPointer(m_window, this);

	// Set GLFW callbacks
	glfwSetKeyCallback(m_window, Window::KeyCallback);
	glfwSetCursorPosCallback(m_window, Window::CursorPosCallback);
	glfwSetFramebufferSizeCallback(m_window, Window::FramebufferSizeCallback);

	// Initialize GLAD
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
		std::cout << "Failed to initialize GLAD" << std::endl;
		return;
	}

	// Capture mouse cursor
	glfwSetInputMode(m_window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

	// Initialize viewport
	glViewport(0, 0, m_width, m_height);

	// Enable depth testing
	glEnable(GL_DEPTH_TEST);
}

Window::~Window()
{
	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplGlfw_Shutdown();
	ImGui::DestroyContext();

	glfwDestroyWindow(m_window);
}

void Window::InitScene()
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
	m_scene = std::make_unique<CubeScene>(this);
	m_scene->OnLoad();
}

void Window::Run()
{
	m_lastTime = glfwGetTime();

	while (!glfwWindowShouldClose(m_window))
	{
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
		m_scene->Simulate(m_deltaTime);

		// Render ImGui
		ImGui::Render();
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

		// Swap front and back buffers
		glfwSwapBuffers(m_window);
	}
}

void Window::ToggleInputMode()
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

void Window::ConstructGUI()
{
	ImGui::Begin("Properties");
	m_scene->ConstructGUI();
	ImGui::End();
}


void Window::KeyCallback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	// Retrieve the pointer to your Window instance
	Window* self = static_cast<Window*>(glfwGetWindowUserPointer(window));
	if (!self) return;

	const float cameraSpeed = 0.05f;

	// Close window
	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS) {
		self->ToggleInputMode();
	}
	// Set background color to red
	else if (key == GLFW_KEY_R && action == GLFW_PRESS) {
		glClearColor(1.0f, 0.0f, 0.0f, 1.0f);
	}
	// Set background color to green
	else if (key == GLFW_KEY_G && action == GLFW_PRESS) {
		glClearColor(0.0f, 1.0f, 0.0f, 1.0f);
	}
	// Set background color to blue
	else if (key == GLFW_KEY_B && action == GLFW_PRESS) {
		glClearColor(0.0f, 0.0f, 1.0f, 1.0f);
	}
	// Set background color to gray
	else if (key == GLFW_KEY_O && action == GLFW_PRESS) {
		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
	}
	else if (key == GLFW_KEY_UP && action == GLFW_PRESS) {
		self->m_depth -= 1.0f;
	}
	else if (key == GLFW_KEY_DOWN && action == GLFW_PRESS) {
		self->m_depth += 1.0f;
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

void Window::CursorPosCallback(GLFWwindow* window, double xPos, double yPos)
{
	Window* self = static_cast<Window*>(glfwGetWindowUserPointer(window));
	assert(self);

	const Event e(EventType::MouseMove, xPos, yPos);
	self->m_eventDispatcher.Dispatch(e);
}

void Window::FramebufferSizeCallback(GLFWwindow* window, int width, int height)
{
	glViewport(0, 0, width, height);
}
