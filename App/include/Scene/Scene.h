#pragma once

#include <Event.h>

class Scene
{
public:
	virtual ~Scene() {};

	// Simulates the scene
	void Simulate(float deltaTime, unsigned int timeSteps = 1);

	// Constructs the scene's GUI properties
	virtual void ConstructGUI() {}

	// Process events
	void OnEvent(Core::Event& event);

protected:

	// Called every timeSteps times per frame to update the scene's logic
	virtual void Update(float deltaTime) = 0;

	// Called every frame to render the scene
	virtual void Render() = 0;

protected:

	// Event callbacks
	virtual bool OnKeyPressed(int key) { return false; }
	virtual bool OnKeyReleased(int key) { return false; }
	virtual bool OnMouseMove(double xPos, double yPos) { return false; }

};