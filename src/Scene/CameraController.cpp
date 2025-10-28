#include <Scene/CameraController.h>

#include <Window/Window.h>

#include <algorithm>

CameraController::CameraController(Window* window, Camera& camera)
	: m_camera(camera)
	, m_window(window)
	, m_mouseHorizontalSensitivity(0.1f)
	, m_mouseVerticalSensitivity(0.1f)
	, m_forwardPressed(false)
	, m_backwardPressed(false)
	, m_leftPressed(false)
	, m_rightPressed(false)
	, m_inputEnabled(true)
{
	m_window->GetEventDispatcher().Subscribe(EventType::MouseMove, [this](const Event& event) {
		OnMouseMove(event.x, event.y);
	});
	m_window->GetEventDispatcher().Subscribe(EventType::KeyPressed, [this](const Event& event) {
		OnKeyPressed(event.key);
	});
	m_window->GetEventDispatcher().Subscribe(EventType::KeyReleased, [this](const Event& event) {
		OnKeyReleased(event.key);
	});
}

void CameraController::Update(float deltaTime)
{
	if (!m_inputEnabled) return;

	const glm::vec3 cameraForward = m_camera.forward();
	const glm::vec3 walkDirection = glm::normalize(glm::vec3(cameraForward.x, 0.0f, cameraForward.z));
	const float movementSpeed = 5.f;
	const glm::vec3 upVector = glm::vec3(0.f, 1.0f, 0.f);

	glm::vec3 moveDelta(0.0f);
	if (m_forwardPressed)
	{
		moveDelta = movementSpeed * walkDirection * deltaTime;
	}
	if (m_backwardPressed)
	{
		moveDelta = -(movementSpeed * walkDirection * deltaTime);
	}
	if (m_leftPressed)
	{
		moveDelta = -(glm::normalize(glm::cross(walkDirection, upVector)) * movementSpeed * deltaTime);
	}
	if (m_rightPressed)
	{
		moveDelta = glm::normalize(glm::cross(walkDirection, upVector)) * movementSpeed * deltaTime;
	}

	m_camera.Move(moveDelta);
}

void CameraController::EnableInput(bool enabled)
{
	m_inputEnabled = enabled;
}

void CameraController::OnMouseMove(double xPos, double yPos)
{
	if (!m_inputEnabled) return;

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

	const float yawOffset = deltaX * m_mouseHorizontalSensitivity;
	const float pitchOffset = -deltaY * m_mouseVerticalSensitivity;

	m_camera.Rotate(pitchOffset, yawOffset, 0.f);

	lastX = xPos;
	lastY = yPos;
}

void CameraController::OnKeyPressed(int key)
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

void CameraController::OnKeyReleased(int key)
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
