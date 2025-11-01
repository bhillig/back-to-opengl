#include <Scene/Scenes/LightingDemoScene.h>

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
const std::string kPositionVS = SHADER_DIR + std::string("/position.vertex.glsl");
const std::string kPositionAndNormalVS = SHADER_DIR + std::string("/positionAndNormal.vertex.glsl");
const std::string kPositionAndColorVS = SHADER_DIR + std::string("/positionAndColor.vertex.glsl");
const std::string kPositionAndTexCoordVS = SHADER_DIR + std::string("/positionAndTexCoords.vertex.glsl");
const std::string kPositionColorAndTexCoordVS = SHADER_DIR + std::string("/positionColorAndTexCoords.vertex.glsl");

// Fragment Shaders
const std::string kColorFromVertexFS = SHADER_DIR + std::string("/colorFromVertex.fragment.glsl");
const std::string kColorFromTextureFS = SHADER_DIR + std::string("/colorFromTexture.fragment.glsl");
const std::string kColorFromTextureMixFS = SHADER_DIR + std::string("/colorFromTextureMix.fragment.glsl");
const std::string kColorFromLightSourceFS = SHADER_DIR + std::string("/colorFromLightSource.fragment.glsl");
const std::string kColorFromUniformFS = SHADER_DIR + std::string("/colorFromUniform.fragment.glsl");
const std::string kYellowFS = SHADER_DIR + std::string("/yellow.fragment.glsl");
const std::string kWhiteFS = SHADER_DIR + std::string("/white.fragment.glsl");

LightingDemoScene::LightingDemoScene()
	: Scene()
	, m_value(0.f)
{
	OnLoad();
}

LightingDemoScene::~LightingDemoScene()
{
	OnUnload();
}

void LightingDemoScene::OnLoad()
{
	Scene::OnLoad();

	float objectVertices[] = {
		// Positions		  // Normals
		-0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f, // Front-face
		 0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
		 0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
		 0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
		-0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
		-0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,

		-0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f, // Back-face
		 0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,
		 0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,
		 0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,
		-0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,
		-0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,

		-0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f, // Left-face
		-0.5f,  0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
		-0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
		-0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
		-0.5f, -0.5f,  0.5f, -1.0f,  0.0f,  0.0f,
		-0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,

		 0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f, // Right-face
		 0.5f,  0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
		 0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
		 0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
		 0.5f, -0.5f,  0.5f,  1.0f,  0.0f,  0.0f,
		 0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,

		-0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f, // Bottom-face
		 0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,
		 0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
		 0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
		-0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
		-0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,

		-0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f, // Top-face
		 0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,
		 0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
		 0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
		-0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
		-0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f
	};

	// Create a vertex array object
	m_vao = std::make_unique<VertexArray>();
	m_vao->Bind();

	// Create a vertex buffer object
	VertexBuffer vertexBuffer(objectVertices, sizeof(objectVertices));
	vertexBuffer.Bind();

	// Specify how our vertex data is formatted
	VertexBufferLayout vertexBufferLayout;
	vertexBufferLayout.Push<float>(3); // Position
	vertexBufferLayout.Push<float>(3); // Normal

	m_vao->Add(vertexBuffer, vertexBufferLayout);

	// Unbind
	glBindVertexArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	// Light source does the same thing as object
	m_lightSourceVAO = std::make_unique<VertexArray>();
	m_lightSourceVAO->Bind();
	m_lightSourceVAO->Add(vertexBuffer, vertexBufferLayout);

	// Unbind
	glBindVertexArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	// Create shaders
	m_lightSourceShader = std::make_unique<Shader>(kPositionVS, kColorFromUniformFS);
	m_colorFromLightSourceShader = std::make_unique<Shader>(kPositionAndNormalVS, kColorFromLightSourceFS);

	// Init camera
	const glm::vec3 cameraPos(1.0f, 0.0f, 4.0f);
	const glm::vec3 cameraForward(0.0f, 0.0f, -1.0f);
	const float cameraFOV = 45.f;

	// Set light source position
	m_lightingPosition = glm::vec3( 0, 1, -3);

	// Set light source color
	m_lightSourceColor = glm::vec3(1.f, 1.f, 1.f);

	// Set object position
	m_objectPosition = glm::vec3(-1.5f, -1.f, -1.f);

	// Set object color
	m_objectColor = glm::vec3(1.f, 0.f, 0.f);

	m_camera = std::make_unique<Camera>(cameraPos, 0.0f, -90.f, 0.0f, cameraFOV);
	m_cameraController = std::make_unique<CameraController>(*m_camera);

	// Set initial color
	const glm::vec4 blackColor(0.0f, 0.0f, 0.0f, 1.0f);
	glClearColor(blackColor.r, blackColor.g, blackColor.b, blackColor.a);
}

void LightingDemoScene::OnUnload()
{
	Scene::OnUnload();
}

void LightingDemoScene::ConstructGUI()
{
	Scene::ConstructGUI();

	ImGui::Text("Cube Scene");
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
	float lightingPos[3]{m_lightingPosition.x, m_lightingPosition.y, m_lightingPosition.z};
	if (ImGui::SliderFloat3("Lighting Position", lightingPos, -5.f, 5.f))
	{
		m_lightingPosition = glm::vec3(lightingPos[0], lightingPos[1], lightingPos[2]);
	}
	float lightSourceColor[3]{ m_lightSourceColor.x, m_lightSourceColor.y, m_lightSourceColor.z };
	if (ImGui::ColorPicker3("Light Source Color", lightSourceColor))
	{
		m_lightSourceColor = glm::vec3(lightSourceColor[0], lightSourceColor[1], lightSourceColor[2]);
	}
	float objectColor[3]{ m_objectColor.x, m_objectColor.y, m_objectColor.z };
	if (ImGui::ColorPicker3("Object Color", objectColor))
	{
		m_objectColor = glm::vec3(objectColor[0], objectColor[1], objectColor[2]);
	}
}

void LightingDemoScene::Update(float deltaTime)
{
	m_cameraController->Update(deltaTime);

	m_value += deltaTime;
}

void LightingDemoScene::Render()
{
	const float rotationSpeed = 45.f;

	// Clear screen
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// View
	glm::mat4 view = m_camera->viewMatrix();
	m_lightSourceShader->SetUniformMatrix4fv("u_View", glm::value_ptr(view));
	m_colorFromLightSourceShader->SetUniformMatrix4fv("u_View", glm::value_ptr(view));

	// Projection
	glm::mat4 projection = glm::perspective(glm::radians(m_camera->fov()), Application::GetApp()->GetWidth() / Application::GetApp()->GetHeight(), 0.1f, 100.f);
	m_lightSourceShader->SetUniformMatrix4fv("u_Projection", glm::value_ptr(projection));
	m_colorFromLightSourceShader->SetUniformMatrix4fv("u_Projection", glm::value_ptr(projection));

	m_lightSourceShader->SetUniform4f("u_Color", m_lightSourceColor.r, m_lightSourceColor.g, m_lightSourceColor.b, 1.f);

	m_colorFromLightSourceShader->SetUniform3f("u_Color", m_objectColor.r, m_objectColor.g, m_objectColor.b);
	m_colorFromLightSourceShader->SetUniform3f("u_LightSourceColor", m_lightSourceColor.r, m_lightSourceColor.g, m_lightSourceColor.b);
	m_colorFromLightSourceShader->SetUniform3f("u_LightSourcePos", m_lightingPosition.x, m_lightingPosition.y, m_lightingPosition.z);
	m_colorFromLightSourceShader->SetUniform3f("u_ViewPos", m_camera->position().x, m_camera->position().y, m_camera->position().z);


	// Light source model
	{
		glm::mat4 model(1.f);
		model = glm::translate(model, m_lightingPosition);
		model = glm::scale(model, glm::vec3(0.1f, 0.1f, 0.1f));
		m_lightSourceShader->SetUniformMatrix4fv("u_Model", glm::value_ptr(model));
		m_lightSourceShader->Bind();
		m_vao->Bind();
		glDrawArrays(GL_TRIANGLES, 0, 36);
	}

	// Object affected by light source
	{
		glm::mat4 model(1.f);
		model = glm::translate(model, m_objectPosition);
		model = glm::rotate(model, m_value * glm::radians(rotationSpeed), glm::vec3(0.f, 1.0f, 0.0f));
		m_colorFromLightSourceShader->SetUniformMatrix4fv("u_Model", glm::value_ptr(model));
		m_colorFromLightSourceShader->Bind();
		m_lightSourceVAO->Bind();
		glDrawArrays(GL_TRIANGLES, 0, 36);
	}
}

void LightingDemoScene::OnKeyPressed(int key)
{
	if (key == GLFW_KEY_ESCAPE)
	{
		m_cameraController->EnableInput(!m_cameraController->inputEnabled());
	}
}
