using Crystal;
namespace Tests
{
	class MouseTest : Entity
	{
		void OnUpdate(float ts)
		{
			Vector2 pos = Input.GetMousePosition();
			Editor.Log($"{pos.X}, {pos.Y}");
		}
	}
}
