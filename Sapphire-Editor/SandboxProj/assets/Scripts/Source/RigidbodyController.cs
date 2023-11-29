using System;
using System.Collections.Generic;
using System.Linq;
using System.Security.Policy;
using System.Text;
using System.Threading.Tasks;
using Crystal;

namespace Sandbox
{
	public class RigidbodyController : Entity
	{
		private Rigidbody2DComponent m_Rigidbody;

		public float speed;
		public float torque;
		void OnCreate()
		{
			m_Rigidbody = GetComponent<Rigidbody2DComponent>();
		}

		public void Spin(float speed)
		{
			m_Rigidbody.ApplyTorque(speed, true);
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
			if(Input.IsKeyDown(KeyCode.E))
			{
				velocity.X = 0.8f;
			}

			velocity *= (speed * ts);

			m_Rigidbody.ApplyLinearImpulse(velocity.XY, true);
		}
	}
}
