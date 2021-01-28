#pragma once
#include <sstream>

#include "Event.h"

class WindowResizedEvent : public Event
{
public:
	WindowResizedEvent(uint32_t width, uint32_t height)
		: m_Width(width), m_Height(height)
	{
	}

	virtual EventType GetType() const override
	{
		return GetStaticType();
	}

	virtual std::string ToString() const override
	{
		std::stringstream ss;

		ss << "WindowResized: " << m_Width << ", " << m_Height;
		return ss.str();
	}

	static EventType GetStaticType()
	{
		return EventType::WindowResizedEvent;
	}

	inline uint32_t GetWidth() const { return m_Width; }
	inline uint32_t GetHeight() const { return m_Height; }
private:
	uint32_t m_Width;
	uint32_t m_Height;
};

class WindowClosedEvent : public Event
{
public:
	WindowClosedEvent()
	{
	}

	virtual EventType GetType() const override
	{
		return GetStaticType();
	}

	virtual std::string ToString() const override
	{
		return "WindowClosed";
	}

	static EventType GetStaticType()
	{
		return EventType::WindowClosedEvent;
	}
};