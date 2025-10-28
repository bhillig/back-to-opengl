#include <Scene/Scene.h>

#include <Window/Window.h>

#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>

Scene::Scene(Window* window)
	: m_window(window)
{
}

void Scene::OnLoad()
{
	m_window->GetEventDispatcher().SubscribeAll([this](const Event& event) {
		OnEvent(event);
		});
}

void Scene::OnUnload()
{
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
