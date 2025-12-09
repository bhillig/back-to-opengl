#include <GameSceneLayer.h>

#include <Application.h>

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <InputEvents.h>

#include <Scene/Scenes/CubeScene.h>
#include <Scene/Scenes/LightingDemoScene.h>
#include <Scene/Scenes/ModelScene.h>

#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>

#include <iostream>

#define STRINGIFY(X) #X

GameSceneLayer::GameSceneLayer()
{
	m_currentScene = std::make_unique<LightingDemoScene>();
}

void GameSceneLayer::OnUpdate(float deltaTime)
{
	// Simulate the current scene (Update and Render)
	m_currentScene->Simulate(deltaTime);
	RenderGUI();
}

void GameSceneLayer::OnEvent(Core::Event& event)
{
	Core::EventDispatcher dispatcher(event);
	dispatcher.Dispatch<Core::KeyPressedEvent>([this](Core::KeyPressedEvent& event) { return OnKeyPressed(event.GetKeyCode()); });

	m_currentScene->OnEvent(event);
}

bool GameSceneLayer::OnKeyPressed(int keyCode)
{
	if (keyCode == GLFW_KEY_ESCAPE)
	{
		ToggleInputMode();
	}
	return false;
}

void GameSceneLayer::RenderGUI()
{
	// Render the current scene
	ImGui::Begin("Properties");
	m_currentScene->ConstructGUI();
	ImGui::Text("Scenes:");
	if (ImGui::Button(STRINGIFY(LightingDemoScene)))
	{
		m_currentScene = std::make_unique<LightingDemoScene>();
	}
	if (ImGui::Button(STRINGIFY(CubeScene)))
	{
		m_currentScene = std::make_unique<CubeScene>();
	}
	if (ImGui::Button(STRINGIFY(ModelScene)))
	{
		m_currentScene = std::make_unique<ModelScene>();
	}
	ImGui::End();
}

void GameSceneLayer::ToggleInputMode()
{
	const int cursorMode = glfwGetInputMode(Core::Application::GetApp()->GetGLFWWindow(), GLFW_CURSOR);
	switch (cursorMode)
	{
	case GLFW_CURSOR_DISABLED:
		glfwSetInputMode(Core::Application::GetApp()->GetGLFWWindow(), GLFW_CURSOR, GLFW_CURSOR_NORMAL);
		break;
	case GLFW_CURSOR_NORMAL:
		glfwSetInputMode(Core::Application::GetApp()->GetGLFWWindow(), GLFW_CURSOR, GLFW_CURSOR_DISABLED);
		break;
	default:
		break;
	}
}
