using Crystal;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

class Bullet : Entity
{
	public Rigidbody2DComponent rb;
	public bool IsShooting;
	public void Create()
	{
		rb = FindEntityByName("Bullet").GetComponent<Rigidbody2DComponent>();
	}

	public void Shoot(Vector3 playerPosition)
	{
		IsShooting = true;
		rb.SetPosition(playerPosition, 0.0f);
		rb.ApplyForce(new Vector2(1000000000.0f, 0.0f), true);
	}
}
