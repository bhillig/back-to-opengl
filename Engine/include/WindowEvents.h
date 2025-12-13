#pragma once

#include <Events/Event.h>

namespace Core
{

class WindowClosedEvent : public Event
{
public:
	WindowClosedEvent() {}
	EVENT_CLASS_TYPE(WindowClosed);
};

class WindowResizedEvent : public Event
{
public:
	WindowResizedEvent(int width, int height)
		: m_width(width), m_height(height)
	{}

	inline int GetWidth() const { return m_width; }
		
	inline int GetHeight() const { return m_height; }

	EVENT_CLASS_TYPE(WindowResized);
private:
	int m_width;
	int m_height;
};

} // namespace Core