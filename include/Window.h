#pragma once
#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <Renderer/Shader.h>
#include <Renderer/Texture.h>
#include <Renderer/VertexArray.h>

#include <memory>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

class Window
{
public:
	Window(const char* name, int width, int height);
	~Window();

	void InitScene();

	void Run();

	GLFWwindow* GetGLFWWindow() const { return m_window; }

	float m_mixAmount = 1.0f;
	float m_depth = -3.0f;

	glm::vec3 m_cameraPos;
	glm::vec3 m_cameraFront;
	glm::vec3 m_cameraUp;

private:

	void ProcessInput();

	// Called by CursorPosCallback
	void OnMouseMove(double xPos, double yPos);

	// Called by CursorPosCallback
	void OnMouseScroll(double xOffset, double yOffset);

private:

	// Callbacks
	static void KeyCallback(GLFWwindow* window, int key, int scancode, int action, int mods);
	static void CursorPosCallback(GLFWwindow* window, double xPos, double yPos);
	static void FramebufferSizeCallback(GLFWwindow* window, int width, int height);
	static void ScrollCallback(GLFWwindow* window, double xOffset, double yOffset);

private:
	GLFWwindow* m_window;
	float m_width;
	float m_height;
	
	float m_deltaTime;

	double m_lastMouseX;
	double m_lastMouseY;

	float m_cameraFOV;
	float m_cameraPitch;
	float m_cameraYaw;

	std::unique_ptr<VertexArray> m_vao;

	std::unique_ptr<Shader> m_shader;

	std::unique_ptr<Texture> m_texture;
	std::unique_ptr<Texture> m_texture2;

};