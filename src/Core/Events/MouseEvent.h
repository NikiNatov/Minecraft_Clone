#pragma once
#include <tuple>
#include <sstream>

#include "Event.h"

class MouseMovedEvent : public Event
{
public:
	MouseMovedEvent(float x, float y)
		: m_XPos(x), m_YPos(y)
	{
	}

	virtual EventType GetType() const override
	{
		return GetStaticType();
	}

	virtual std::string ToString() const override
	{
		std::stringstream ss;

		ss << "MouseMoved: " << m_XPos << ", " << m_YPos;
		return ss.str();
	}

	static EventType GetStaticType()
	{
		return EventType::MouseMovedEvent;
	}

	inline float GetMouseX() const { return m_XPos; }
	inline float GetMouseY() const { return m_YPos; }
	inline std::pair<float, float> GetMousePosition() const { return { m_XPos, m_YPos }; }
private:
	float m_XPos;
	float m_YPos;
};

class MouseScrolledEvent : public Event
{
public:
	MouseScrolledEvent(float XOffset, float YOffset)
		: m_XOffset(XOffset), m_YOffset(YOffset)
	{
	}

	virtual EventType GetType() const override
	{
		return GetStaticType();
	}

	virtual std::string ToString() const override
	{
		std::stringstream ss;

		ss << "MouseScrolled: " << m_XOffset << ", " << m_YOffset;
		return ss.str();
	}

	static EventType GetStaticType()
	{
		return EventType::MouseScrolledEvent;
	}

	inline float GetMouseXOffset() const { return m_XOffset; }
	inline float GetMouseYOffset() const { return m_YOffset; }
private:
	float m_XOffset;
	float m_YOffset;
};

class MouseButtonPressedEvent : public Event
{
public:
	MouseButtonPressedEvent(int button)
		: m_Button(button)
	{
	}

	virtual EventType GetType() const override
	{
		return GetStaticType();
	}

	virtual std::string ToString() const override
	{
		std::stringstream ss;

		ss << "MouseButtonPressed: " << m_Button;
		return ss.str();
	}

	static EventType GetStaticType()
	{
		return EventType::MouseButtonPressedEvent;
	}

	inline int GetButton() const { return m_Button; }

private:
	int m_Button;
};

class MouseButtonReleasedEvent : public Event
{
public:
	MouseButtonReleasedEvent(int button)
		: m_Button(button)
	{
	}

	virtual EventType GetType() const override
	{
		return GetStaticType();
	}

	virtual std::string ToString() const override
	{
		std::stringstream ss;

		ss << "MouseButtonReleased: " << m_Button;
		return ss.str();
	}

	static EventType GetStaticType()
	{
		return EventType::MouseButtonReleasedEvent;
	}

	inline int GetButton() const { return m_Button; }

private:
	int m_Button;
};