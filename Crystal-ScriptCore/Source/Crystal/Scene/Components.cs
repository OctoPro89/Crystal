using System;
using System.Collections.Generic;
using System.Linq;
using System.Runtime.CompilerServices;
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
		public float Density
		{
			get
			{
				InternalCalls.BoxCollider2DComponent_GetDensity(Entity.ID, out float density);
				return density;
			}
			set
			{
				InternalCalls.BoxCollider2DComponent_SetDensity(Entity.ID, value);
			}
		}
		public float Friction
		{
			get
			{
				InternalCalls.BoxCollider2DComponent_GetFriction(Entity.ID, out float friction);
				return friction;
			}
			set
			{
				InternalCalls.BoxCollider2DComponent_SetFriction(Entity.ID, value);
			}
		}

		public float GravityScale
		{
			get
			{
				InternalCalls.BoxCollider2DComponent_GetGravityScale(Entity.ID, out float gravityScale);
				return gravityScale;
			}
			set
			{
				InternalCalls.BoxCollider2DComponent_SetGravityScale(Entity.ID, value);
			}
		}

		public float Restitution
		{
			get
			{
				InternalCalls.BoxCollider2DComponent_GetRestitution(Entity.ID, out float restitution);
				return restitution;
			}
			set
			{
				InternalCalls.BoxCollider2DComponent_SetRestitution(Entity.ID, value);
			}
		}

		public float RestitutionThreshold
		{
			get
			{
				InternalCalls.BoxCollider2DComponent_GetRestitutionThreshold(Entity.ID, out float restitutionThreshold);
				return restitutionThreshold;
			}
			set
			{
				InternalCalls.BoxCollider2DComponent_SetRestitutionThreshold(Entity.ID, value);
			}
		}

		public Vector2 Offset
		{
			get
			{
				InternalCalls.BoxCollider2DComponent_GetOffset(Entity.ID, out Vector2 offset);
				return offset;
			}
			set
			{
				InternalCalls.BoxCollider2DComponent_SetOffset(Entity.ID, ref value);
			}
		}

		public Vector2 Size
		{
			get
			{
				InternalCalls.BoxCollider2DComponent_GetSize(Entity.ID, out Vector2 size);
				return size;
			}
			set
			{
				InternalCalls.BoxCollider2DComponent_SetSize(Entity.ID, ref value);
			}
		}
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

		public Vector3 Rotation
		{
			get
			{
				InternalCalls.TransformComponent_GetRotation(Entity.ID, out Vector3 rotation);
				return rotation;
			}
			set
			{
				InternalCalls.TransformComponent_SetRotation(Entity.ID, ref value);
			}
		}

		public Vector3 Scale
		{
			get
			{
				InternalCalls.TransformComponent_GetScale(Entity.ID, out Vector3 scale);
				return scale;
			}
			set
			{
				InternalCalls.TransformComponent_SetScale(Entity.ID, ref value);
			}
		}
	}

	public class Rigidbody2DComponent : Component
	{
		public enum BodyType
		{
			Static = 0,
			Dynamic = 1,
			Kinematic = 2
		};

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

		public void ApplyForce(Vector2 force, bool wake)
		{
			InternalCalls.Rigidbody2DComponent_ApplyForceToCenter(Entity.ID, ref force, wake);
		}

		public Rigidbody2DComponent.BodyType Type
		{
			get
			{
				return InternalCalls.Rigidbody2DComponent_GetType(Entity.ID);
			}
			set
			{
				InternalCalls.Rigidbody2DComponent_SetType(Entity.ID, value);
			}
		}

		public Vector2 LinearVelocity
		{
			get 
			{
				InternalCalls.Rigidbody2DComponent_GetLinearVelocity(Entity.ID, out Vector2 Velocity);
				return Velocity;
			}
		}

		public float AngularVelocity
		{
			get
			{
				InternalCalls.Rigidbody2DComponent_GetAngularVelocity(Entity.ID, out float Velocity);
				return Velocity;
			}
		}
	}

	public class AudioComponent : Component
	{
		public void Play()
		{
			InternalCalls.AudioComponent_PlayAudio(Entity.ID);
		}

		public void SetVolume(float volumeMultiplier)
		{
			InternalCalls.AudioComponent_SetVolumeMultiplier(Entity.ID, volumeMultiplier);
		}
	}
}
