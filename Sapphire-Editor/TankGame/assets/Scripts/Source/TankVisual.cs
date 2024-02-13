using Crystal;
public class TankVisual : Entity
{
	private SpriteRendererComponent sprite;

	public int health = 100;

	public void Create(SpriteRendererComponent Sprite)
	{
		sprite = Sprite;
	}

	public void TakeDamage(int damage)
	{
		sprite.Color = new Vector4(255, sprite.Color.Y - damage, sprite.Color.Z - damage, 255);
		health -= damage;
	}
}
