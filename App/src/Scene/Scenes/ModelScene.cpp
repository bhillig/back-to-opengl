#include <Scene/Scenes/ModelScene.h>

#include <Application.h>

#include <ECS/Component.h>

#include <Renderer/VertexBuffer.h>
#include <Renderer/VertexBufferLayout.h>
#include <Renderer/ElementBuffer.h>
#include <Renderer/Shader.h>

#include <CameraController.h>

#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>

// Vertex Shaders
const std::string kPositionNormalAndTexCoordVS = SHADER_DIR + std::string("/positionNormalAndTexCoords.vertex.glsl");

// Fragment Shaders
const std::string kColorFromTextureFS = SHADER_DIR + std::string("/colorFromTexture.fragment.glsl");

// Models
const std::string kBackpackModel = MODEL_DIR + std::string("/backpack/backpack.obj");

ModelScene::ModelScene()
	: m_entityManager(10)
{
	// Set model
	m_model = std::make_unique<Model>(kBackpackModel.c_str());

	// Create shaders
	m_modelShader = std::make_unique<Shader>(kPositionNormalAndTexCoordVS, kColorFromTextureFS);

	// Init camera
	const glm::vec3 cameraPos(0.0f, 0.0f, 4.0f);
	const glm::vec3 cameraForward(0.0f, 0.0f, -1.0f);
	const float cameraFOV = 45.f;

	m_camera = std::make_unique<Camera>(cameraPos, 0.0f, -90.f, 0.0f, cameraFOV);
	m_cameraController = std::make_unique<CameraController>(*m_camera);

	// Set initial color
	const glm::vec4 bgColor(0.0f, 0.0f, 0.0f, 1.0f);
	glClearColor(bgColor.r, bgColor.g, bgColor.b, bgColor.a);
}

void ModelScene::ConstructGUI()
{
	Scene::ConstructGUI();

	ImGui::Begin("Level Tree");

	if (ImGui::Button("+ Add Entity"))
	{
		m_entityManager.CreateEntity();
	}


	for (size_t i = 0; i < m_entityManager.GetEntityCount(); ++i)
	{
		ImGui::Separator();

		ECS::Entity& entity = m_entityManager.GetEntity(i);

		// Display Entity Name
		const std::string entityDisplayName = std::string("Entity ") + std::to_string(entity.GetID()) + "\n";
		ImGui::Text(entityDisplayName.c_str());


		// Display Entity Transform
		if (entity.HasComponent<ECS::TransformComponent>())
		{
			ECS::TransformComponent& transformComp = entity.GetComponent<ECS::TransformComponent>();

			float objectPos[3]{ transformComp.x, transformComp.y, transformComp.z };
			const std::string transformLabel = std::string("Transform##") + entityDisplayName;
			if (ImGui::InputFloat3(transformLabel.c_str(), objectPos))
			{
				transformComp.x = static_cast<double>(objectPos[0]);
				transformComp.y = static_cast<double>(objectPos[1]);
				transformComp.z = static_cast<double>(objectPos[2]);
			}
			const std::string removeTransformLabel = std::string("Remove Transform Component##") + entityDisplayName;
			if (ImGui::Button(removeTransformLabel.c_str()))
			{
				entity.RemoveComponent<ECS::TransformComponent>();
			}
		}
		// Prompt to add transform
		else
		{
			const std::string addTransformLabel = std::string("Add Transform Component##") + entityDisplayName;
			if (ImGui::Button(addTransformLabel.c_str()))
			{
				entity.AddComponent<ECS::TransformComponent>();
			}
		}

		// Display Entity Mesh
		if (entity.HasComponent<ECS::MeshComponent>())
		{
			ImGui::Text("Mesh: [ACTIVE]");
			const std::string removeMeshLabel = std::string("Remove Mesh Component##") + entityDisplayName;
			if (ImGui::Button(removeMeshLabel.c_str()))
			{
				entity.RemoveComponent<ECS::MeshComponent>();
			}
		}
		// Prompt to add mesh
		else
		{
			const std::string addMeshLabel = std::string("Add Mesh Component##") + entityDisplayName;
			if (ImGui::Button(addMeshLabel.c_str()))
			{
				entity.AddComponent<ECS::MeshComponent>(m_model.get());
			}
		}
	}

	ImGui::End();

}

void ModelScene::Update(float deltaTime)
{
	m_cameraController->Update(deltaTime);
}

void ModelScene::Render()
{
	// Clear screen
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// View
	glm::mat4 view = m_camera->viewMatrix();
	m_modelShader->SetUniformMatrix4fv("u_View", glm::value_ptr(view));

	// Projection
	glm::mat4 projection = glm::perspective(glm::radians(m_camera->fov()), static_cast<float>(Core::Application::GetApp()->GetWindow().GetWidth()) / Core::Application::GetApp()->GetWindow().GetHeight(), 0.1f, 100.f);
	m_modelShader->SetUniformMatrix4fv("u_Projection", glm::value_ptr(projection));

	for (size_t i = 0; i < m_entityManager.GetEntityCount(); ++i)
	{
		ECS::TransformComponent& transformComp = m_entityManager.GetComponentFromEntity<ECS::TransformComponent>(i);
		ECS::MeshComponent& meshComp = m_entityManager.GetComponentFromEntity<ECS::MeshComponent>(i);
		if (!transformComp.IsActive() || !meshComp.IsActive())
		{
			continue;
		}

		// Set model matrix for model
		glm::mat4 modelObj(1.f);
		modelObj = glm::translate(modelObj, glm::vec3(transformComp.x, transformComp.y, transformComp.z));
		m_modelShader->SetUniformMatrix4fv("u_Model", glm::value_ptr(modelObj));

		// Draw model
		if (Model* model = meshComp.GetModel())
		{
			model->Draw(*m_modelShader);
		}
	}
}

void ModelScene::OnLoseFocus()
{
	if (m_cameraController)
	{
		m_cameraController->OnLoseControl();
	}
}

bool ModelScene::OnMouseMove(double xPos, double yPos)
{
	if (m_cameraController)
	{
		m_cameraController->OnMouseMove(xPos, yPos);
	}
	return false;
}

bool ModelScene::OnKeyPressed(int key)
{
	if (m_cameraController)
	{
		m_cameraController->OnKeyPressed(key);
	}
	return false;
}

bool ModelScene::OnKeyReleased(int key)
{
	if (m_cameraController)
	{
		m_cameraController->OnKeyReleased(key);
	}
	return false;
}
