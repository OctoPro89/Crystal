#pragma once

#include <Crystal/Core/Core.h>
#include "Crystal/Core/KeyCodes.h"
#include "Crystal/Core/MouseCodes.h"


namespace Crystal {
	class Input /* Input class */
	{
	public: 
		static bool IsKeyPressed(KeyCode keycode); /* Static function for seeing if a key is pressed */

		static bool IsMouseButtonPressed(MouseCode button); /* Static function for seeing if a mouse button is pressed */
		static float GetMouseX(); /* Static function for getting the mouse's X position */
		static float GetMouseY(); /* Static function for getting the mouse's Y position */
		static std::pair<float, float> GetMousePosition(); /* std::pair ( two values ) to get the mouse's position together*/
	};
}
