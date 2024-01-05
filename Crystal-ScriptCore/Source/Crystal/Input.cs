using System.Threading;

namespace Crystal
{
	public class Input
	{
		public static bool IsKeyDown(KeyCode keycode)
		{
			return InternalCalls.Input_IsKeyDown(keycode);
		}

		public static Vector2 GetMousePosition()
		{
			InternalCalls.Input_GetMousePos(out Vector2 mousePos);
			return mousePos;
		}

		public static float GetMouseX() 
		{
			InternalCalls.Input_GetMouseX(out float mouseX);
			return mouseX;
		}

		public static float GetMouseY()
		{
			InternalCalls.Input_GetMouseY(out float mouseY);
			return mouseY;
		}
	}
}
