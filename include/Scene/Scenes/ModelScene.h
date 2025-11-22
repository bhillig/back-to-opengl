#pragma once

#include <Scene/Scene.h>

#include <Renderer/Model.h>
#include <Renderer/Shader.h>
#include <Renderer/Texture.h>
#include <Renderer/VertexArray.h>

#include <memory>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

class Camera;
class CameraController;

class ModelScene final : public Scene
{
public:

	ModelScene();
	~ModelScene() override;

	void OnLoad() override;

	void OnUnload() override;

	void ConstructGUI() override;

protected:

	void Update(float deltaTime) override;

	void Render() override;

protected:

	void OnKeyPressed(int key) override;

private:

	glm::vec3 m_objectPosition;

	std::unique_ptr<Camera> m_camera;
	std::unique_ptr<CameraController> m_cameraController;

	std::unique_ptr<Shader> m_modelShader;

	std::unique_ptr<Model> m_model;

	float m_scale;
};