#pragma once

#include <Scene/Camera.h>

#include <memory>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

class Window;

class CameraController
{
public:
	CameraController(Window* window, Camera& camera);

	bool inputEnabled() const { return m_inputEnabled; }

	void Update(float deltaTime);

	void EnableInput(bool enabled);

protected:

	void OnMouseMove(double xPos, double yPos);

	void OnKeyPressed(int key);

	void OnKeyReleased(int key);

private:
	Window* m_window;
	Camera& m_camera;

	float m_mouseHorizontalSensitivity;
	float m_mouseVerticalSensitivity;

	bool m_forwardPressed;
	bool m_backwardPressed;
	bool m_leftPressed;
	bool m_rightPressed;
	bool m_inputEnabled;
};