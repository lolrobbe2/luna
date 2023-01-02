#pragma once

#include <core/events/event.h>

namespace luna {

	class windowResizeEvent : public Event
	{
	public:
		windowResizeEvent(unsigned int width, unsigned int height)
			: m_Width(width), m_Height(height) {}

		unsigned int getWidth() const { return m_Width; }
		unsigned int getHeight() const { return m_Height; }

		std::string toString() const override
		{
			std::stringstream ss;
			ss << "WindowResizeEvent: " << m_Width << ", " << m_Height;
			return ss.str();
		}

		EVENT_CLASS_TYPE(WindowResize)
		EVENT_CLASS_CATEGORY(eventCategoryApplication)
	private:
		unsigned int m_Width, m_Height;
	};

	class windowCloseEvent : public Event
	{
	public:
		windowCloseEvent() = default;

		EVENT_CLASS_TYPE(WindowClose)
			EVENT_CLASS_CATEGORY(eventCategoryApplication)
	};

	class windowFocusEvent : public Event
	{
	public:
		windowFocusEvent() = default;
		EVENT_CLASS_TYPE(WindowFocus)
			EVENT_CLASS_CATEGORY(eventCategoryApplication)
	};

	class windowLostFocusEvent : public Event
	{
	public:
		windowLostFocusEvent() = default;

		EVENT_CLASS_TYPE(WindowLostFocus)
			EVENT_CLASS_CATEGORY(eventCategoryApplication)
	};

	class appTickEvent : public Event
	{
	public:
		appTickEvent() = default;

		EVENT_CLASS_TYPE(AppTick)
			EVENT_CLASS_CATEGORY(eventCategoryApplication)
	};

	class appUpdateEvent : public Event
	{
	public:
		appUpdateEvent() = default;

		EVENT_CLASS_TYPE(AppUpdate)
			EVENT_CLASS_CATEGORY(eventCategoryApplication)
	};

	class appRenderEvent : public Event
	{
	public:
		appRenderEvent() = default;

		EVENT_CLASS_TYPE(AppRender)
			EVENT_CLASS_CATEGORY(eventCategoryApplication)
	};
}