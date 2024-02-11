using Crystal;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

public abstract class TankController : Entity
{
	public Rigidbody2DComponent rb;

	public void Create(Rigidbody2DComponent WheelRB)
	{
		rb = WheelRB;
	}

	public void Move(float ts, float speed)
	{
		Vector2 velocity = Vector2.Zero;

		if (Input.IsKeyDown(KeyCode.A))
		{
			velocity.X = -1.0f;
		}

		if (Input.IsKeyDown(KeyCode.D))
		{
			velocity.X = 1.0f;
		}

		velocity *= (speed * ts);

		Editor.ClearConsole();
		Editor.Log($"{rb.LinearVelocity.X}, {rb.LinearVelocity.Y}");

		if (rb.LinearVelocity.X > 10.0f)
		{
			rb.LinearVelocity = 10.0f;
		}

		rb.ApplyLinearImpulse(velocity, true);
	}
}
