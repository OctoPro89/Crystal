using Crystal;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

class Player : Entity
{
	public float speed;
	public float jumpForce;
	private Rigidbody2DComponent rb;
	private BoxCollider2DComponent bc2d;
	private bool isJumping = false;
	private int numOfAllowedJumps = 2;
	private int numOfJumps = 0;
	void OnCreate()
	{
		rb = GetComponent<Rigidbody2DComponent>();
		bc2d = GetComponent<BoxCollider2DComponent>();
	}
	void OnUpdate(float ts)
	{
		Vector3 velocity = Vector3.Zero;
		if (Input.IsKeyDown(KeyCode.W) && isJumping == false)
		{
			velocity.Y += jumpForce;
			isJumping = true;
		}
		if (Input.IsKeyDown(KeyCode.D) || Input.IsKeyDown(KeyCode.Right))
		{
			velocity.X = 1.0f;
		}
		if (Input.IsKeyDown(KeyCode.A) || Input.IsKeyDown(KeyCode.Left))
		{
			velocity.X = -1.0f;
		}
		velocity *= (speed * ts);

		rb.ApplyLinearImpulse(velocity.XY, true);
	}
	void OnCollisionEnter(HitInfo hit)
	{
		isJumping = false;
	}
}
