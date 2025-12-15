#pragma once

#include <Scene/Scene.h>

#include <ECS/EntityManager.h>

#include <CameraController.h>

#include <Renderer/Model.h>
#include <Renderer/Shader.h>
#include <Renderer/Texture.h>
#include <Renderer/VertexArray.h>

#include <memory>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

class Camera;

class ModelScene final : public Scene
{
public:

	ModelScene();

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

	std::unique_ptr<Camera> m_camera;
	std::unique_ptr<CameraController> m_cameraController;

	std::unique_ptr<Shader> m_modelShader;

	std::unique_ptr<Model> m_model;

	ECS::EntityManager m_entityManager;
};