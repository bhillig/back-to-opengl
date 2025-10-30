#include <Scene/Scenes/CubeScene.h>

#include <Application.h>

#include <Scene/CameraController.h>

#include <Renderer/ElementBuffer.h>
#include <Renderer/Shader.h>
#include <Renderer/VertexArray.h>
#include <Renderer/VertexBuffer.h>
#include <Renderer/VertexBufferLayout.h>

#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

// Vertex Shaders
const std::string kPositionVS = SHADER_DIR + std::string("/position.vertex.glsl");
const std::string kPositionAndColorVS = SHADER_DIR + std::string("/positionAndColor.vertex.glsl");
const std::string kPositionAndTexCoordVS = SHADER_DIR + std::string("/positionAndTexCoords.vertex.glsl");
const std::string kPositionColorAndTexCoordVS = SHADER_DIR + std::string("/positionColorAndTexCoords.vertex.glsl");

// Fragment Shaders
const std::string kColorFromVertexFS = SHADER_DIR + std::string("/colorFromVertex.fragment.glsl");
const std::string kColorFromTextureFS = SHADER_DIR + std::string("/colorFromTexture.fragment.glsl");
const std::string kColorFromTextureMixFS = SHADER_DIR + std::string("/colorFromTextureMix.fragment.glsl");
const std::string kColorFromLightSourceFS = SHADER_DIR + std::string("/colorFromLightSource.fragment.glsl");
const std::string kYellowFS = SHADER_DIR + std::string("/yellow.fragment.glsl");
const std::string kWhiteFS = SHADER_DIR + std::string("/white.fragment.glsl");

// Textures
const std::string kContainerTexture = TEXTURE_DIR + std::string("/container.jpg");
const std::string kWallTexture = TEXTURE_DIR + std::string("/wall.jpg");
const std::string kAwesomeFaceTexture = TEXTURE_DIR + std::string("/awesomeface.png");
const std::string kMorganFreemanTrueTexture = TEXTURE_DIR + std::string("/true.png");

glm::vec3 cubePositions[] = {
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

CubeScene::CubeScene()
	: Scene()
{
	OnLoad();
}

CubeScene::~CubeScene()
{
	OnUnload();
}

void CubeScene::OnLoad()
{
	Scene::OnLoad();

	float vertices[] = {
		// Position           // Texture Coordinates
		-0.5f, -0.5f, -0.5f,  0.0f, 0.0f,
		 0.5f, -0.5f, -0.5f,  1.0f, 0.0f,
		 0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
		 0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
		-0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
		-0.5f, -0.5f, -0.5f,  0.0f, 0.0f,

		-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
		 0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
		 0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
		 0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
		-0.5f,  0.5f,  0.5f,  0.0f, 1.0f,
		-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,

		-0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
		-0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
		-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
		-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
		-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
		-0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

		 0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
		 0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
		 0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
		 0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
		 0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
		 0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

		-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
		 0.5f, -0.5f, -0.5f,  1.0f, 1.0f,
		 0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
		 0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
		-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
		-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,

		-0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
		 0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
		 0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
		 0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
		-0.5f,  0.5f,  0.5f,  0.0f, 0.0f,
		-0.5f,  0.5f, -0.5f,  0.0f, 1.0f
	};

	// Create a vertex array object
	m_vao = std::make_unique<VertexArray>();
	m_vao->Bind();

	// Create a vertex buffer object
	VertexBuffer vertexBuffer(vertices, sizeof(vertices));
	vertexBuffer.Bind();

	// Specify how our vertex data is formatted
	VertexBufferLayout vertexBufferLayout;
	vertexBufferLayout.Push<float>(3); // Position
	vertexBufferLayout.Push<float>(2); // Texture Coordinates

	m_vao->Add(vertexBuffer, vertexBufferLayout);

	// Unbind
	glBindVertexArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	// Create shader
	m_shader = std::make_unique<Shader>(kPositionAndTexCoordVS, kColorFromTextureMixFS);

	// Create textures
	const unsigned int textureSlot = 0;
	const unsigned int texture2Slot = 1;
	m_texture = std::make_unique<Texture>(kContainerTexture, textureSlot);
	m_texture2 = std::make_unique<Texture>(kMorganFreemanTrueTexture, texture2Slot);

	m_shader->SetUniform1i("u_Texture1", textureSlot);
	m_shader->SetUniform1i("u_Texture2", texture2Slot);

	// Init camera
	const glm::vec3 cameraPos(0.0f, 0.0f, 3.0f);
	const glm::vec3 cameraForward(0.0f, 0.0f, -1.0f);
	const float cameraFOV = 45.f;

	m_camera = std::make_unique<Camera>(cameraPos, 0.0f, -90.f, 0.0f, cameraFOV);
	m_cameraController = std::make_unique<CameraController>(*m_camera);

	// Set initial color
	const glm::vec4 grayColor(0.2f, 0.3f, 0.3f, 1.0f);
	glClearColor(grayColor.r, grayColor.g, grayColor.b, grayColor.a);
}

void CubeScene::OnUnload()
{
	Scene::OnUnload();
}

void CubeScene::ConstructGUI()
{
	Scene::ConstructGUI();

	ImGui::Text("Cube Scene");
	float fov = m_camera->fov();
	if (ImGui::SliderFloat("Camera FOV", &fov, 1.0f, 100.f))
	{
		m_camera->SetFOV(fov);
	}
}

void CubeScene::Update(float deltaTime)
{
	m_value += deltaTime;

	if (m_cameraController)
	{
		m_cameraController->Update(deltaTime);
	}
}

void CubeScene::Render()
{
	const float rotationSpeed = 50.f;

	// Clear screen
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// View
	glm::mat4 view = m_camera->viewMatrix();
	m_shader->SetUniformMatrix4fv("u_View", glm::value_ptr(view));

	// Projection
	glm::mat4 projection = glm::perspective(glm::radians(m_camera->fov()), Application::GetApp()->GetWidth() / Application::GetApp()->GetHeight(), 0.1f, 100.f);
	m_shader->SetUniformMatrix4fv("u_Projection", glm::value_ptr(projection));

	m_shader->SetUniform1f("u_MixAmount", 1.0f);

	// Draw rectangle
	m_texture->Bind();
	m_texture2->Bind();
	m_vao->Bind();
	for (int i = 0; i < 10; ++i)
	{
		// Model
		glm::mat4 model(1.f);
		model = glm::translate(model, cubePositions[i]);
		model = glm::rotate(model, m_value * glm::radians(rotationSpeed), glm::vec3(0.5f, 1.0f, 0.0f));
		m_shader->SetUniformMatrix4fv("u_Model", glm::value_ptr(model));
		m_shader->Bind();
		glDrawArrays(GL_TRIANGLES, 0, 36);
	}
	m_texture->Unbind();
}

void CubeScene::OnKeyPressed(int key)
{
	if (key == GLFW_KEY_ESCAPE)
	{
		m_cameraController->EnableInput(!m_cameraController->inputEnabled());
	}
}
