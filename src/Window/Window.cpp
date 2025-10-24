#include <Window/Window.h>

#include <Renderer/ElementBuffer.h>
#include <Renderer/Shader.h>
#include <Renderer/VertexArray.h>
#include <Renderer/VertexBuffer.h>
#include <Renderer/VertexBufferLayout.h>

#include <stb_image.h>

#include <algorithm>
#include <iostream>

const std::string kPositionVS = SHADER_DIR + std::string("/position.vertex.glsl");
const std::string kPositionAndColorVS = SHADER_DIR + std::string("/positionAndColor.vertex.glsl");
const std::string kPositionAndTexCoordVS = SHADER_DIR + std::string("/positionAndTexCoords.vertex.glsl");
const std::string kPositionColorAndTexCoordVS = SHADER_DIR + std::string("/positionColorAndTexCoords.vertex.glsl");

const std::string kColorFromVertexFS = SHADER_DIR + std::string("/colorFromVertex.fragment.glsl");
const std::string kColorFromTextureFS = SHADER_DIR + std::string("/colorFromTexture.fragment.glsl");
const std::string kColorFromTextureMixFS = SHADER_DIR + std::string("/colorFromTextureMix.fragment.glsl");
const std::string kYellowFS = SHADER_DIR + std::string("/yellow.fragment.glsl");

const std::string kContainerTexture = TEXTURE_DIR + std::string("/container.jpg");
const std::string kWallTexture = TEXTURE_DIR + std::string("/wall.jpg");
const std::string kAwesomeFaceTexture = TEXTURE_DIR + std::string("/awesomeface.png");
const std::string kMorganFreemanTrueTexture = TEXTURE_DIR + std::string("/true.png");

glm::vec3 cubePositions[] = {
glm::vec3(0.0f,  0.0f,  0.0f),
glm::vec3(2.0f,  5.0f, -15.0f),
glm::vec3(-1.5f, -2.2f, -2.5f),
glm::vec3(-3.8f, -2.0f, -12.3f),
glm::vec3(2.4f, -0.4f, -3.5f),
glm::vec3(-1.7f,  3.0f, -7.5f),
glm::vec3(1.3f, -2.0f, -2.5f),
glm::vec3(1.5f,  2.0f, -2.5f),
glm::vec3(1.5f,  0.2f, -1.5f),
glm::vec3(-1.3f,  1.0f, -1.5f)
};

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
	glfwSetScrollCallback(m_window, Window::ScrollCallback);

	// Initialize GLAD
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
		std::cout << "Failed to initialize GLAD" << std::endl;
		return;
	}

	// Capture mouse cursor
	glfwSetInputMode(m_window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

	// Initialize viewport
	glViewport(0, 0, m_width, m_height);

	// Set initial color
	const glm::vec4 grayColor(0.2f, 0.3f, 0.3f, 1.0f);
	glClearColor(grayColor.r, grayColor.g, grayColor.b, grayColor.a);

	// Enable depth testing
	glEnable(GL_DEPTH_TEST);
}

Window::~Window()
{
	glfwDestroyWindow(m_window);
}

void Window::InitScene()
{

	float vertices[] = {
	// Position           // Texture Coordinates
	-0.5f, -0.5f, -0.5f,  0.0f, 0.0f,
	 0.5f, -0.5f, -0.5f,  1.0f, 0.0f,
	 0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
	 0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
	-0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
	-0.5f, -0.5f, -0.5f,  0.0f, 0.0f,

	-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
	 0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
	 0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
	 0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
	-0.5f,  0.5f,  0.5f,  0.0f, 1.0f,
	-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,

	-0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
	-0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
	-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
	-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
	-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
	-0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

	 0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
	 0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
	 0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
	 0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
	 0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
	 0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

	-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
	 0.5f, -0.5f, -0.5f,  1.0f, 1.0f,
	 0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
	 0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
	-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
	-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,

	-0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
	 0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
	 0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
	 0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
	-0.5f,  0.5f,  0.5f,  0.0f, 0.0f,
	-0.5f,  0.5f, -0.5f,  0.0f, 1.0f
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
	vertexBufferLayout.Push<float>(2); // Texture Coordinates

	m_vao->Add(vertexBuffer, vertexBufferLayout);

	// Unbind
	glBindVertexArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	// Create shader
	m_shader = std::make_unique<Shader>(kPositionAndTexCoordVS, kColorFromTextureMixFS);

	// Create textures
	const unsigned int textureSlot = 0;
	const unsigned int texture2Slot = 1;
	m_texture = std::make_unique<Texture>(kContainerTexture, textureSlot);
	m_texture2 = std::make_unique<Texture>(kMorganFreemanTrueTexture, texture2Slot);

	m_shader->SetUniform1i("u_Texture1", textureSlot);
	m_shader->SetUniform1i("u_Texture2", texture2Slot);

	// Init camera
	const glm::vec3 cameraPos(0.0f, 0.0f, 3.0f);
	const glm::vec3 cameraForward(0.0f, 0.0f, -1.0f);
	const float cameraFOV = 45.f;
	m_camera = std::make_unique<Camera>(cameraPos, cameraForward, cameraFOV, m_eventDispatcher);
}

void Window::Run()
{
	float value = 0.0f;
	float rotationSpeed = 50.f;

	float lastTime = glfwGetTime();

	while (!glfwWindowShouldClose(m_window))
	{
		// Process events
		glfwPollEvents();

		// Clear screen
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		const float currentTime = glfwGetTime();
		m_deltaTime = currentTime - lastTime;
		lastTime = currentTime;

		value += m_deltaTime;

		m_camera->Update(m_deltaTime);

		// View
		glm::mat4 view;
		view = glm::lookAt(	glm::vec3(m_camera->position()), // Camera location
							glm::vec3(m_camera->position() + m_camera->forward()), // Target location
							glm::vec3(0.f, 1.0f, 0.f));	// World up direction

		m_shader->SetUniformMatrix4fv("u_View", glm::value_ptr(view));

		// Projection
		glm::mat4 projection = glm::perspective(glm::radians(m_camera->fov()), m_width / m_height, 0.1f, 100.f);
		m_shader->SetUniformMatrix4fv("u_Projection", glm::value_ptr(projection));

		m_shader->SetUniform1f("u_MixAmount", m_mixAmount);

		// Draw rectangle
		m_texture->Bind();
		m_texture2->Bind();
		m_vao->Bind();
		for (int i = 0; i < 10; ++i)
		{
			// Model
			glm::mat4 model(1.f);
			model = glm::translate(model, cubePositions[i]);
			model = glm::rotate(model, value * glm::radians(rotationSpeed), glm::vec3(0.5f, 1.0f, 0.0f));
			m_shader->SetUniformMatrix4fv("u_Model", glm::value_ptr(model));
			m_shader->Bind();
			glDrawArrays(GL_TRIANGLES, 0, 36);
		}
		m_texture->Unbind();

		// Swap front and back buffers
		glfwSwapBuffers(m_window);
	}
}

void Window::OnMouseScroll(double xOffset, double yOffset)
{
	const float minCameraFOV = 1.0f;
	const float maxCameraFOV = 45.0f;
	//m_cameraFOV = std::clamp(m_cameraFOV - static_cast<float>(yOffset), minCameraFOV, maxCameraFOV);
}

void Window::KeyCallback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	// Retrieve the pointer to your Window instance
	Window* self = static_cast<Window*>(glfwGetWindowUserPointer(window));
	if (!self) return;

	const float cameraSpeed = 0.05f;

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

void Window::ScrollCallback(GLFWwindow* window, double xOffset, double yOffset)
{
	Window* self = static_cast<Window*>(glfwGetWindowUserPointer(window));
	assert(self);

	self->OnMouseScroll(xOffset, yOffset);
}
