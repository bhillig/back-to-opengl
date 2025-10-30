#include <Scene/Scene.h>

#include <Application.h>

#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>

Scene::Scene()
	: m_eventHandle({ 0 })
{
}

Scene::~Scene()
{
}

void Scene::OnLoad()
{
	m_eventHandle = Application::GetApp()->GetEventDispatcher().SubscribeAll([this](const Event& event) {
		OnEvent(event);
		});
}

void Scene::OnUnload()
{
	Application::GetApp()->GetEventDispatcher().UnsubscribeAll(m_eventHandle);
}

void Scene::Simulate(float deltaTime, unsigned int timeSteps /* = 1*/)
{
	for (unsigned int i = 0; i < timeSteps; ++i)
	{
		Update(deltaTime);
	}
	Render();
}

void Scene::OnEvent(const Event& event)
{
	switch (event.type)
	{
	case EventType::KeyPressed:
		OnKeyPressed(event.key);
		break;
	case EventType::KeyReleased:
		OnKeyReleased(event.key);
		break;
	case EventType::MouseMove:
		OnMouseMove(event.x, event.y);
		break;
	default:
		break;
	}
}
