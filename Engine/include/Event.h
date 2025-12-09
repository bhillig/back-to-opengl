#pragma once

enum class EventType
{
	MouseMove = 0,
	KeyPressed = 1,
	KeyReleased = 2
};

struct Event
{
	EventType type;
	double x;
	double y;
	int key;
};
