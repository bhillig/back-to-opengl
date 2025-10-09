#include "Window.h"

#include <Renderer/ElementBuffer.h>
#include <Renderer/Shader.h>
#include <Renderer/VertexBuffer.h>

#include <iostream>

const char* vertexShaderSource = "#version 460 core\n"
"layout (location = 0) in vec3 aPos;\n"
"void main()\n"
"{\n"
"   gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
"}\0";

const char* orangeFragmentShaderSource = "#version 460 core\n"
"out vec4 FragColor;\n"
"void main()\n"
"{\n"
"	FragColor = vec4(1.0f, 0.5f, 0.2f, 1.0f);\n"
"}\0";

const char* yellowFragmentShaderSource = "#version 460 core\n"
"out vec4 FragColor;\n"
"void main()\n"
"{\n"
"	FragColor = vec4(1.0f, 1.0f, 0.0f, 1.0f);\n"
"}\0";

static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	// Close window
	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS) {
		glfwSetWindowShouldClose(window, GLFW_TRUE);
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
	// Toggle wireframe mode
	else if (key == GLFW_KEY_W && action == GLFW_PRESS) {
		GLint polygonMode[2];
		glGetIntegerv(GL_POLYGON_MODE, polygonMode);
		if (polygonMode[0] == GL_FILL)
		{
			glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
		}
		else
		{
			glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
		}
	}
}

static void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	glViewport(0, 0, width, height);
}

Window::Window(const char* name, int width, int height)
{
	// Set window hints (OpenGL version and profile)
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	// Create window with (width, height, name)
	m_window = glfwCreateWindow(width, height, name, nullptr, nullptr);

	// Set GLFW callbacks
	glfwSetKeyCallback(m_window, key_callback);
	glfwSetFramebufferSizeCallback(m_window, framebuffer_size_callback);

	// Update context
	glfwMakeContextCurrent(m_window);

	// Initialize GLAD
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
		std::cout << "Failed to initialize GLAD" << std::endl;
		return;
	}

	// Initialize viewport
	glViewport(0, 0, width, height);

	// Set initial color (Gray)
	glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
}

Window::~Window()
{
	glfwDestroyWindow(m_window);
}

void Window::InitScene()
{
	// Vertices of triangle one
	const float vertices[] = {
		-1.0f, 0.0f, 0.f, // Bottom-left
		-0.5f, 0.5f, 0.f, // Top
		0.0f, 0.0f, 0.0f, // Bottom-right
	};

	// Vertices of triangle two
	const float vertices2[] = {
		0.0f, 0.0f, 0.0f, // Bottom-left
		0.5f, 0.5f, 0.0f, // Top
		1.0f, 0.0f, 0.0f // Bottom-right
	};

	const unsigned int indices[] = {
		0, 1, 2,
	};

	const unsigned int indices2[] = {
		0, 1, 2,
	};

	// Create a vertex array object
	glGenVertexArrays(1, &m_vao);

	// Bind the vertex array object
	glBindVertexArray(m_vao);

	// Create a vertex buffer object
	VertexBuffer vertexBuffer(vertices, sizeof(vertices));
	vertexBuffer.Bind();

	// Create an element buffer object
	ElementBuffer elementBuffer(indices, sizeof(indices));
	elementBuffer.Bind();

	// Specify how our vertex data is formatted
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	// Unbind
	glBindVertexArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

	// Create and bind vao2
	glGenVertexArrays(1, &m_vao2);
	glBindVertexArray(m_vao2);

	// Create and bind vbo2
	VertexBuffer vertexBuffer2(vertices2, sizeof(vertices2));
	vertexBuffer2.Bind();

	// Create and bind ebo2
	ElementBuffer elementBuffer2(indices2, sizeof(indices2));
	elementBuffer2.Bind();

	// Specify how our vertex data is formatted
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	// Unbind again
	glBindVertexArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

	// Create orange and yellow shaders
	m_orangeShader = std::make_unique<Shader>(vertexShaderSource, orangeFragmentShaderSource);
	m_yellowShader = std::make_unique<Shader>(vertexShaderSource, yellowFragmentShaderSource);
}

void Window::Run()
{
	while (!glfwWindowShouldClose(m_window))
	{
		// Process events
		glfwPollEvents();

		// Clear screen with the color specified
		glClear(GL_COLOR_BUFFER_BIT);

		// Draw two triangles
		if (m_orangeShader)
		{
			m_orangeShader->Bind();
		}
		glBindVertexArray(m_vao);
		glDrawElements(GL_TRIANGLES, 3, GL_UNSIGNED_INT, 0);
		if (m_yellowShader)
		{
			m_yellowShader->Bind();
		}
		glBindVertexArray(m_vao2);
		glDrawElements(GL_TRIANGLES, 3, GL_UNSIGNED_INT, 0);
		glBindVertexArray(0);

		// Swap front and back buffers
		glfwSwapBuffers(m_window);
	}
}
