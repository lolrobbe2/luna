#pragma once
#include <lnpch.h>
#include <core/events/event.h>
#include <core/events/mouseCodes.h>
namespace luna {

	class mouseMovedEvent : public Event
	{
	public:
		mouseMovedEvent(const float x, const float y)
			: m_MouseX(x), m_MouseY(y) {}

		float GetX() const { return m_MouseX; }
		float GetY() const { return m_MouseY; }

		std::string toString() const override
		{
			std::stringstream ss;
			ss << "MouseMovedEvent: " << m_MouseX << ", " << m_MouseY;
			return ss.str();
		}

		EVENT_CLASS_TYPE(MouseMoved)
			EVENT_CLASS_CATEGORY(eventCategoryMouse | eventCategoryInput)
	private:
		float m_MouseX, m_MouseY;
	};

	class mouseScrolledEvent : public Event
	{
	public:
		mouseScrolledEvent(const float xOffset, const float yOffset)
			: m_XOffset(xOffset), m_YOffset(yOffset) {}

		float getXOffset() const { return m_XOffset; }
		float getYOffset() const { return m_YOffset; }

		std::string toString() const override
		{
			std::stringstream ss;
			ss << "MouseScrolledEvent: " << getXOffset() << ", " << getYOffset();
			return ss.str();
		}

		EVENT_CLASS_TYPE(MouseScrolled)
			EVENT_CLASS_CATEGORY(eventCategoryMouse | eventCategoryInput)
	private:
		float m_XOffset, m_YOffset;
	};

	class mouseButtonEvent : public Event
	{
	public:
		mouseCode getMouseButton() const { return m_Button; }

		EVENT_CLASS_CATEGORY(eventCategoryMouse | eventCategoryInput | eventCategoryMouseButton)
	protected:
		mouseButtonEvent(const mouseCode button)
			: m_Button(button) {}

		mouseCode m_Button;
	};

	class mouseButtonPressedEvent : public mouseButtonEvent
	{
	public:
		mouseButtonPressedEvent(const mouseCode button,const bool doubleCLick)
			: mouseButtonEvent(button) {this->doubleClick = doubleClick;}

		bool isDoubleClick() { return doubleClick; }

		std::string toString() const override
		{
			std::stringstream ss;
			ss << "MouseButtonPressedEvent: " << m_Button;
			return ss.str();
		}

		EVENT_CLASS_TYPE(MouseButtonPressed)
		bool doubleClick = false;
	};

	class mouseButtonReleasedEvent : public mouseButtonEvent
	{
	public:
		mouseButtonReleasedEvent(const mouseCode button)
			: mouseButtonEvent(button) {}

		std::string toString() const override
		{
			std::stringstream ss;
			ss << "MouseButtonReleasedEvent: " << m_Button;
			return ss.str();
		}

		EVENT_CLASS_TYPE(MouseButtonReleased)
	};

}