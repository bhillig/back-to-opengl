#pragma once

#include <Scene/Scene.h>

#include <CameraController.h>

#include <Renderer/Shader.h>
#include <Renderer/Texture.h>
#include <Renderer/VertexArray.h>

#include <memory>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

class Camera;

class LightingDemoScene final : public Scene
{
public:

	LightingDemoScene();

	void ConstructGUI() override;

protected:

	void Update(float deltaTime) override;

	void Render() override;

protected:

	void OnLoseFocus() override;
	bool OnMouseMove(double xPos, double yPos) override;
	bool OnKeyPressed(int key) override;
	bool OnKeyReleased(int key) override;

private:

	glm::vec3 m_lightingPosition;
	glm::vec3 m_objectPosition;

	glm::vec3 m_lightSourceColor;
	glm::vec3 m_objectColor;

	float m_value;

	std::unique_ptr<Camera> m_camera;
	std::unique_ptr<CameraController> m_cameraController;

	std::unique_ptr<VertexArray> m_vao;
	std::unique_ptr<VertexArray> m_lightSourceVAO;

	std::unique_ptr<Shader> m_lightSourceShader;
	std::unique_ptr<Shader> m_colorFromLightSourceShader;

	std::unique_ptr<Texture> m_crateTexture;
	std::unique_ptr<Texture> m_crateSpecularTexture;

	bool m_flashlightOn;
};