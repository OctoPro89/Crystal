#pragma once

#include "Crystal/Scene/SceneCamera.h"
#include "Crystal/Renderer/Texture.h"
#include "Crystal/Renderer/SubTexture2D.h"
#include "Crystal/Audio/SoundSystem.h"
#include "Crystal/Core/UUID.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/quaternion.hpp>


namespace Crystal
{
	struct IDComponent
	{
		UUID ID;

		IDComponent() = default;
		IDComponent(const IDComponent&) = default;
	};

	struct TagComponent
	{
		std::string Tag;

		TagComponent() = default;
		TagComponent(const TagComponent&) = default;
		TagComponent(const std::string& tag)
			: Tag(tag) {}
	};
	struct TransformComponent
	{
		glm::vec3 Translation = { 0.0f, 0.0f, 0.0f };
		glm::vec3 Rotation = { 0.0f, 0.0f, 0.0f };
		glm::vec3 Scale = { 1.0f, 1.0f, 1.0f };

		TransformComponent() = default;
		TransformComponent(const TransformComponent&) = default;
		TransformComponent(const glm::vec3& translation)
			: Translation(translation) {}

		glm::mat4 GetTransform() const
		{
			glm::mat4 rotation = glm::toMat4(glm::quat(Rotation));

			return glm::translate(glm::mat4(1.0f), Translation) * rotation * glm::scale(glm::mat4(1.0f), Scale);
		}
	};

	struct SpriteRendererComponent
	{
		glm::vec4 Color = { 1.0f,1.0f,1.0f,1.0f };
		Ref<Texture2D> Texture;
		Ref<SubTexture2D> SubTex = nullptr;
		float TilingFactor = 1.0f;
		bool IsAtlas = false;
		glm::vec2 cellSize{ 0.0f, 0.0f };
		glm::vec2 texCoords{ 0.0f, 0.0f };

		SpriteRendererComponent() = default;
		SpriteRendererComponent(const SpriteRendererComponent&) = default;
		SpriteRendererComponent(const glm::vec4& color)
			: Color(color) {}
	};

	struct CircleRendererComponent
	{
		glm::vec4 Color = { 1.0f,1.0f,1.0f,1.0f };
		float Radius = 0.5f; // 1 meter
		float Thickness = 1.0f;
		float Fade = 0.0f;

		CircleRendererComponent() = default;
		CircleRendererComponent(const CircleRendererComponent&) = default;
	};

	struct CameraComponent
	{
		Crystal::SceneCamera Camera;
		bool Primary = true;
		bool FixedAspectRatio = false;

		CameraComponent() = default;
		CameraComponent(const CameraComponent&) = default;
	};

	struct ScriptComponent
	{
		std::string ClassName;

		ScriptComponent() = default;
		ScriptComponent(const ScriptComponent&) = default;
	};

	// Forward declaration
	class ScriptableEntity;
	
	struct NativeScriptComponent
	{
		ScriptableEntity* Instance = nullptr;

		ScriptableEntity*(*InstantiateScript)();
		void (*DestroyScript)(NativeScriptComponent*);

		template<typename T>
		void Bind()
		{
			InstantiateScript = []() { return static_cast<ScriptableEntity*>(new T()); };
			DestroyScript = [](NativeScriptComponent* nsc) { delete nsc->Instance; nsc->Instance = nullptr; };
		}
	};

	// Physics

	struct Rigidbody2DComponent
	{
		enum class BodyType {
			Static = 0, Dynamic, Kinematic
		};
		BodyType Type = BodyType::Static;
		bool FixedRotation = false;

		// Storage for runtime
		void* RuntimeBody = nullptr;

		Rigidbody2DComponent() = default;
		Rigidbody2DComponent(const Rigidbody2DComponent&) = default;
	};

	struct BoxCollider2DComponent
	{
		glm::vec2 Offset = { 0.0f, 0.0f };
		glm::vec2 Size = { 0.5f, 0.5f };


		// TODO: move to physics material
		float Density = 1.0f;
		float Friction = 0.5f;
		float Restitution = 0.0f;
		float RestitutionThreshold = 0.5f;
		float GravityScale = 1.0f;

		// Storage for runtime
		void* RuntimeBody = nullptr;

		BoxCollider2DComponent() = default;
		BoxCollider2DComponent(const BoxCollider2DComponent&) = default;
	};

	struct CircleCollider2DComponent
	{
		glm::vec2 Offset = { 0.0f, 0.0f };
		float Radius = 0.5f;


		// TODO: move to physics material
		float Density = 1.0f;
		float Friction = 0.5f;
		float Restitution = 0.0f;
		float RestitutionThreshold = 0.5f;
		float GravityScale = 1.0f;

		// Storage for runtime
		void* RuntimeBody = nullptr;

		CircleCollider2DComponent() = default;
		CircleCollider2DComponent(const CircleCollider2DComponent&) = default;
	};

	class Entity;

	struct DistanceJoint2DComponent
	{
		glm::vec2 AnchorOffset1{ 0.0f, 0.0f };
		glm::vec2 AnchorOffset2{ 0.0f, 0.0f };
		float RestLength;
		float MinDistance;
		float MaxDistance;
		float Damping;
		float Stiffness;
		bool ShouldBodiesCollide;
		std::string AttachedStr = "...";
		UUID Attached;

		DistanceJoint2DComponent() = default;
		DistanceJoint2DComponent(const DistanceJoint2DComponent&) = default;
	};

	struct AudioComponent
	{
		std::string audioFilePath = "";
		float volumeMultiplier = 1.0f;
		AudioComponent() = default;
		AudioComponent(const AudioComponent&) = default;
		void Play()
		{
			SetVolume();
			SoundSystem::PlayAudio(audioFilePath.c_str());
		}
		void SetVolume()
		{
			SoundSystem::SetVolumeMultiplier(volumeMultiplier);
		}
	};

	template<typename... Component>
	struct ComponentGroup
	{

	};
	using AllComponents = ComponentGroup<TransformComponent, SpriteRendererComponent, CameraComponent,
		Rigidbody2DComponent, BoxCollider2DComponent, CircleCollider2DComponent, CircleRendererComponent,
		NativeScriptComponent, ScriptComponent, AudioComponent, DistanceJoint2DComponent>;
}
