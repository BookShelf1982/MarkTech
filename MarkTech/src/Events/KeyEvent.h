#pragma once
#include "Event.h"
#include <sstream>

namespace MarkTech {
	
	class MARKTECH_API KeyEvent : public Event
	{
	public:
		inline int GetKeyCode() const { return m_KeyCode; }

		EVENT_CLASS_CATEGORY(EventCategoryKeyboard | EventCategoryInput)

	protected:
		KeyEvent(int keycode)
			: m_KeyCode(keycode) {}

		int m_KeyCode;
	};

	class MARKTECH_API KeyPressedEvent : public KeyEvent
	{
	public:
		KeyPressedEvent(int keycode, int repeatCount)
			: KeyEvent(keycode), m_RepeatCount(repeatCount) {}

		inline int GetRepeatCount() const { return m_RepeatCount; }

		std::string ToString() const override
		{
			std::stringstream ss;
			ss << "KeyPressedEvent: " << m_KeyCode << " (" << m_RepeatCount << " repeats)";
			return ss.str();
		}

		EVENT_CLASS_TYPE(KeyPressed)

	private:
		int m_RepeatCount;
	};

	class MARKTECH_API KeyReleassedEvent : public KeyEvent
	{
	public:
		KeyReleassedEvent(int keycode)
			: KeyEvent(keycode) {}

		std::string ToString() const override
		{
			std::stringstream ss;
			ss << "KeyReleassedEvent: " << m_KeyCode;
			return ss.str();
		}

		EVENT_CLASS_TYPE(KeyPressed)
	};

}