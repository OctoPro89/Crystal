using System;
using Crystal;

namespace Sandbox
{
	public class Player : Entity
	{
		private Rigidbody2DComponent rb;
		void OnCreate()
		{
			rb = GetComponent<Rigidbody2DComponent>();
		}
		void OnUpdate(float ts)
		{
			float speed = 0.01f;
			Vector3 velocity = Vector3.Zero;

			if (Input.IsKeyDown(KeyCode.W))
				velocity.Y = 1.0f;
			else if (Input.IsKeyDown(KeyCode.S))
				velocity.Y = -1.0f;

			if (Input.IsKeyDown(KeyCode.A))
				velocity.X = -1.0f;
			else if (Input.IsKeyDown(KeyCode.D))
				velocity.X = 1.0f;

			velocity *= speed;
			rb.ApplyLinearImpulse(velocity.XY, true);
		}

	}
}