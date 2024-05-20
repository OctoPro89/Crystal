using Crystal;
using System;

namespace Sandbox
{
	public class RigidbodyController : Entity
	{
		private Rigidbody2DComponent m_Rigidbody;
		private TextRendererComponent m_TextRenderer;

		public float speed;
		public float torque;

		void OnCreate()
		{
			m_Rigidbody = GetComponent<Rigidbody2DComponent>();
			m_TextRenderer = FindEntityByName("Velocity").GetComponent<TextRendererComponent>();
			Console.WriteLine("Player ID: " + this.ID);
		}

		void OnUpdate(float ts)
		{
			Vector3 velocity = Vector3.Zero;
			if (Input.IsKeyDown(KeyCode.W) || Input.IsKeyDown(KeyCode.Up))
			{
				velocity.Y = 1.0f;
				m_Rigidbody.ApplyTorque(torque, true);
			}
			else if (Input.IsKeyDown(KeyCode.S) || Input.IsKeyDown(KeyCode.Down))
			{
				velocity.Y = -1.0f;
				m_Rigidbody.ApplyTorque(torque, true);
			}

			if (Input.IsKeyDown(KeyCode.A) || Input.IsKeyDown(KeyCode.Left))
			{
				velocity.X = -1.0f;
				m_Rigidbody.ApplyTorque(torque, true);
			}
			else if (Input.IsKeyDown(KeyCode.D) || Input.IsKeyDown(KeyCode.Right))
			{
				velocity.X = 1.0f;
				m_Rigidbody.ApplyTorque(torque, true);
			}

			velocity *= (speed * ts);

			Editor.ClearConsole();

			Vector2 rbVelocity = m_Rigidbody.LinearVelocity;
			Editor.Log($"{rbVelocity.X}, {rbVelocity.Y}");

			m_TextRenderer.Text = $"Velocity: {rbVelocity.X.ToString("0.00")}, {rbVelocity.Y.ToString("0.00")}"; /* Format the string */

			m_Rigidbody.ApplyLinearImpulse(velocity.XY, true);
		}
	}
}
