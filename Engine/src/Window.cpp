#include <Window.h>

#include <iostream>

#include <InputEvents.h>

#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>

namespace Core
{

Window::Window(const WindowSpecification& windowSpec)
	: m_windowSpecification(windowSpec) , m_handle(nullptr)
{
}

bool Window::Create()
{
	// Set window hints (OpenGL version and profile)
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE); // Required for macOS
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	// Create window 
	m_handle = glfwCreateWindow(m_windowSpecification.Width, m_windowSpecification.Height, m_windowSpecification.Title.c_str(), nullptr, nullptr);
	if (!m_handle)
	{
		std::cerr << "Failed to create GLFW window!\n";
		return false;
	}

	// Update context
	glfwMakeContextCurrent(m_handle);

	// Allows callbacks to access this application class
	glfwSetWindowUserPointer(m_handle, this);

	// Set GLFW callbacks
	glfwSetKeyCallback(m_handle, [](GLFWwindow* window, int key, int scancode, int action, int mods) {
		Window* self = static_cast<Window*>(glfwGetWindowUserPointer(window));
		if (!self) return;

		if (action == GLFW_PRESS || action == GLFW_REPEAT)
		{
			KeyPressedEvent event(key, action == GLFW_REPEAT);
			self->m_windowSpecification.Callback(event);
		}
		else if (action == GLFW_RELEASE)
		{
			KeyReleasedEvent event(key);
			self->m_windowSpecification.Callback(event);
		}
	});

	glfwSetCursorPosCallback(m_handle, [](GLFWwindow* window, double xPos, double yPos) {
		Window* self = static_cast<Window*>(glfwGetWindowUserPointer(window));
		if (!self) return;
		
		MouseMovedEvent event(xPos, yPos);
		self->m_windowSpecification.Callback(event);
	});

	glfwSetFramebufferSizeCallback(m_handle, [](GLFWwindow* window, int width, int height) {
		Window* self = static_cast<Window*>(glfwGetWindowUserPointer(window));
		if (!self) return;

		// TODO: Make this a WindowResize Event and raise it here
		self->m_windowSpecification.Width = width;
		self->m_windowSpecification.Height = height;
		glViewport(0, 0, width, height);
	});

	// Initialize GLAD
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
		std::cerr << "Failed to initialize GLAD" << std::endl;
		return false;
	}

	// Capture mouse cursor
	glfwSetInputMode(m_handle, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

	// Initialize viewport
	glViewport(0, 0, m_windowSpecification.Width, m_windowSpecification.Height);

	// Enable depth testing
	glEnable(GL_DEPTH_TEST);

	return true;
}

void Window::SetupGUIForFrame()
{
	ImGui_ImplOpenGL3_NewFrame();
	ImGui_ImplGlfw_NewFrame();
	ImGui::NewFrame();
}

void Window::RenderGUI()
{
	ImGui::Render();
	ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}

void Window::Update()
{
	// Swap front and back buffers
	glfwSwapBuffers(m_handle);
}

Window::~Window()
{
	// Shutdown ImGui
	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplGlfw_Shutdown();
	ImGui::DestroyContext();

	// Shutdown GLFW window
	glfwDestroyWindow(m_handle);
}

} // namespace Core
