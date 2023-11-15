using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace Crystal
{
	public abstract class Component
	{
		public Entity Entity { get; internal set; }
	}

	public class SpriteRendererComponent : Component
	{
		public Vector4 Color
		{
			get
			{
				InternalCalls.SpriteRendererComponent_GetColor(Entity.ID, out Vector4 color);
				return color;
			}
			set
			{
				InternalCalls.SpriteRendererComponent_SetColor(Entity.ID, ref value);
			}
		}
		public float AtlasCoordsX
		{
			get
			{
				InternalCalls.SpriteRendererComponent_GetTexIndex(Entity.ID, out Vector2 coords);
				return coords.X;
			}
			set
			{
				InternalCalls.SpriteRendererComponent_SetTexIndexX(Entity.ID, ref value);
			}
		}
		
		public float AtlasCoordsY
		{
			get
			{
				InternalCalls.SpriteRendererComponent_GetTexIndex(Entity.ID, out Vector2 coords);
				return coords.Y;
			}
			set
			{
				InternalCalls.SpriteRendererComponent_SetTexIndexY(Entity.ID, ref value);
			}
		}
	}

	public class BoxCollider2DComponent : Component
	{

	}

	public class TransformComponent : Component
	{
		public Vector3 Translation
		{
			get
			{
				InternalCalls.TransformComponent_GetTranslation(Entity.ID, out Vector3 translation);
				return translation;
			}
			set
			{
				InternalCalls.TransformComponent_SetTranslation(Entity.ID, ref value);
			}
		}
	}

	public class Rigidbody2DComponent : Component
	{

		public void ApplyLinearImpulse(Vector2 impulse, Vector2 worldPosition, bool wake)
		{
			InternalCalls.Rigidbody2DComponent_ApplyLinearImpulse(Entity.ID, ref impulse, ref worldPosition, wake);
		}

		public void ApplyLinearImpulse(Vector2 impulse, bool wake)
		{
			InternalCalls.Rigidbody2DComponent_ApplyLinearImpulseToCenter(Entity.ID, ref impulse, wake);
		}
	}
}
