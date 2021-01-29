#pragma once
#include <sstream>

#include "Event.h"

class KeyPressedEvent : public Event
{
public:
	KeyPressedEvent(int keycode, int repeatCount)
		: m_KeyCode(keycode), m_RepeatCount(repeatCount)
	{}

	virtual EventType GetType() const override
	{
		return GetStaticType();
	}

	virtual std::string ToString() const override
	{
		std::stringstream ss;

		ss << "KeyPressed: " << m_KeyCode;
		return ss.str();
	}

	static EventType GetStaticType()
	{
		return EventType::KeyPressedEvent;
	}

	inline int GetKeyCode() const { return m_KeyCode; }
	inline int GetRepeatCount() const { return m_RepeatCount; }

private:
	int m_KeyCode;
	int m_RepeatCount;
};

class KeyReleasedEvent : public Event
{
public:
	KeyReleasedEvent(int keycode)
		: m_KeyCode(keycode)
	{}

	virtual EventType GetType() const override
	{
		return GetStaticType();
	}

	virtual std::string ToString() const override
	{
		std::stringstream ss;

		ss << "KeyReleased: " << m_KeyCode;
		return ss.str();
	}

	static EventType GetStaticType()
	{
		return EventType::KeyReleasedEvent;
	}

	inline int GetKeyCode() const { return m_KeyCode; }

private:
	int m_KeyCode;
};

class KeyTypedEvent : public Event
{
public:
	KeyTypedEvent(int keycode)
		: m_KeyCode(keycode)
	{}

	virtual EventType GetType() const override
	{
		return GetStaticType();
	}

	virtual std::string ToString() const override
	{
		std::stringstream ss;

		ss << "KeyTyped: " << m_KeyCode;
		return ss.str();
	}

	static EventType GetStaticType()
	{
		return EventType::KeyTypedEvent;
	}

	inline int GetKeyCode() const { return m_KeyCode; }

private:
	int m_KeyCode;
};