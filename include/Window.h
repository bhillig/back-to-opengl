#pragma once
#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <Renderer/Shader.h>
#include <Renderer/Texture.h>
#include <Renderer/VertexArray.h>

#include <memory>

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

private:
	GLFWwindow* m_window;
	float m_width;
	float m_height;

	std::unique_ptr<VertexArray> m_vao;

	std::unique_ptr<Shader> m_shader;

	std::unique_ptr<Texture> m_texture;
	std::unique_ptr<Texture> m_texture2;
};