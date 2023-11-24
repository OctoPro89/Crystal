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

		public Vector2 AtlasCoords
		{
			get
			{
				InternalCalls.SpriteRendererComponent_GetTexIndex(Entity.ID, out Vector2 coords);
				return coords;
			}
			set
			{
				InternalCalls.SpriteRendererComponent_SetTexIndex(Entity.ID, ref value);
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

		public void ApplyTorque(float Torque, bool wake)
		{
			InternalCalls.Rigidbody2DComponent_ApplyTorque(Entity.ID, Torque, wake);
		}

		public void ApplyForce(Vector2 force, Vector2 point, bool wake)
		{
			InternalCalls.Rigidbody2DComponent_ApplyForce(Entity.ID, ref force, ref point, wake);
		}

		public void ApplyForceToCenter(Vector2 force, bool wake)
		{
			InternalCalls.Rigidbody2DComponent_ApplyForceToCenter(Entity.ID, ref force, wake);
		}
	}
}
