#include "crystalpch.h"
#include "Crystal/Core/Window.h"

#ifdef CRYSTAL_PLATFORM_WINDOWS
	#include "Platform/Windows/WindowsWindow.h"				 /* include windows window if the platform is windows */
#endif

namespace Crystal
{
	Scope<Window> Window::Create(const WindowProps& props) 
	{
		#ifdef CRYSTAL_PLATFORM_WINDOWS
			return CreateScope<WindowsWindow>(props);		 /* give back a CreateScope(unique pointer) to a windows window if that's the platform*/
		#else
			CRYSTAL_CORE_ASSERT(false, "Unknown platform!"); /* otherwise throw error and assert */
			return nullptr;									 /* return nothing */
		#endif
	}
}
