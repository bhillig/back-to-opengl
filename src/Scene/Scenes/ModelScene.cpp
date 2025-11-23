#include <Scene/Scenes/ModelScene.h>

#include <Application.h>

#include <Renderer/VertexBuffer.h>
#include <Renderer/VertexBufferLayout.h>
#include <Renderer/ElementBuffer.h>
#include <Renderer/Shader.h>

#include <Scene/CameraController.h>

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
	: Scene()
{
	OnLoad();
}

ModelScene::~ModelScene()
{
	OnUnload();
}

void ModelScene::OnLoad()
{
	Scene::OnLoad();

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

	// Set object position
	m_objectPosition = glm::vec3(0.f, 0.f, 0.f);

	// Set object scale
	m_scale = 1.f;

	// Set initial color
	const glm::vec4 bgColor(0.0f, 0.0f, 0.0f, 1.0f);
	glClearColor(bgColor.r, bgColor.g, bgColor.b, bgColor.a);
}

void ModelScene::OnUnload()
{
	Scene::OnUnload();
}

void ModelScene::ConstructGUI()
{
	Scene::ConstructGUI();

	ImGui::Text("Model Scene");
	float fov = m_camera->fov();
	if (ImGui::SliderFloat("Camera FOV", &fov, 1.0f, 100.f))
	{
		m_camera->SetFOV(fov);
	}
	float objectPos[3]{ m_objectPosition.x, m_objectPosition.y, m_objectPosition.z };
	if (ImGui::SliderFloat3("Object Position", objectPos, -5.f, 5.f))
	{
		m_objectPosition = glm::vec3(objectPos[0], objectPos[1], objectPos[2]);
	}
	float scale = m_scale;
	if (ImGui::SliderFloat("Object Scale", &scale, 0.1f, 2.f))
	{
		m_scale = scale;
	}
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
	glm::mat4 projection = glm::perspective(glm::radians(m_camera->fov()), static_cast<float>(Core::Application::GetApp()->GetWidth()) / Core::Application::GetApp()->GetHeight(), 0.1f, 100.f);
	m_modelShader->SetUniformMatrix4fv("u_Projection", glm::value_ptr(projection));

	// Set model matrix for model
	glm::mat4 modelObj(1.f);
	modelObj = glm::translate(modelObj, m_objectPosition);
	modelObj = glm::scale(modelObj, glm::vec3(m_scale, m_scale, m_scale));
	m_modelShader->SetUniformMatrix4fv("u_Model", glm::value_ptr(modelObj));

	// Draw model
	m_model->Draw(*m_modelShader);
}

void ModelScene::OnKeyPressed(int key)
{
	if (key == GLFW_KEY_ESCAPE)
	{
		m_cameraController->EnableInput(!m_cameraController->inputEnabled());
	}
}
