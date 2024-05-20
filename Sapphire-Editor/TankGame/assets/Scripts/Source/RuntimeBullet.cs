using Crystal;

class RuntimeBullet : Entity
{
	LineRendererComponent lr;

	public HitInfo info;
	public bool HasHit = false;

	void Create(LineRendererComponent lineRenderer)
	{
		lr = lineRenderer;
	}

	void OnCollisionEnter(HitInfo hitInfo)
	{
		info = hitInfo;
		HasHit = true;
	}
}
