#pragma once

namespace Crystal
{
	typedef enum class KeyCode : uint16_t
	{
		// From glfw3.h
		Space = 32,
		Apostrophe = 39, /* ' */
		Comma = 44, /* , */
		Minus = 45, /* - */
		Period = 46, /* . */
		Slash = 47, /* / */

		D0 = 48, /* 0 */
		D1 = 49, /* 1 */
		D2 = 50, /* 2 */
		D3 = 51, /* 3 */
		D4 = 52, /* 4 */
		D5 = 53, /* 5 */
		D6 = 54, /* 6 */
		D7 = 55, /* 7 */
		D8 = 56, /* 8 */
		D9 = 57, /* 9 */

		Semicolon = 59, /* ; */
		Equal = 61, /* = */

		A = 65,
		B = 66,
		C = 67,
		D = 68,
		E = 69,
		F = 70,
		G = 71,
		H = 72,
		I = 73,
		J = 74,
		K = 75,
		L = 76,
		M = 77,
		N = 78,
		O = 79,
		P = 80,
		Q = 81,
		R = 82,
		S = 83,
		T = 84,
		U = 85,
		V = 86,
		W = 87,
		X = 88,
		Y = 89,
		Z = 90,

		LeftBracket = 91,  /* [ */
		Backslash = 92,  /* \ */
		RightBracket = 93,  /* ] */
		GraveAccent = 96,  /* ` */

		World1 = 161, /* non-US #1 */
		World2 = 162, /* non-US #2 */

		/* Function keys */
		Escape = 256,
		Enter = 257,
		Tab = 258,
		Backspace = 259,
		Insert = 260,
		Delete = 261,
		Right = 262,
		Left = 263,
		Down = 264,
		Up = 265,
		PageUp = 266,
		PageDown = 267,
		Home = 268,
		End = 269,
		CapsLock = 280,
		ScrollLock = 281,
		NumLock = 282,
		PrintScreen = 283,
		Pause = 284,
		F1 = 290,
		F2 = 291,
		F3 = 292,
		F4 = 293,
		F5 = 294,
		F6 = 295,
		F7 = 296,
		F8 = 297,
		F9 = 298,
		F10 = 299,
		F11 = 300,
		F12 = 301,
		F13 = 302,
		F14 = 303,
		F15 = 304,
		F16 = 305,
		F17 = 306,
		F18 = 307,
		F19 = 308,
		F20 = 309,
		F21 = 310,
		F22 = 311,
		F23 = 312,
		F24 = 313,
		F25 = 314,

		/* Keypad */
		KP0 = 320,
		KP1 = 321,
		KP2 = 322,
		KP3 = 323,
		KP4 = 324,
		KP5 = 325,
		KP6 = 326,
		KP7 = 327,
		KP8 = 328,
		KP9 = 329,
		KPDecimal = 330,
		KPDivide = 331,
		KPMultiply = 332,
		KPSubtract = 333,
		KPAdd = 334,
		KPEnter = 335,
		KPEqual = 336,

		LeftShift = 340,
		LeftControl = 341,
		LeftAlt = 342,
		LeftSuper = 343,
		RightShift = 344,
		RightControl = 345,
		RightAlt = 346,
		RightSuper = 347,
		Menu = 348
	} Key;

	inline std::ostream& operator<<(std::ostream& os, KeyCode keyCode)
	{
		os << static_cast<int32_t>(keyCode);
		return os;
	}
}

// From glfw3.h
#define CRYSTAL_KEY_SPACE           ::Crystal::Key::Space
#define CRYSTAL_KEY_APOSTROPHE      ::Crystal::Key::Apostrophe    /* ' */
#define CRYSTAL_KEY_COMMA           ::Crystal::Key::Comma         /* , */
#define CRYSTAL_KEY_MINUS           ::Crystal::Key::Minus         /* - */
#define CRYSTAL_KEY_PERIOD          ::Crystal::Key::Period        /* . */
#define CRYSTAL_KEY_SLASH           ::Crystal::Key::Slash         /* / */
#define CRYSTAL_KEY_0               ::Crystal::Key::D0
#define CRYSTAL_KEY_1               ::Crystal::Key::D1
#define CRYSTAL_KEY_2               ::Crystal::Key::D2
#define CRYSTAL_KEY_3               ::Crystal::Key::D3
#define CRYSTAL_KEY_4               ::Crystal::Key::D4
#define CRYSTAL_KEY_5               ::Crystal::Key::D5
#define CRYSTAL_KEY_6               ::Crystal::Key::D6
#define CRYSTAL_KEY_7               ::Crystal::Key::D7
#define CRYSTAL_KEY_8               ::Crystal::Key::D8
#define CRYSTAL_KEY_9               ::Crystal::Key::D9
#define CRYSTAL_KEY_SEMICOLON       ::Crystal::Key::Semicolon     /* ; */
#define CRYSTAL_KEY_EQUAL           ::Crystal::Key::Equal         /* = */
#define CRYSTAL_KEY_A               ::Crystal::Key::A
#define CRYSTAL_KEY_B               ::Crystal::Key::B
#define CRYSTAL_KEY_C               ::Crystal::Key::C
#define CRYSTAL_KEY_D               ::Crystal::Key::D
#define CRYSTAL_KEY_E               ::Crystal::Key::E
#define CRYSTAL_KEY_F               ::Crystal::Key::F
#define CRYSTAL_KEY_G               ::Crystal::Key::G
#define CRYSTAL_KEY_H               ::Crystal::Key::H
#define CRYSTAL_KEY_I               ::Crystal::Key::I
#define CRYSTAL_KEY_J               ::Crystal::Key::J
#define CRYSTAL_KEY_K               ::Crystal::Key::K
#define CRYSTAL_KEY_L               ::Crystal::Key::L
#define CRYSTAL_KEY_M               ::Crystal::Key::M
#define CRYSTAL_KEY_N               ::Crystal::Key::N
#define CRYSTAL_KEY_O               ::Crystal::Key::O
#define CRYSTAL_KEY_P               ::Crystal::Key::P
#define CRYSTAL_KEY_Q               ::Crystal::Key::Q
#define CRYSTAL_KEY_R               ::Crystal::Key::R
#define CRYSTAL_KEY_S               ::Crystal::Key::S
#define CRYSTAL_KEY_T               ::Crystal::Key::T
#define CRYSTAL_KEY_U               ::Crystal::Key::U
#define CRYSTAL_KEY_V               ::Crystal::Key::V
#define CRYSTAL_KEY_W               ::Crystal::Key::W
#define CRYSTAL_KEY_X               ::Crystal::Key::X
#define CRYSTAL_KEY_Y               ::Crystal::Key::Y
#define CRYSTAL_KEY_Z               ::Crystal::Key::Z
#define CRYSTAL_KEY_LEFT_BRACKET    ::Crystal::Key::LeftBracket   /* [ */
#define CRYSTAL_KEY_BACKSLASH       ::Crystal::Key::Backslash     /* \ */
#define CRYSTAL_KEY_RIGHT_BRACKET   ::Crystal::Key::RightBracket  /* ] */
#define CRYSTAL_KEY_GRAVE_ACCENT    ::Crystal::Key::GraveAccent   /* ` */
#define CRYSTAL_KEY_WORLD_1         ::Crystal::Key::World1        /* non-US #1 */
#define CRYSTAL_KEY_WORLD_2         ::Crystal::Key::World2        /* non-US #2 */

/* Function keys */
#define CRYSTAL_KEY_ESCAPE          ::Crystal::Key::Escape
#define CRYSTAL_KEY_ENTER           ::Crystal::Key::Enter
#define CRYSTAL_KEY_TAB             ::Crystal::Key::Tab
#define CRYSTAL_KEY_BACKSPACE       ::Crystal::Key::Backspace
#define CRYSTAL_KEY_INSERT          ::Crystal::Key::Insert
#define CRYSTAL_KEY_DELETE          ::Crystal::Key::Delete
#define CRYSTAL_KEY_RIGHT           ::Crystal::Key::Right
#define CRYSTAL_KEY_LEFT            ::Crystal::Key::Left
#define CRYSTAL_KEY_DOWN            ::Crystal::Key::Down
#define CRYSTAL_KEY_UP              ::Crystal::Key::Up
#define CRYSTAL_KEY_PAGE_UP         ::Crystal::Key::PageUp
#define CRYSTAL_KEY_PAGE_DOWN       ::Crystal::Key::PageDown
#define CRYSTAL_KEY_HOME            ::Crystal::Key::Home
#define CRYSTAL_KEY_END             ::Crystal::Key::End
#define CRYSTAL_KEY_CAPS_LOCK       ::Crystal::Key::CapsLock
#define CRYSTAL_KEY_SCROLL_LOCK     ::Crystal::Key::ScrollLock
#define CRYSTAL_KEY_NUM_LOCK        ::Crystal::Key::NumLock
#define CRYSTAL_KEY_PRINT_SCREEN    ::Crystal::Key::PrintScreen
#define CRYSTAL_KEY_PAUSE           ::Crystal::Key::Pause
#define CRYSTAL_KEY_F1              ::Crystal::Key::F1
#define CRYSTAL_KEY_F2              ::Crystal::Key::F2
#define CRYSTAL_KEY_F3              ::Crystal::Key::F3
#define CRYSTAL_KEY_F4              ::Crystal::Key::F4
#define CRYSTAL_KEY_F5              ::Crystal::Key::F5
#define CRYSTAL_KEY_F6              ::Crystal::Key::F6
#define CRYSTAL_KEY_F7              ::Crystal::Key::F7
#define CRYSTAL_KEY_F8              ::Crystal::Key::F8
#define CRYSTAL_KEY_F9              ::Crystal::Key::F9
#define CRYSTAL_KEY_F10             ::Crystal::Key::F10
#define CRYSTAL_KEY_F11             ::Crystal::Key::F11
#define CRYSTAL_KEY_F12             ::Crystal::Key::F12
#define CRYSTAL_KEY_F13             ::Crystal::Key::F13
#define CRYSTAL_KEY_F14             ::Crystal::Key::F14
#define CRYSTAL_KEY_F15             ::Crystal::Key::F15
#define CRYSTAL_KEY_F16             ::Crystal::Key::F16
#define CRYSTAL_KEY_F17             ::Crystal::Key::F17
#define CRYSTAL_KEY_F18             ::Crystal::Key::F18
#define CRYSTAL_KEY_F19             ::Crystal::Key::F19
#define CRYSTAL_KEY_F20             ::Crystal::Key::F20
#define CRYSTAL_KEY_F21             ::Crystal::Key::F21
#define CRYSTAL_KEY_F22             ::Crystal::Key::F22
#define CRYSTAL_KEY_F23             ::Crystal::Key::F23
#define CRYSTAL_KEY_F24             ::Crystal::Key::F24
#define CRYSTAL_KEY_F25             ::Crystal::Key::F25

/* Keypad */
#define CRYSTAL_KEY_KP_0            ::Crystal::Key::KP0
#define CRYSTAL_KEY_KP_1            ::Crystal::Key::KP1
#define CRYSTAL_KEY_KP_2            ::Crystal::Key::KP2
#define CRYSTAL_KEY_KP_3            ::Crystal::Key::KP3
#define CRYSTAL_KEY_KP_4            ::Crystal::Key::KP4
#define CRYSTAL_KEY_KP_5            ::Crystal::Key::KP5
#define CRYSTAL_KEY_KP_6            ::Crystal::Key::KP6
#define CRYSTAL_KEY_KP_7            ::Crystal::Key::KP7
#define CRYSTAL_KEY_KP_8            ::Crystal::Key::KP8
#define CRYSTAL_KEY_KP_9            ::Crystal::Key::KP9
#define CRYSTAL_KEY_KP_DECIMAL      ::Crystal::Key::KPDecimal
#define CRYSTAL_KEY_KP_DIVIDE       ::Crystal::Key::KPDivide
#define CRYSTAL_KEY_KP_MULTIPLY     ::Crystal::Key::KPMultiply
#define CRYSTAL_KEY_KP_SUBTRACT     ::Crystal::Key::KPSubtract
#define CRYSTAL_KEY_KP_ADD          ::Crystal::Key::KPAdd
#define CRYSTAL_KEY_KP_ENTER        ::Crystal::Key::KPEnter
#define CRYSTAL_KEY_KP_EQUAL        ::Crystal::Key::KPEqual

#define CRYSTAL_KEY_LEFT_SHIFT      ::Crystal::Key::LeftShift
#define CRYSTAL_KEY_LEFT_CONTROL    ::Crystal::Key::LeftControl
#define CRYSTAL_KEY_LEFT_ALT        ::Crystal::Key::LeftAlt
#define CRYSTAL_KEY_LEFT_SUPER      ::Crystal::Key::LeftSuper
#define CRYSTAL_KEY_RIGHT_SHIFT     ::Crystal::Key::RightShift
#define CRYSTAL_KEY_RIGHT_CONTROL   ::Crystal::Key::RightControl
#define CRYSTAL_KEY_RIGHT_ALT       ::Crystal::Key::RightAlt
#define CRYSTAL_KEY_RIGHT_SUPER     ::Crystal::Key::RightSuper
#define CRYSTAL_KEY_MENU            ::Crystal::Key::Menu