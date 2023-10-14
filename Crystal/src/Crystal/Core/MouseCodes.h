#pragma once

namespace Crystal
{
	typedef enum class MouseCode : uint16_t
	{
		// From glfw3.h
		Button0 = 0,
		Button1 = 1,
		Button2 = 2,
		Button3 = 3,
		Button4 = 4,
		Button5 = 5,
		Button6 = 6,
		Button7 = 7,

		ButtonLast = Button7,
		ButtonLeft = Button0,
		ButtonRight = Button1,
		ButtonMiddle = Button2
	} Mouse;

	inline std::ostream& operator<<(std::ostream& os, MouseCode mouseCode)
	{
		os << static_cast<int32_t>(mouseCode);
		return os;
	}
}

#define CRYSTAL_MOUSE_BUTTON_0      ::Crystal::Mouse::Button0
#define CRYSTAL_MOUSE_BUTTON_1      ::Crystal::Mouse::Button1
#define CRYSTAL_MOUSE_BUTTON_2      ::Crystal::Mouse::Button2
#define CRYSTAL_MOUSE_BUTTON_3      ::Crystal::Mouse::Button3
#define CRYSTAL_MOUSE_BUTTON_4      ::Crystal::Mouse::Button4
#define CRYSTAL_MOUSE_BUTTON_5      ::Crystal::Mouse::Button5
#define CRYSTAL_MOUSE_BUTTON_6      ::Crystal::Mouse::Button6
#define CRYSTAL_MOUSE_BUTTON_7      ::Crystal::Mouse::Button7
#define CRYSTAL_MOUSE_BUTTON_LAST   ::Crystal::Mouse::ButtonLast
#define CRYSTAL_MOUSE_BUTTON_LEFT   ::Crystal::Mouse::ButtonLeft
#define CRYSTAL_MOUSE_BUTTON_RIGHT  ::Crystal::Mouse::ButtonRight
#define CRYSTAL_MOUSE_BUTTON_MIDDLE ::Crystal::Mouse::ButtonMiddle