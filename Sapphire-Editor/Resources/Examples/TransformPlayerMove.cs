using Crystal;
class TransformMove : Entity
{
	private float speed = 15.0f;
	public Vector3 transform;
	public TransformComponent tc;
	void OnCreate()
	{
		tc = GetComponent<TransformComponent>();
	}

	void OnUpdate()
	{
		if (Input.IsKeyDown(KeyCode.W))
			transform.Y += speed;
		if (Input.IsKeyDown(KeyCode.S))
			transform.Y -= speed;
		if (Input.IsKeyDown(KeyCode.A))
			transform.X += speed;
		if (Input.IsKeyDown(KeyCode.D))
			transform.X -= speed;

		tc.Translation = transform;
	}
}
