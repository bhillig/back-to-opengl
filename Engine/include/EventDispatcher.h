#pragma once

#include <Event.h>

#include <functional>
#include <unordered_map>
#include <vector>

struct EventHandle
{
	size_t id;
};

struct EventListener
{
	size_t id;
	std::function<void(const Event& e)> callback;
};

class EventDispatcher
{
	using ListenerCallback = std::function<void(const Event& e)>;

public:
	EventDispatcher() = default;
	~EventDispatcher() = default;

	EventHandle Subscribe(EventType type, ListenerCallback callback)
	{
		EventListener listener{ m_nextId++, callback };
		m_listeners[type].emplace_back(listener);
		return { listener.id };
	}

	EventHandle SubscribeAll(ListenerCallback callback)
	{
		EventListener listener{ m_nextId++, callback };
		m_globalListeners.emplace_back(listener);
		return { listener.id };
	}

	void Unsubscribe(EventHandle handle)
	{
		for (auto& [type, listeners] : m_listeners)
		{
			auto it = std::find_if(listeners.begin(), listeners.end(), [this, handle](const EventListener& listener) {
				return handle.id == listener.id;
				});

			if (it != listeners.end())
			{
				listeners.erase(it);
			}
		}
	}

	void UnsubscribeAll(EventHandle handle)
	{
		auto it = std::find_if(m_globalListeners.begin(), m_globalListeners.end(), [this, handle](const EventListener& listener) {
			return handle.id == listener.id;
			});

		if (it != m_globalListeners.end())
		{
			m_globalListeners.erase(it);
		}
	}

	void Dispatch(const Event& event)
	{
		// Dispatch to global listeners first
		for (auto& listener : m_globalListeners)
		{
			listener.callback(event);
		}

		// Dispatch to those listening for this event type
		auto it = m_listeners.find(event.type);
		if (it == m_listeners.end())
			return;

		for (auto& listener : it->second)
		{
			listener.callback(event);
		}
	}

private:
	std::unordered_map<EventType, std::vector<EventListener>> m_listeners;
	std::vector<EventListener> m_globalListeners;
	size_t m_nextId = 0;
};
