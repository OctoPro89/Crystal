#pragma once

#include "Log.h"
#include "crystalpch.h"

#include "Crystal/Core/Core.h"
#include "Crystal/Events/Event.h"

namespace Crystal {
	struct WindowProps /* window props struct */
	{
		std::string Title;											/* title of the window */
		uint32_t Width;												/* width of the window */
		uint32_t Height;											/* height of the window */

		WindowProps(const std::string& title = "Crystal Engine",	/* constructor for window props that takes in a name, width, and height that set the private members */
			uint32_t width = 1920,
			uint32_t height = 1080)
			: Title(title), Width(width), Height(height) {} /* defaulting the title to the given one width to given one and height to given one */
	};

	//Interface representing a desktop system based window

	class Window																	/* window class*/
	{
	public:
		using EventCallbackFn = std::function<void(Event&)>;						/* using statement to make things quicker */

		virtual ~Window() {}														/* virtual deconstructor */
				
		virtual void OnUpdate() = 0;												/* pure virtual (needs to be implemented somewhere else) OnUpdate function */

		virtual uint32_t GetWidth() const = 0;										/* pure virtual GetWidth function that returns a uint32_t */
		virtual uint32_t GetHeight() const = 0;										/* pure virtual GetHeight function that returns a uint32_t */

		// Window Attributes
		virtual void SetEventCallback(const EventCallbackFn& callback) = 0;			/* pure virtual SetEvent callback function that takes in a const eventcallbackfn reference */
		virtual void SetVSync(bool enabled) = 0;									/* pure virtual SetVSync function that sets vsync on or off that takes in a boolean */
		virtual bool IsVSync() const = 0;											/* pure virtual function that returns a bool that is true if vsync is on and false if vsync is off */
		virtual void SetWindowTitle(std::string& title) = 0;						/* pure virtual function that sets the window's title that takes in a string reference to the wanted title */

		virtual void* GetNativeWindow() const = 0;									/* pure virtual function that returns a void pointer to the native window */

		static Scope<Window> Create(const WindowProps& props = WindowProps());		/* static Scope to a window that creates a window that takes in a window props */
	};
}
