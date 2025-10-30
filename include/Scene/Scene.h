#pragma once

#include <Core/EventDispatcher.h>

class Scene
{
public:
	Scene();

	virtual ~Scene();

	// Called when the application loads the scene
	virtual void OnLoad();

	// Called when the application unloads the scene
	virtual void OnUnload();

	// Simulates the scene
	void Simulate(float deltaTime, unsigned int timeSteps = 1);

	// Constructs the scene's GUI properties
	virtual void ConstructGUI() {}

protected:

	// Called every timeSteps times per frame to update the scene's logic
	virtual void Update(float deltaTime) = 0;

	// Called every frame to render the scene
	virtual void Render() = 0;

protected:

	// Event callbacks
	virtual void OnKeyPressed(int key) {}
	virtual void OnKeyReleased(int key) {}
	virtual void OnMouseMove(double xPos, double yPos) {}

private:

	// Process events
	void OnEvent(const Event& event);

	EventHandle m_eventHandle;

};