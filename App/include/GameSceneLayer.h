#pragma once

#include <Layer.h>

#include <memory>

class Scene;

class GameSceneLayer : public Core::Layer
{

public:
	GameSceneLayer();
	~GameSceneLayer() override;

	void OnUpdate(float deltaTime) override;

	void OnEvent();

private:

	void RenderGUI();

	std::unique_ptr<Scene> m_currentScene;
};