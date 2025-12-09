#pragma once

#include <Scene/Scene.h>

#include <Renderer/Shader.h>
#include <Renderer/Texture.h>
#include <Renderer/VertexArray.h>

#include <memory>

class Camera;
class CameraController;

class CubeScene final : public Scene
{
public:

	CubeScene();
	~CubeScene() override;

	void OnLoad() override;

	void OnUnload() override;

	void ConstructGUI() override;

protected:

	void Update(float deltaTime) override;

	void Render() override;

protected:

	void OnKeyPressed(int key) override;

private:

	float m_value = 0.0f;

	std::unique_ptr<Camera> m_camera;
	std::unique_ptr<CameraController> m_cameraController;

	std::unique_ptr<VertexArray> m_vao;

	std::unique_ptr<Shader> m_shader;

	std::unique_ptr<Texture> m_texture;
	std::unique_ptr<Texture> m_texture2;
};