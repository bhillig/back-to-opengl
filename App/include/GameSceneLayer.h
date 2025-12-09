#pragma once

#include <Layer.h>

#include <Scene/Scene.h>
#include <Event.h>

#include <memory>

class GameSceneLayer : public Core::Layer
{

public:
	GameSceneLayer();

	void OnUpdate(float deltaTime) override;

	void OnEvent(Core::Event& event) override;

// Event callbacks
private:
	bool OnKeyPressed(int keyCode);

private:

	void RenderGUI();

	void ToggleInputMode();

	std::unique_ptr<Scene> m_currentScene;
};