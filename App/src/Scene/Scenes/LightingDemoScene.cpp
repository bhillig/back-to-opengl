#include <Scene/Scenes/LightingDemoScene.h>

#include <Application.h>

#include <Renderer/VertexBuffer.h>
#include <Renderer/VertexBufferLayout.h>
#include <Renderer/ElementBuffer.h>
#include <Renderer/Shader.h>

#include <CameraController.h>

#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>

// Vertex Shaders
const std::string kPositionVS = SHADER_DIR + std::string("/position.vertex.glsl");
const std::string kPositionAndNormalVS = SHADER_DIR + std::string("/positionAndNormal.vertex.glsl");
const std::string kPositionAndTexCoordVS = SHADER_DIR + std::string("/positionAndTexCoords.vertex.glsl");
const std::string kPositionNormalAndTexCoordVS = SHADER_DIR + std::string("/positionNormalAndTexCoords.vertex.glsl");
const std::string kPositionColorAndTexCoordVS = SHADER_DIR + std::string("/positionColorAndTexCoords.vertex.glsl");

// Fragment Shaders
const std::string kColorFromVertexFS = SHADER_DIR + std::string("/colorFromVertex.fragment.glsl");
const std::string kColorFromTextureFS = SHADER_DIR + std::string("/colorFromTexture.fragment.glsl");
const std::string kColorFromTextureMixFS = SHADER_DIR + std::string("/colorFromTextureMix.fragment.glsl");
const std::string kColorFromLightSourceFS = SHADER_DIR + std::string("/colorFromLightSource.fragment.glsl");
const std::string kColorFromUniformFS = SHADER_DIR + std::string("/colorFromUniform.fragment.glsl");
const std::string kYellowFS = SHADER_DIR + std::string("/yellow.fragment.glsl");
const std::string kWhiteFS = SHADER_DIR + std::string("/white.fragment.glsl");

// Textures
const std::string kCrateTexture = TEXTURE_DIR + std::string("/crate.png");
const std::string kCrateSpecularTexture = TEXTURE_DIR + std::string("/crate_specular.png");

const int kCrateDiffuseTexureSlot = 0;
const int kCrateSpecularTextureSlot = 1;

glm::vec3 lightingCubePositions[] = {
	glm::vec3(0.0f,  0.0f,  0.0f),
	glm::vec3(2.0f,  5.0f, -15.0f),
	glm::vec3(-1.5f, -2.2f, -2.5f),
	glm::vec3(-3.8f, -2.0f, -12.3f),
	glm::vec3(2.4f, -0.4f, -3.5f),
	glm::vec3(-1.7f,  3.0f, -7.5f),
	glm::vec3(1.3f, -2.0f, -2.5f),
	glm::vec3(1.5f,  2.0f, -2.5f),
	glm::vec3(1.5f,  0.2f, -1.5f),
	glm::vec3(-1.3f,  1.0f, -1.5f)
};

glm::vec3 pointLightPositions[] = {
	glm::vec3(0.7f,  0.2f,  2.0f),
	glm::vec3(2.3f, -3.3f, -4.0f),
	glm::vec3(-4.0f,  2.0f, -12.0f),
	glm::vec3(0.0f,  0.0f, -3.0f)
};

LightingDemoScene::LightingDemoScene()
	: m_value(0.f)
{
	float objectVertices[] = {
		// positions          // normals           // texture coords
		-0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f, 0.0f,
		 0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f, 0.0f,
		 0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f, 1.0f,
		 0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f, 1.0f,
		-0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f, 1.0f,
		-0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f, 0.0f,

		-0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   0.0f, 0.0f,
		 0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   1.0f, 0.0f,
		 0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   1.0f, 1.0f,
		 0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   1.0f, 1.0f,
		-0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   0.0f, 1.0f,
		-0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   0.0f, 0.0f,

		-0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  1.0f, 0.0f,
		-0.5f,  0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  1.0f, 1.0f,
		-0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  0.0f, 1.0f,
		-0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  0.0f, 1.0f,
		-0.5f, -0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  0.0f, 0.0f,
		-0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  1.0f, 0.0f,

		 0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  1.0f, 0.0f,
		 0.5f,  0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  1.0f, 1.0f,
		 0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  0.0f, 1.0f,
		 0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  0.0f, 1.0f,
		 0.5f, -0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  0.0f, 0.0f,
		 0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  1.0f, 0.0f,

		-0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  0.0f, 1.0f,
		 0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  1.0f, 1.0f,
		 0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  1.0f, 0.0f,
		 0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  1.0f, 0.0f,
		-0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  0.0f, 0.0f,
		-0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  0.0f, 1.0f,

		-0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  0.0f, 1.0f,
		 0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  1.0f, 1.0f,
		 0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  1.0f, 0.0f,
		 0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  1.0f, 0.0f,
		-0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  0.0f, 0.0f,
		-0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  0.0f, 1.0f
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
	vertexBufferLayout.Push<float>(2); // Texture Coordinates

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
	m_colorFromLightSourceShader = std::make_unique<Shader>(kPositionNormalAndTexCoordVS, kColorFromLightSourceFS);

	// Init crate texture
	m_crateTexture = std::make_unique<Texture>(kCrateTexture, kCrateDiffuseTexureSlot);
	m_crateSpecularTexture = std::make_unique<Texture>(kCrateSpecularTexture, kCrateSpecularTextureSlot);

	// Init camera
	const glm::vec3 cameraPos(0.0f, 0.0f, 4.0f);
	const glm::vec3 cameraForward(0.0f, 0.0f, -1.0f);
	const float cameraFOV = 45.f;

	// Set light source position
	m_lightingPosition = glm::vec3(0, 0, -2.f);

	// Set light source color
	m_lightSourceColor = glm::vec3(1.f, 1.f, 1.f);

	// Set object position
	m_objectPosition = glm::vec3(0.f, -1.f, 0.5f);

	// Set object color
	m_objectColor = glm::vec3(1.f, 0.f, 0.f);

	m_camera = std::make_unique<Camera>(cameraPos, 0.0f, -90.f, 0.0f, cameraFOV);
	m_cameraController = std::make_unique<CameraController>(*m_camera);

	// Set material properties on the object	
	m_colorFromLightSourceShader->SetUniform1i("u_Material.diffuse", kCrateDiffuseTexureSlot);
	m_colorFromLightSourceShader->SetUniform1i("u_Material.specular", kCrateSpecularTextureSlot);
	m_colorFromLightSourceShader->SetUniform1f("u_Material.shininess", 32.f);

	// Set directional light properties
	m_colorFromLightSourceShader->SetUniform3f("u_DirectionalLight.direction", -0.1f, -1.0f, -0.2f);
	m_colorFromLightSourceShader->SetUniform3f("u_DirectionalLight.ambient", 0.2f, 0.2f, 0.2f);
	m_colorFromLightSourceShader->SetUniform3f("u_DirectionalLight.diffuse", 0.5f, 0.5f, 0.5f);
	m_colorFromLightSourceShader->SetUniform3f("u_DirectionalLight.specular", 1.f, 1.f, 1.f);

	// Set Point light properties
	m_colorFromLightSourceShader->SetUniform3f("u_PointLights[0].ambient", 0.2f, 0.2f, 0.2f);
	m_colorFromLightSourceShader->SetUniform3f("u_PointLights[0].diffuse", 0.5f, 0.5f, 0.5f);
	m_colorFromLightSourceShader->SetUniform3f("u_PointLights[0].specular", 1.f, 1.f, 1.f);
	m_colorFromLightSourceShader->SetUniform1f("u_PointLights[0].constant", 1.f);
	m_colorFromLightSourceShader->SetUniform1f("u_PointLights[0].linear", 0.09f);
	m_colorFromLightSourceShader->SetUniform1f("u_PointLights[0].quadratic", 0.032f);

	m_colorFromLightSourceShader->SetUniform3f("u_PointLights[1].ambient", 0.2f, 0.2f, 0.2f);
	m_colorFromLightSourceShader->SetUniform3f("u_PointLights[1].diffuse", 0.5f, 0.5f, 0.5f);
	m_colorFromLightSourceShader->SetUniform3f("u_PointLights[1].specular", 1.f, 1.f, 1.f);
	m_colorFromLightSourceShader->SetUniform1f("u_PointLights[1].constant", 1.f);
	m_colorFromLightSourceShader->SetUniform1f("u_PointLights[1].linear", 0.09f);
	m_colorFromLightSourceShader->SetUniform1f("u_PointLights[1].quadratic", 0.032f);

	m_colorFromLightSourceShader->SetUniform3f("u_PointLights[2].ambient", 0.2f, 0.2f, 0.2f);
	m_colorFromLightSourceShader->SetUniform3f("u_PointLights[2].diffuse", 0.5f, 0.5f, 0.5f);
	m_colorFromLightSourceShader->SetUniform3f("u_PointLights[2].specular", 1.f, 1.f, 1.f);
	m_colorFromLightSourceShader->SetUniform1f("u_PointLights[2].constant", 1.f);
	m_colorFromLightSourceShader->SetUniform1f("u_PointLights[2].linear", 0.09f);
	m_colorFromLightSourceShader->SetUniform1f("u_PointLights[2].quadratic", 0.032f);

	m_colorFromLightSourceShader->SetUniform3f("u_PointLights[3].ambient", 0.2f, 0.2f, 0.2f);
	m_colorFromLightSourceShader->SetUniform3f("u_PointLights[3].diffuse", 0.5f, 0.5f, 0.5f);
	m_colorFromLightSourceShader->SetUniform3f("u_PointLights[3].specular", 1.f, 1.f, 1.f);
	m_colorFromLightSourceShader->SetUniform1f("u_PointLights[3].constant", 1.f);
	m_colorFromLightSourceShader->SetUniform1f("u_PointLights[3].linear", 0.09f);
	m_colorFromLightSourceShader->SetUniform1f("u_PointLights[3].quadratic", 0.032f);

	// Set Spot Light properties
	m_colorFromLightSourceShader->SetUniform1f("u_SpotLight.innerCutOff", glm::cos(glm::radians(12.5f)));
	m_colorFromLightSourceShader->SetUniform1f("u_SpotLight.outerCutOff", glm::cos(glm::radians(17.5f)));
	m_colorFromLightSourceShader->SetUniform3f("u_SpotLight.ambient", 0.2f, 0.2f, 0.2f);
	m_colorFromLightSourceShader->SetUniform3f("u_SpotLight.diffuse", 0.5f, 0.5f, 0.5f);
	m_colorFromLightSourceShader->SetUniform3f("u_SpotLight.specular", 1.f, 1.f, 1.f);

	// Set flashlight off
	m_flashlightOn = false;

	// Set initial color
	const glm::vec4 blackColor(0.0f, 0.0f, 0.0f, 1.0f);
	glClearColor(blackColor.r, blackColor.g, blackColor.b, blackColor.a);
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
	for (int i = 0; i < 4; ++i)
	{
		float lightingPos[3]{ pointLightPositions[i].x, pointLightPositions[i].y, pointLightPositions[i].z };
		std::string lightingText = "Lighting Position " + std::to_string(i);
		if (ImGui::SliderFloat3(lightingText.c_str(), lightingPos, -5.f, 5.f))
		{
			pointLightPositions[i] = glm::vec3(lightingPos[0], lightingPos[1], lightingPos[2]);
		}
	}
}

void LightingDemoScene::Update(float deltaTime)
{
	m_cameraController->Update(deltaTime);

	m_value += deltaTime;
}

void LightingDemoScene::Render()
{
	const float rotationSpeed = 0.f;

	// Clear screen
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// View
	glm::mat4 view = m_camera->viewMatrix();
	m_lightSourceShader->SetUniformMatrix4fv("u_View", glm::value_ptr(view));
	m_colorFromLightSourceShader->SetUniformMatrix4fv("u_View", glm::value_ptr(view));

	// Projection
	glm::mat4 projection = glm::perspective(glm::radians(m_camera->fov()), static_cast<float>(Core::Application::GetApp()->GetWidth()) / Core::Application::GetApp()->GetHeight(), 0.1f, 100.f);
	m_lightSourceShader->SetUniformMatrix4fv("u_Projection", glm::value_ptr(projection));
	m_colorFromLightSourceShader->SetUniformMatrix4fv("u_Projection", glm::value_ptr(projection));

	m_lightSourceShader->SetUniform4f("u_Color", m_lightSourceColor.r, m_lightSourceColor.g, m_lightSourceColor.b, 1.f);

	m_colorFromLightSourceShader->SetUniform3f("u_PointLights[0].position", pointLightPositions[0].x, pointLightPositions[0].y, pointLightPositions[0].z);
	m_colorFromLightSourceShader->SetUniform3f("u_PointLights[1].position", pointLightPositions[1].x, pointLightPositions[1].y, pointLightPositions[1].z);
	m_colorFromLightSourceShader->SetUniform3f("u_PointLights[2].position", pointLightPositions[2].x, pointLightPositions[2].y, pointLightPositions[2].z);
	m_colorFromLightSourceShader->SetUniform3f("u_PointLights[3].position", pointLightPositions[3].x, pointLightPositions[3].y, pointLightPositions[3].z);

	m_colorFromLightSourceShader->SetUniform3f("u_SpotLight.position", m_camera->position().x, m_camera->position().y, m_camera->position().z);
	m_colorFromLightSourceShader->SetUniform3f("u_SpotLight.direction", m_camera->forward().x, m_camera->forward().y, m_camera->forward().z);

	m_colorFromLightSourceShader->SetUniform1b("u_UseSpotLight", m_flashlightOn);

	m_colorFromLightSourceShader->SetUniform3f("u_ViewPos", m_camera->position().x, m_camera->position().y, m_camera->position().z);

	// Draw four point lights
	const int numberOfPointLights = sizeof(pointLightPositions) / sizeof(pointLightPositions[0]);
	for (int i = 0; i < numberOfPointLights; ++i)
	{
		glm::mat4 model(1.f);
		model = glm::translate(model, pointLightPositions[i]);
		model = glm::scale(model, glm::vec3(0.1f, 0.1f, 0.1f));
		m_lightSourceShader->SetUniformMatrix4fv("u_Model", glm::value_ptr(model));
		m_lightSourceShader->Bind();
		m_vao->Bind();
		glDrawArrays(GL_TRIANGLES, 0, 36);
	}

	// Objects affected by the light source
	m_lightSourceVAO->Bind();
	m_crateTexture->Bind(kCrateDiffuseTexureSlot);
	m_crateSpecularTexture->Bind(kCrateSpecularTextureSlot);

	const int numberOfCubes = sizeof(lightingCubePositions) / sizeof(lightingCubePositions[0]);
	for (unsigned int i = 0; i < numberOfCubes; i++)
	{
		glm::mat4 model = glm::mat4(1.0f);
		model = glm::translate(model, lightingCubePositions[i]);
		float angle = 20.0f * i;
		model = glm::rotate(model, glm::radians(angle), glm::vec3(1.0f, 0.3f, 0.5f));
		m_colorFromLightSourceShader->SetUniformMatrix4fv("u_Model", glm::value_ptr(model));
		m_colorFromLightSourceShader->Bind();
		glDrawArrays(GL_TRIANGLES, 0, 36);
	}
	m_lightSourceVAO->Unbind();
	m_crateTexture->Unbind();
	m_crateSpecularTexture->Unbind();
}

bool LightingDemoScene::OnMouseMove(double xPos, double yPos)
{
	if (m_cameraController)
	{
		m_cameraController->OnMouseMove(xPos, yPos);
	}
	return false;
}

bool LightingDemoScene::OnKeyPressed(int key)
{
	if (m_cameraController)
	{
		m_cameraController->OnKeyPressed(key);
	}

	if (key == GLFW_KEY_ESCAPE)
	{
		m_cameraController->EnableInput(!m_cameraController->inputEnabled());
		return true;
	}
	if (key == GLFW_KEY_F)
	{
		m_flashlightOn = !m_flashlightOn;
		return true;
	}
	return false;
}

bool LightingDemoScene::OnKeyReleased(int key)
{
	if (m_cameraController)
	{
		m_cameraController->OnKeyReleased(key);
	}
	return false;
}
