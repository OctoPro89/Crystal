using Crystal;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

class Player1 : TankController
{
	private AudioComponent m_Audio;
	private LineRendererComponent m_LineRenderer;
	private Bullet m_Bullet;

	public float speed;

	public float XOff;
	public float YOff;

	private float tempXOff;
	private float TurretAngle = 0.0f;

	private Entity turret;
	private RuntimeBullet bullet;
	private bool m_HasShot = false;

	void OnCreate()
	{
		tempXOff = XOff;
		m_Audio = GetComponent<AudioComponent>();
		m_LineRenderer = GetComponent<LineRendererComponent>();
		turret = FindEntityByName("Tank1Turret");
		bullet = FindEntityByName("Bullet").As<RuntimeBullet>();
		m_Bullet = new Bullet();

		Rigidbody2DComponent wheelRB = FindEntityByName("Wheel2").GetComponent<Rigidbody2DComponent>();
		TextRendererComponent textRenderer = FindEntityByName("Player1Health").GetComponent<TextRendererComponent>();
		this.Create(wheelRB, textRenderer);
		m_Bullet.Create();
	}

	void TurretShoot()
	{
		m_Audio.Play();
		
		m_Bullet.Shoot(new Vector3(this.Translation.X + 5.0f, this.Translation.Y, this.Translation.Z));
	}

	void TurretEndShoot()
	{
		tempXOff = XOff;
		m_HasShot = false;
	}

	void TurretUpdate(float ts)
	{
		// -- INPUT --

		if(Input.IsKeyDown(KeyCode.W))
		{
			TurretAngle += 0.35f;
		}

		else if (Input.IsKeyDown(KeyCode.S))
		{
			TurretAngle -= 0.35f;
		}

		if (Input.IsKeyDown(KeyCode.E) && !m_HasShot)
		{
			m_HasShot = true;
			TurretShoot();
			CrystalUtils.FuncAfterDelay(0.5f, TurretEndShoot);
		}

		// -- INPUT --

		// -- POSITIONING --

		Vector3 Rotation = new Vector3(this.Rotation.X, this.Rotation.Y, this.Rotation.Z + TurretAngle);
		Vector3 Pos = new Vector3(this.Translation.X + tempXOff, this.Translation.Y + YOff, -0.01f);
		Vector3 thisPos = new Vector3(this.Translation.X, this.Translation.Y, this.Translation.Z - 0.01f);

		turret.Rotation = Rotation;
		turret.Translation = Pos;

		// -- POSITIONING --

		/* TODO
		if (m_HasShot)
		{
			tempXOff = CrystalMath.Lerp(tempXOff, -0.25f, 0.03f);
		}
		*/

		if (bullet.HasHit)
		{
			m_LineRenderer.SetPositions(thisPos, new Vector3(bullet.info.XA, bullet.info.YA, -0.01f));

			bullet.HasHit = false;
			m_Bullet.IsShooting = false;
		}
		else if (m_Bullet.IsShooting)
		{
			m_LineRenderer.SetPositions(thisPos, new Vector3(bullet.Translation.X, bullet.Translation.Y, bullet.Translation.Z - 0.01f));
		}
	}

	void OnUpdate(float ts)
	{
		TurretUpdate(ts);

		this.UpdateText();
		this.Move(ts, speed);
	}
}
