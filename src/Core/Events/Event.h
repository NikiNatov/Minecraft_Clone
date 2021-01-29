#pragma once

#include <functional>
#include <string>

enum class EventType
{
	None = 0,
	KeyPressedEvent, KeyReleasedEvent, KeyTypedEvent,
	MouseMovedEvent, MouseScrolledEvent, MouseButtonPressedEvent, MouseButtonReleasedEvent,
	WindowResizedEvent, WindowClosedEvent
};

class Event
{
public:
	Event() {}
	~Event() {}

	virtual EventType GetType() const = 0;
	virtual std::string ToString() const = 0;

	bool Handled = false;
};

class EventDispatcher
{
public:
	EventDispatcher(Event& event)
		: m_Event(event)
	{
	}

	template<typename T>
	bool Dispatch(const std::function<bool(T&)>& fn)
	{
		if (m_Event.GetType() == T::GetStaticType())
		{
			m_Event.Handled = fn(static_cast<T&>(m_Event));
			return true;
		}

		return false;
	}
private:
	Event& m_Event;
};