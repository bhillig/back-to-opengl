#pragma once

#include <Core/Event.h>

#include <functional>
#include <unordered_map>
#include <vector>

class EventDispatcher
{
	using ListenerCallback = std::function<void(const Event& e)>;

public:
	EventDispatcher() = default;
	~EventDispatcher() = default;

	void Subscribe(EventType type, ListenerCallback callback)
	{
		m_listeners[type].emplace_back(callback);
	}

	void SubscribeAll(ListenerCallback callback)
	{
		m_globalListeners.emplace_back(callback);
	}

	void Dispatch(const Event& event)
	{
		// Dispatch to global listeners first
		for (auto& listener : m_globalListeners)
		{
			listener(event);
		}

		// Dispatch to those listening for this event type
		auto it = m_listeners.find(event.type);
		if (it == m_listeners.end())
			return;

		for (auto& listener : it->second)
		{
			listener(event);
		}
	}

private:
	std::unordered_map<EventType, std::vector<ListenerCallback>> m_listeners;
	std::vector<ListenerCallback> m_globalListeners;
};
