#pragma once

#include "Event.h"

namespace Crystal {

	class WindowResizeEvent : public Event /* Window Resize Event that extends/inherits from Event */
	{
	public:
		WindowResizeEvent(unsigned int width, unsigned int height) /* Window Resize Event constructor that takes in a width and height */
			: m_Width(width), m_Height(height) {}

		inline unsigned int GetWidth() const { return m_Width; } /* GetWidth Function that returns an unsigned int */
		inline unsigned int GetHeight() const { return m_Height; } /* GetHeight Function that returns and unsigned int */

		std::string ToString() const override /* Function for turning the event into a string */
		{
			std::stringstream ss; /* stringstream */
			ss << "WindowResizeEvent:" << m_Width << ", " << m_Height; /* width & height */
			return ss.str(); /* return the string */
		}

		EVENT_CLASS_TYPE(WindowResize) /* The window resize EVENT_CLASS_TYPE macro */
		EVENT_CLASS_CATEGORY(EventCategoryApplication) /* The macro EVENT_CLASS_CATEGORY with eventcategoryapplication*/
	private:
		unsigned int m_Width, m_Height; /* Unsigned int for width and height */
	};

	class WindowCloseEvent : public Event
	{
	public:
		WindowCloseEvent() {}

		EVENT_CLASS_TYPE(WindowClose)
		EVENT_CLASS_CATEGORY(EventCategoryApplication)
	};

	class AppTickEvent : public Event
	{
	public:
		AppTickEvent() {}

		EVENT_CLASS_TYPE(AppTick)
		EVENT_CLASS_CATEGORY(EventCategoryApplication)
	};

	class AppUpdateEvent : public Event
	{
	public:
		AppUpdateEvent() {}

		EVENT_CLASS_TYPE(AppUpdate)
		EVENT_CLASS_CATEGORY(EventCategoryApplication)
	};
}
