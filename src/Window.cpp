#include "Window.h"

#include <Renderer/ElementBuffer.h>
#include <Renderer/Shader.h>
#include <Renderer/VertexArray.h>
#include <Renderer/VertexBuffer.h>
#include <Renderer/VertexBufferLayout.h>

#include <stb_image.h>

#include <iostream>

const std::string kPositionVS = SHADER_DIR + std::string("/position.vertex.glsl");
const std::string kPositionAndColorVS = SHADER_DIR + std::string("/positionAndColor.vertex.glsl");
const std::string kPositionColorAndTexCoordVS = SHADER_DIR + std::string("/positionColorAndTexCoords.vertex.glsl");

const std::string kColorFromVertexFS = SHADER_DIR + std::string("/colorFromVertex.fragment.glsl");
const std::string kColorFromTextureFS = SHADER_DIR + std::string("/colorFromTexture.fragment.glsl");
const std::string kYellowFS = SHADER_DIR + std::string("/yellow.fragment.glsl");

const std::string kContainerTexture = TEXTURE_DIR + std::string("/container.jpg");
const std::string kWallTexture = TEXTURE_DIR + std::string("/wall.jpg");


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
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE); // Required for macOS
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
	// Vertices of rectangle
	const float vertices[] = {
		// Position			// Color			// Texture Coordinates
		-0.5f, -0.5f, 0.f,	1.0f, 0.0f, 0.0f,	0.0f, 0.0f, // Bottom-left
		0.5f, -0.5f, 0.f,	0.0f, 1.0f, 0.0f,	1.0f, 0.0f, // Bottom-right
		0.5f, 0.5f, 0.0f,	0.0f, 0.0f, 1.0f,	1.0f, 1.0f, // Top-right
		-0.5f, 0.5f, 0.0f,	0.0f, 0.0f, 1.0f,   0.0f, 1.0f	// Top-left
	};

	// Vertices of triangle two
	const float vertices2[] = {
		0.0f, 0.0f, 0.0f, // Bottom-left
		0.5f, 0.5f, 0.0f, // Top
		1.0f, 0.0f, 0.0f // Bottom-right
	};

	const unsigned int indices[] = {
		0, 1, 2,
		2, 3, 0
	};

	const unsigned int indices2[] = {
		0, 1, 2,
	};

	// Create a vertex array object
	m_vao = std::make_unique<VertexArray>();
	m_vao->Bind();

	// Create a vertex buffer object
	VertexBuffer vertexBuffer(vertices, sizeof(vertices));
	vertexBuffer.Bind();

	// Specify how our vertex data is formatted
	VertexBufferLayout vertexBufferLayout;
	vertexBufferLayout.Push<float>(3); // Position
	vertexBufferLayout.Push<float>(3); // Color
	vertexBufferLayout.Push<float>(2); // Texture Coordinates

	m_vao->Add(vertexBuffer, vertexBufferLayout);

	// Create an element buffer object
	ElementBuffer elementBuffer(indices, sizeof(indices));
	elementBuffer.Bind();

	// Unbind
	glBindVertexArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

	// Create and bind vao2
	m_vao2 = std::make_unique<VertexArray>();
	m_vao2->Bind();

	// Create and bind vbo2
	VertexBuffer vertexBuffer2(vertices2, sizeof(vertices2));
	vertexBuffer2.Bind();

	VertexBufferLayout vertexBufferLayout2;
	vertexBufferLayout2.Push<float>(3); // Position

	m_vao2->Add(vertexBuffer2, vertexBufferLayout2);

	// Create and bind ebo2
	ElementBuffer elementBuffer2(indices2, sizeof(indices2));
	elementBuffer2.Bind();

	// Create orange and yellow shaders
	m_shader = std::make_unique<Shader>(kPositionColorAndTexCoordVS, kColorFromTextureFS);
	m_shader2 = std::make_unique<Shader>(kPositionVS, kYellowFS);

	// Create and bind texture
	glGenTextures(1, &m_textureID);
	glBindTexture(GL_TEXTURE_2D, m_textureID);

	// Set texture parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	// Load texture and generate mipmap
	int width, height, numberOfColorChannels;
	const unsigned char* data = stbi_load(kContainerTexture.c_str(), &width, &height, &numberOfColorChannels, 0);

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
	glGenerateMipmap(GL_TEXTURE_2D);

	// Free image memory now that the texture is set
	stbi_image_free((void*)data);

	// Unbind again
	glBindVertexArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	glBindTexture(GL_TEXTURE_2D, 0);
}

void Window::Run()
{
	while (!glfwWindowShouldClose(m_window))
	{
		// Process events
		glfwPollEvents();

		// Clear screen with the color specified
		glClear(GL_COLOR_BUFFER_BIT);

		// Draw rectangle
		glBindTexture(GL_TEXTURE_2D, m_textureID);
		m_shader->Bind();
		m_vao->Bind();
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
		glBindTexture(GL_TEXTURE_2D, 0);

		// Draw triangle
		m_shader2->Bind();
		m_vao2->Bind();
		glDrawElements(GL_TRIANGLES, 3, GL_UNSIGNED_INT, 0);
		m_shader2->Unbind();
		m_vao2->Unbind();

		// Swap front and back buffers
		glfwSwapBuffers(m_window);
	}
}
