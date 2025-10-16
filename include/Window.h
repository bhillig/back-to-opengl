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

	float mixAmount = 0.2f;

private:
	GLFWwindow* m_window;

	std::unique_ptr<VertexArray> m_vao;
	std::unique_ptr<VertexArray> m_vao2;

	std::unique_ptr<Shader> m_shader;
	std::unique_ptr<Shader> m_shader2;

	std::unique_ptr<Texture> m_texture;
	std::unique_ptr<Texture> m_texture2;
};