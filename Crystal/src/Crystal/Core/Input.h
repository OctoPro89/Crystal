#pragma once

#include <Crystal/Core/Core.h>
#include "Crystal/Core/KeyCodes.h"
#include "Crystal/Core/MouseCodes.h"


namespace Crystal {
	class Input
	{
	public: 
		static bool IsKeyPressed(KeyCode keycode);

		static bool IsMouseButtonPressed(MouseCode button);
		static float GetMouseX();
		static float GetMouseY();
		static std::pair<float, float> GetMousePosition();
	};
}