#pragma once

#include <Camera.h>

#include <memory>

class CameraController
{
public:
	CameraController(Camera& camera);
	~CameraController();

	void Update(float deltaTime);

	void OnMouseMove(double xPos, double yPos);

	void OnKeyPressed(int key);

	void OnKeyReleased(int key);

	void OnLoseControl();

private:
	Camera& m_camera;

	float m_mouseHorizontalSensitivity;
	float m_mouseVerticalSensitivity;

	bool m_forwardPressed;
	bool m_backwardPressed;
	bool m_leftPressed;
	bool m_rightPressed;

	bool m_firstMouseMoveEvent;
};