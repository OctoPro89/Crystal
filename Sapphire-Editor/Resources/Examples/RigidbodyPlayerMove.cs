using Crystal;
public class RigidbodyPlayerMove : Entity
{
	private Rigidbody2DComponent m_Rigidbody;
	void OnCreate()
	{
		m_Rigidbody = GetComponent<Rigidbody2DComponent>();
	}
	void OnUpdate(float ts)
	{
		float speed = 100f;
		Vector3 velocity = Vector3.Zero;

		if (Input.IsKeyDown(KeyCode.W))
			velocity.Y = 1.0f;
		else if (Input.IsKeyDown(KeyCode.S))
			velocity.Y = -1.0f;

		if (Input.IsKeyDown(KeyCode.A))
			velocity.X = -1.0f;
		else if (Input.IsKeyDown(KeyCode.D))
			velocity.X = 1.0f;

		velocity *= (speed * ts);

		m_Rigidbody.ApplyLinearImpulse(velocity.XY, true);
	}
}
