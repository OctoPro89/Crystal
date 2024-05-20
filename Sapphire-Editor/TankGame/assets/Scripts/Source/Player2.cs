using Crystal;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

class Player2 : TankController
{
	private AudioComponent m_Audio;
	private SpriteRendererComponent m_Sprite;
	private LineRendererComponent m_LineRenderer;
	//private TankVisual m_Visual;

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

		//m_Visual = new TankVisual();
		m_Audio = GetComponent<AudioComponent>();
		turret = FindEntityByName("Tank2Turret");
		m_Sprite = GetComponent<SpriteRendererComponent>();
		m_LineRenderer = GetComponent<LineRendererComponent>();

		Rigidbody2DComponent wheelRB = FindEntityByName("Wheel2 Copy").GetComponent<Rigidbody2DComponent>();

		//m_Visual.Create(m_Sprite);
		this.playerNum = 2;
		TextRendererComponent textRenderer = FindEntityByName("Player2Health").GetComponent<TextRendererComponent>();
		this.Create(wheelRB, textRenderer);
	}

	void TurretShoot()
	{
		m_Audio.Play();
		//m_Visual.TakeDamage(17);
		//m_LineRenderer.SetPositions(turret.Translation, new Vector3(turret.Translation.X - 100.0f, turret.Translation.Y, turret.Translation.Z));
	}

	void TurretEndShoot()
	{
		tempXOff = XOff;
		m_HasShot = false;
	}

	void TurretUpdate(float ts)
	{
		Vector3 Rotation = new Vector3(this.Rotation.X, this.Rotation.Y, this.Rotation.Z - 180.0f);
		turret.Rotation = Rotation;

		Vector3 Pos = new Vector3(this.Translation.X + tempXOff, this.Translation.Y + YOff, -0.01f);
		turret.Translation = Pos;

		if (Input.IsKeyDown(KeyCode.Up))
		{
			TurretAngle += 1.0f;
		}

		if (Input.IsKeyDown(KeyCode.Down))
		{
			TurretAngle -= 1.0f;
		}

		if (Input.IsKeyDown(KeyCode.Slash) && !m_HasShot)
		{
			m_HasShot = true;
			TurretShoot();
			CrystalUtils.FuncAfterDelay(0.5f, TurretEndShoot);
		}

		if (m_HasShot)
		{
			tempXOff = CrystalMath.Lerp(tempXOff, 0.25f, 0.03f);
		}
	}

	void OnUpdate(float ts)
	{
		TurretUpdate(ts);

		this.UpdateText();
		this.Move(ts, speed);
	}
}
