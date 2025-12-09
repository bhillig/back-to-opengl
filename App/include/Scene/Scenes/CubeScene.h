#pragma once

#include <Scene/Scene.h>

#include <CameraController.h>

#include <Renderer/Shader.h>
#include <Renderer/Texture.h>
#include <Renderer/VertexArray.h>

#include <memory>

class Camera;

class CubeScene final : public Scene
{
public:

	CubeScene();
	void ConstructGUI() override;

protected:

	void Update(float deltaTime) override;

	void Render() override;

protected:

	bool OnMouseMove(double xPos, double yPos) override;
	bool OnKeyPressed(int key) override;
	bool OnKeyReleased(int key) override;

private:

	float m_value = 0.0f;

	std::unique_ptr<Camera> m_camera;
	std::unique_ptr<CameraController> m_cameraController;

	std::unique_ptr<VertexArray> m_vao;

	std::unique_ptr<Shader> m_shader;

	std::unique_ptr<Texture> m_texture;
	std::unique_ptr<Texture> m_texture2;
};