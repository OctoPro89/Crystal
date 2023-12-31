﻿using System;
using System.Runtime.CompilerServices;

namespace Crystal
{
	public static class InternalCalls
	{
		[MethodImplAttribute(MethodImplOptions.InternalCall)]
		internal extern static bool Entity_HasComponent(ulong entityID, Type componentType);

		[MethodImplAttribute(MethodImplOptions.InternalCall)]
		internal extern static ulong Entity_FindEntityByName(string name);

		[MethodImplAttribute(MethodImplOptions.InternalCall)]
		internal extern static object GetScriptInstance(ulong entityID);

		[MethodImplAttribute(MethodImplOptions.InternalCall)]
		internal extern static void TransformComponent_GetTranslation(ulong entityID, out Vector3 translation);

		[MethodImplAttribute(MethodImplOptions.InternalCall)]
		internal extern static void TransformComponent_SetTranslation(ulong entityID, ref Vector3 translation);

		[MethodImplAttribute(MethodImplOptions.InternalCall)]
		internal extern static void TransformComponent_GetRotation(ulong entityID, out Vector3 rotation);

		[MethodImplAttribute(MethodImplOptions.InternalCall)]
		internal extern static void TransformComponent_SetRotation(ulong entityID, ref Vector3 rotation);

		[MethodImplAttribute(MethodImplOptions.InternalCall)]
		internal extern static void TransformComponent_GetScale(ulong entityID, out Vector3 scale);

		[MethodImplAttribute(MethodImplOptions.InternalCall)]
		internal extern static void TransformComponent_SetScale(ulong entityID, ref Vector3 scale);

		[MethodImplAttribute(MethodImplOptions.InternalCall)]
		internal extern static void Rigidbody2DComponent_ApplyLinearImpulse(ulong entityID, ref Vector2 impulse, ref Vector2 point, bool wake);

		[MethodImplAttribute(MethodImplOptions.InternalCall)]
		internal extern static void Rigidbody2DComponent_ApplyTorque(ulong entityID, float Torque, bool wake);

		[MethodImplAttribute(MethodImplOptions.InternalCall)]
		internal extern static void Rigidbody2DComponent_ApplyForce(ulong entityID, ref Vector2 force, ref Vector2 point, bool wake);

		[MethodImplAttribute(MethodImplOptions.InternalCall)]
		internal extern static void Rigidbody2DComponent_ApplyForceToCenter(ulong entityID, ref Vector2 force, bool wake);

		[MethodImplAttribute(MethodImplOptions.InternalCall)]
		internal extern static void Rigidbody2DComponent_ApplyLinearImpulseToCenter(ulong entityID, ref Vector2 impulse, bool wake);

		[MethodImplAttribute(MethodImplOptions.InternalCall)]
		internal extern static void Rigidbody2DComponent_GetPosition(ulong entityID, out Vector2 velocity);

		[MethodImplAttribute(MethodImplOptions.InternalCall)]
		internal extern static void SpriteRendererComponent_GetColor(ulong entityID, out Vector4 color);

		[MethodImplAttribute(MethodImplOptions.InternalCall)]
		internal extern static void SpriteRendererComponent_SetColor(ulong entityID, ref Vector4 color);

		[MethodImplAttribute(MethodImplOptions.InternalCall)]
		internal extern static void SpriteRendererComponent_GetTexIndex(ulong entityID, out Vector2 coords);

		[MethodImplAttribute(MethodImplOptions.InternalCall)]
		internal extern static void SpriteRendererComponent_SetTexIndex(ulong entityID, ref Vector2 index);

		[MethodImplAttribute(MethodImplOptions.InternalCall)]
		internal extern static void BoxCollider2DComponent_SetDensity(ulong entityID, float density);

		[MethodImplAttribute(MethodImplOptions.InternalCall)]
		internal extern static void BoxCollider2DComponent_GetDensity(ulong entityID, out float density);

		[MethodImplAttribute(MethodImplOptions.InternalCall)]
		internal extern static void BoxCollider2DComponent_SetFriction(ulong entityID, float friction);

		[MethodImplAttribute(MethodImplOptions.InternalCall)]
		internal extern static void BoxCollider2DComponent_GetFriction(ulong entityID, out float friction);

		[MethodImplAttribute(MethodImplOptions.InternalCall)]
		internal extern static void BoxCollider2DComponent_SetGravityScale(ulong entityID, float gravityScale);

		[MethodImplAttribute(MethodImplOptions.InternalCall)]
		internal extern static void BoxCollider2DComponent_GetGravityScale(ulong entityID, out float gravityScale);

		[MethodImplAttribute(MethodImplOptions.InternalCall)]
		internal extern static void BoxCollider2DComponent_SetOffset(ulong entityID, ref Vector2 Offset);

		[MethodImplAttribute(MethodImplOptions.InternalCall)]
		internal extern static void BoxCollider2DComponent_GetOffset(ulong entityID, out Vector2 Offset);

		[MethodImplAttribute(MethodImplOptions.InternalCall)]
		internal extern static void BoxCollider2DComponent_SetSize(ulong entityID, ref Vector2 Size);

		[MethodImplAttribute(MethodImplOptions.InternalCall)]
		internal extern static void BoxCollider2DComponent_GetSize(ulong entityID, out Vector2 Size);

		[MethodImplAttribute(MethodImplOptions.InternalCall)]
		internal extern static void BoxCollider2DComponent_SetRestitution(ulong entityID, float restitution);

		[MethodImplAttribute(MethodImplOptions.InternalCall)]
		internal extern static void BoxCollider2DComponent_GetRestitution(ulong entityID, out float restitution);

		[MethodImplAttribute(MethodImplOptions.InternalCall)]
		internal extern static void BoxCollider2DComponent_SetRestitutionThreshold(ulong entityID, float restitutionThreshold);

		[MethodImplAttribute(MethodImplOptions.InternalCall)]
		internal extern static void BoxCollider2DComponent_GetRestitutionThreshold(ulong entityID, out float restitutionThreshold);

		[MethodImplAttribute(MethodImplOptions.InternalCall)]
		internal extern static bool Input_IsKeyDown(KeyCode keycode);

		[MethodImplAttribute(MethodImplOptions.InternalCall)]
		internal extern static bool Input_IsMouseDown(MouseCode button);

		[MethodImplAttribute(MethodImplOptions.InternalCall)]
		internal extern static void Input_GetMousePos(out Vector2 mousePos);

		[MethodImplAttribute(MethodImplOptions.InternalCall)]
		internal extern static bool Input_GetMouseX(out float x);

		[MethodImplAttribute(MethodImplOptions.InternalCall)]
		internal extern static bool Input_GetMouseY(out float y);

		[MethodImplAttribute(MethodImplOptions.InternalCall)]
		internal extern static void AudioComponent_PlayAudio(ulong entityID);

		[MethodImplAttribute(MethodImplOptions.InternalCall)]
		internal extern static void AudioComponent_SetVolumeMultiplier(ulong entityID, float volumeMultiplier);

		[MethodImplAttribute(MethodImplOptions.InternalCall)]
		internal extern static void Editor_ConsoleLog(string message);

		[MethodImplAttribute(MethodImplOptions.InternalCall)]
		internal extern static void Editor_ConsoleWarn(string message);

		[MethodImplAttribute(MethodImplOptions.InternalCall)]
		internal extern static void Editor_ConsoleError(string message);

		[MethodImplAttribute(MethodImplOptions.InternalCall)]
		internal extern static void Scene_CreateEntity(out ulong UUID);

		[MethodImplAttribute(MethodImplOptions.InternalCall)]
		internal extern static void Scene_DestroyEntity(ulong entityID);
	}
}
