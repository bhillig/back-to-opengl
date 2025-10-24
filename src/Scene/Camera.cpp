#include <Scene/Camera.h>

#include <GLFW/glfw3.h>

#include <algorithm>

Camera::Camera(const glm::vec3& pos, const glm::vec3& forward, float fov, EventDispatcher& eventDispatcher)
	: m_position(pos)
	, m_forward(forward)
	, m_rotation(glm::vec3(0.f, -90.f, 0.f))
	, m_fov(fov)
	, m_mouseHorizontalSensitivity(0.1f)
	, m_mouseVerticalSensitivity(0.1f)
	, m_forwardPressed(false)
	, m_backwardPressed(false)
	, m_leftPressed(false)
	, m_rightPressed(false)
{
	eventDispatcher.Subscribe(EventType::MouseMove, [this](const Event& event) {
		OnMouseMove(event.x, event.y);
	});

	eventDispatcher.Subscribe(EventType::KeyPressed, [this](const Event& event) {
		OnKeyPressed(event.key);
	});

	eventDispatcher.Subscribe(EventType::KeyReleased, [this](const Event& event) {
		OnKeyReleased(event.key);
	});
}

void Camera::Update(float deltaTime)
{
	// Update movement
	const glm::vec3 walkDirection = glm::normalize(glm::vec3(m_forward.x, 0.0f, m_forward.z));
	const float movementSpeed = 5.f;
	const glm::vec3 upVector = glm::vec3(0.f, 1.0f, 0.f);

	if (m_forwardPressed)
	{
		m_position += movementSpeed * walkDirection * deltaTime;
	}
	if (m_backwardPressed)
	{
		m_position -= movementSpeed * walkDirection * deltaTime;
	}
	if (m_leftPressed)
	{
		m_position -= glm::normalize(glm::cross(walkDirection, upVector)) * movementSpeed * deltaTime;
	}
	if (m_rightPressed)
	{
		m_position += glm::normalize(glm::cross(walkDirection, upVector)) * movementSpeed * deltaTime;
	}
}

void Camera::OnMouseMove(double xPos, double yPos)
{
	static double lastX;
	static double lastY;

	static bool firstEvent = true;
	if (firstEvent)
	{
		lastX = xPos;
		lastY = yPos;
		firstEvent = false;
	}

	const float deltaX = xPos - lastX;
	const float deltaY = yPos - lastY;

	float& pitch = m_rotation[0];
	float& yaw = m_rotation[1];

	// Update yaw
	yaw += deltaX * m_mouseHorizontalSensitivity;

	// Update pitch
	const float minPitch = -89.f;
	const float maxPitch = 89.f;
	pitch = std::clamp(pitch - deltaY * m_mouseVerticalSensitivity, minPitch, maxPitch); // reversed since y-coordinates range from bottom to top

	// Update forward
	m_forward.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
	m_forward.y = sin(glm::radians(pitch));
	m_forward.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));

	lastX = xPos;
	lastY = yPos;
}

void Camera::OnKeyPressed(int key)
{
	if (key == GLFW_KEY_W)
	{
		m_forwardPressed = true;
	}
	else if (key == GLFW_KEY_S)
	{
		m_backwardPressed = true;
	}
	else if (key == GLFW_KEY_A)
	{
		m_leftPressed = true;
	}
	else if (key == GLFW_KEY_D)
	{
		m_rightPressed = true;
	}
}

void Camera::OnKeyReleased(int key)
{
	if (key == GLFW_KEY_W)
	{
		m_forwardPressed = false;
	}
	else if (key == GLFW_KEY_S)
	{
		m_backwardPressed = false;
	}
	else if (key == GLFW_KEY_A)
	{
		m_leftPressed = false;
	}
	else if (key == GLFW_KEY_D)
	{
		m_rightPressed = false;
	}
}
