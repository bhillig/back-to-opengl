#pragma once

#include <Core/EventDispatcher.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

class Camera
{
public:
	Camera(const glm::vec3& pos, const glm::vec3& forward, float fov, EventDispatcher& eventDispatcher);

	const glm::vec3& position() const { return m_position; }
	const glm::vec3& forward() const { return m_forward; }
	const glm::vec3& rotation() const { return m_rotation; }
	float fov() const { return m_fov; }

	void Update(float deltaTime);

	void EnableInput(bool enabled);

	void SetFOV(float fov);

private:

	void OnMouseMove(double xPos, double yPos);

	void OnKeyPressed(int key);

	void OnKeyReleased(int key);

private:
	glm::vec3 m_position;
	glm::vec3 m_forward;
	glm::vec3 m_rotation;
	float m_fov;

	float m_mouseHorizontalSensitivity;
	float m_mouseVerticalSensitivity;

	// TODO: Temporary hack
	bool m_forwardPressed;
	bool m_backwardPressed;
	bool m_leftPressed;
	bool m_rightPressed;
	bool m_inputEnabled;
};