using Crystal;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

public abstract class TankController : Entity
{
	public Rigidbody2DComponent rb;
	public TextRendererComponent textRenderer;

	public int playerNum = 1;
	public int playerHealth = 100;

	public void Create(Rigidbody2DComponent WheelRB, TextRendererComponent tr)
	{
		textRenderer = tr;
		rb = WheelRB;
	}

	public void Move(float ts, float speed)
	{
		Vector2 velocity = Vector2.Zero;

		if (playerNum == 1)
		{
			if (Input.IsKeyDown(KeyCode.A))
			{
				velocity.X = -1.0f;
			}

			if (Input.IsKeyDown(KeyCode.D))
			{
				velocity.X = 1.0f;
			}
		}
		else if (playerNum == 2)
		{
			if (Input.IsKeyDown(KeyCode.Left))
			{
				velocity.X = -1.0f;
			}

			if (Input.IsKeyDown(KeyCode.Right))
			{
				velocity.X = 1.0f;
			}
		}

		velocity *= (speed * ts);

		rb.ApplyLinearImpulse(velocity, true);
	}

	public void UpdateText()
	{
		textRenderer.Text = $"Player {playerNum} Health: {playerHealth}";
	}
}
