#pragma once
#include <core/events/event.h>
#include <core/events/keyCodes.h>

namespace luna {

	class keyEvent : public Event
	{
	public:
		keyCode getkeyCode() const { return m_keyCode; }

		EVENT_CLASS_CATEGORY(eventCategoryKeyboard | eventCategoryInput)
	protected:
		keyEvent(const keyCode keyCode)
			: m_keyCode(keyCode) {}

		keyCode m_keyCode;
	};

	class keyPressedEvent : public keyEvent
	{
	public:
		keyPressedEvent(const keyCode keyCode, bool isRepeat = false)
			: keyEvent(keyCode), m_IsRepeat(isRepeat) {}

		bool isRepeat() const { return m_IsRepeat; }

		std::string toString() const override
		{
			std::stringstream ss;
			ss << "KeyPressedEvent: " << m_keyCode << " (repeat = " << m_IsRepeat << ")";
			return ss.str();
		}

		EVENT_CLASS_TYPE(KeyPressed)
	private:
		bool m_IsRepeat;
	};

	class keyReleasedEvent : public keyEvent
	{
	public:
		keyReleasedEvent(const keyCode keyCode)
			: keyEvent(keyCode) {}

		std::string toString() const override
		{
			std::stringstream ss;
			ss << "keyReleasedEvent: " << m_keyCode;
			return ss.str();
		}

		EVENT_CLASS_TYPE(KeyReleased)
	};

	class keyTypedEvent : public keyEvent
	{
	public:
		keyTypedEvent(const keyCode keyCode)
			: keyEvent(keyCode) {}

		std::string toString() const override
		{
			std::stringstream ss;
			ss << "KeyTypedEvent: " << m_keyCode;
			return ss.str();
		}

		EVENT_CLASS_TYPE(KeyTyped)
	};
}