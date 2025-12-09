#include <GameSceneLayer.h>

#include <Scene/Scenes/CubeScene.h>
#include <Scene/Scenes/LightingDemoScene.h>
#include <Scene/Scenes/ModelScene.h>


#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>

#include <iostream>

GameSceneLayer::GameSceneLayer()
{
	m_currentScene = std::make_unique<LightingDemoScene>();
}

GameSceneLayer::~GameSceneLayer()
{
	m_currentScene.release();
}

void GameSceneLayer::OnUpdate(float deltaTime)
{
	// Simulate the current scene (Update and Render)
	m_currentScene->Simulate(deltaTime);
	RenderGUI();
}

void GameSceneLayer::OnEvent()
{
}

void GameSceneLayer::RenderGUI()
{
	// Render the current scene
	ImGui::Begin("Properties");
	m_currentScene->ConstructGUI();
	ImGui::Text("Scenes:");
	if (ImGui::Button("Lighting Scene"))
	{
		m_currentScene = std::make_unique<LightingDemoScene>();
	}
	if (ImGui::Button("Cube Scene"))
	{
		m_currentScene = std::make_unique<CubeScene>();
	}
	if (ImGui::Button("Model Scene"))
	{
		m_currentScene = std::make_unique<ModelScene>();
	}
	ImGui::End();
}
