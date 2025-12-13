#pragma once

#include <Layer.h>

class GUILayer : public Core::Layer
{

public:
	GUILayer();

	void OnEvent(Core::Event& event) override;

private:

	bool OnKeyPressed(int key);

	void ToggleControl();

	bool m_hasControl; // Whether this layer has control (consumes events)
};