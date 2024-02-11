using Crystal;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

class Player1 : TankController
{
	private AudioComponent m_Audio;

	public float speed;

	public float XOff;
	public float YOff;

	private float tempXOff;
	private float TurretAngle = 0.0f;

	private Entity turret;
	private bool m_HasShot = false;

	void OnCreate()
	{
		tempXOff = XOff;
		m_Audio = GetComponent<AudioComponent>();
		turret = FindEntityByName("Tank1Turret");

		Rigidbody2DComponent wheelRB = FindEntityByName("Wheel2").GetComponent<Rigidbody2DComponent>();
		this.Create(wheelRB);
	}

	void TurretShoot()
	{
		m_Audio.Play();
	}

	void TurretEndShoot()
	{
		tempXOff = XOff;
		m_HasShot = false;
	}

	void TurretUpdate(float ts)
	{

		Vector3 Rotation = new Vector3(this.Rotation.X, this.Rotation.Y, this.Rotation.Z);
		turret.Rotation = Rotation;

		Vector3 Pos = new Vector3(this.Translation.X + tempXOff, this.Translation.Y + YOff, -0.01f);
		turret.Translation = Pos;

		if(Input.IsKeyDown(KeyCode.W))
		{
			TurretAngle += 1.0f;
		}

		if (Input.IsKeyDown(KeyCode.S))
		{
			TurretAngle -= 1.0f;
		}

		if (Input.IsKeyDown(KeyCode.E) && !m_HasShot)
		{
			m_HasShot = true;
			TurretShoot();
			CrystalUtils.FuncAfterDelay(0.5f, TurretEndShoot);
		}

		if(m_HasShot)
		{
			tempXOff = CrystalMath.Lerp(tempXOff, -0.25f, 0.03f);
		}
	}

	void OnUpdate(float ts)
	{
		TurretUpdate(ts);
		this.Move(ts, speed);
	}	
}
