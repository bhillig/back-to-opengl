#pragma once

#include <Camera.h>
#include <EventDispatcher.h>

#include <memory>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

class Window;

class CameraController
{
public:
	CameraController(Camera& camera);
	~CameraController();

	bool inputEnabled() const { return m_inputEnabled; }

	void Update(float deltaTime);

	void EnableInput(bool enabled);

protected:

	void OnMouseMove(double xPos, double yPos);

	void OnKeyPressed(int key);

	void OnKeyReleased(int key);

private:
	Camera& m_camera;

	float m_mouseHorizontalSensitivity;
	float m_mouseVerticalSensitivity;

	bool m_forwardPressed;
	bool m_backwardPressed;
	bool m_leftPressed;
	bool m_rightPressed;

	bool m_inputEnabled;
	bool m_firstMouseMoveEvent;

	EventHandle m_mouseEventHandler;
	EventHandle m_keyPressedEventHandler;
	EventHandle m_keyReleasedEventHandler;
};