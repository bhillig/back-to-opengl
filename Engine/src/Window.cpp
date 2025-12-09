#include <Window.h>

#include <iostream>

#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>

namespace Core
{

Window::Window(const WindowSpecification& windowSpec)
	: m_windowSpecification(windowSpec)
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
