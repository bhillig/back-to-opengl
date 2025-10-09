#include "Window.h"

#include <iostream>

const char* vertexShaderSource = "#version 460 core\n"
"layout (location = 0) in vec3 aPos;\n"
"void main()\n"
"{\n"
"   gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
"}\0";

const char* fragmentShaderSource = "#version 460 core\n"
"out vec4 FragColor;\n"
"void main()\n"
"{\n"
"	FragColor = vec4(1.0f, 0.5f, 0.2f, 1.0f);\n"
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
	int  success;
	char infoLog[512];

	// Vertices of a rectangle
	const float vertices[] = {
		-0.5f, -0.5f, 0.f, // Bottom-left
		0.5f, -0.5f, 0.f, // Bottom-right
		0.5f, 0.5f, 0.0f, // Top-right
		-0.5f, 0.5f, 0.f // Top-left
	};

	const unsigned int indices[] = {
		0, 1, 2,
		2, 3, 0
	};

	// Create a vertex array object
	glGenVertexArrays(1, &m_vao);

	// Bind the vertex array object
	glBindVertexArray(m_vao);

	// Create a vertex buffer object
	unsigned int vbo;
	glGenBuffers(1, &vbo);

	// Bind the vertex buffer object
	glBindBuffer(GL_ARRAY_BUFFER, vbo);

	// Load the vbo with vertex data
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	// Create an element buffer object
	unsigned int ebo;
	glGenBuffers(1, &ebo);

	// Bind the element buffer object
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);

	// Load the ebo with indices
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	// Specify how our vertex data is formatted
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	// Create a vertex shader object
	unsigned int vertexShader = glCreateShader(GL_VERTEX_SHADER);

	// Load the vertex shader object with our vertex shader source code
	glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);

	// Compile the vertex shader
	glCompileShader(vertexShader);
	glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
	}

	// Create a fragment shader object
	unsigned int fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);

	// Load the fragment shader object with our fragment shader source code
	glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);

	// Compile the fragment shader
	glCompileShader(fragmentShader);
	glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << std::endl;
	}

	// Create a shader program
	m_shaderProgram = glCreateProgram();

	// Attach vertex and fragment shaders to the program
	glAttachShader(m_shaderProgram, vertexShader);
	glAttachShader(m_shaderProgram, fragmentShader);

	// Link the shader program
	glLinkProgram(m_shaderProgram);
	glGetProgramiv(m_shaderProgram, GL_LINK_STATUS, &success);
	if (!success) {
		glGetProgramInfoLog(m_shaderProgram, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::LINKING::FAILED\n" << infoLog << std::endl;
	}

	glBindVertexArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	glUseProgram(0);

	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);
}

void Window::Run()
{
	while (!glfwWindowShouldClose(m_window))
	{
		// Process events
		glfwPollEvents();

		// Clear screen with the color specified
		glClear(GL_COLOR_BUFFER_BIT);

		// Draw orange rectangle
		glUseProgram(m_shaderProgram);
		glBindVertexArray(m_vao);
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
		glBindVertexArray(0);

		// Swap front and back buffers
		glfwSwapBuffers(m_window);
	}
}
