#include "crystalpch.h"
#include "Crystal/Core/Window.h"

#ifdef CRYSTAL_PLATFORM_WINDOWS
	#include "Platform/Windows/WindowsWindow.h"
#endif

namespace Crystal
{
	Scope<Window> Window::Create(const WindowProps& props)
	{
		#ifdef CRYSTAL_PLATFORM_WINDOWS
			return CreateScope<WindowsWindow>(props);
		#else
			CRYSTAL_CORE_ASSERT(false, "Unknown platform!");
			return nullptr;
		#endif
	}
}