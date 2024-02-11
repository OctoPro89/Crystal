using Crystal;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

public abstract class TankController : Entity
{
	public Rigidbody2DComponent rb;

	public void Create()
	{
		rb = GetComponent<Rigidbody2DComponent>();
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

		rb.ApplyLinearImpulse(velocity, true);
	}
}
