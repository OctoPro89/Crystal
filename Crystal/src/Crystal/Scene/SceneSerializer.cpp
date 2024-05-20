#include "crystalpch.h"
#include "SceneSerializer.h"

#include "Entity.h"
#include "Components.h"
#include <Crystal/Scripting/ScriptEngine.h>
#include <Crystal/Projects/Project.h>

// #define CRYSTAL_NO_EDITOR
#include <Helpers/EditorHelper.h>

#include <fstream>

#include <yaml-cpp/yaml.h>

namespace YAML {

	template<>
	struct convert<glm::vec2>
	{
		static Node encode(const glm::vec2& rhs)
		{
			Node node;
			node.push_back(rhs.x);
			node.push_back(rhs.y);
			node.SetStyle(EmitterStyle::Flow);
			return node;
		}

		static bool decode(const Node& node, glm::vec2& rhs)
		{
			if (!node.IsSequence() || node.size() != 2)
				return false;

			rhs.x = node[0].as<float>();
			rhs.y = node[1].as<float>();
			return true;
		}
	};

	template<>
	struct convert<glm::vec3>
	{
		static Node encode(const glm::vec3& rhs)
		{
			Node node;
			node.push_back(rhs.x);
			node.push_back(rhs.y);
			node.push_back(rhs.z);
			node.SetStyle(EmitterStyle::Flow);
			return node;
		}

		static bool decode(const Node& node, glm::vec3& rhs)
		{
			if (!node.IsSequence() || node.size() != 3)
				return false;

			rhs.x = node[0].as<float>();
			rhs.y = node[1].as<float>();
			rhs.z = node[2].as<float>();
			return true;
		}
	};

	template<>
	struct convert<glm::vec4>
	{
		static Node encode(const glm::vec4& rhs)
		{
			Node node;
			node.push_back(rhs.x);
			node.push_back(rhs.y);
			node.push_back(rhs.z);
			node.push_back(rhs.w);
			node.SetStyle(EmitterStyle::Flow);
			return node;
		}

		static bool decode(const Node& node, glm::vec4& rhs)
		{
			if (!node.IsSequence() || node.size() != 4)
				return false;

			rhs.x = node[0].as<float>();
			rhs.y = node[1].as<float>();
			rhs.z = node[2].as<float>();
			rhs.w = node[3].as<float>();
			return true;
		}
	};
	template<>
	struct convert<Crystal::UUID>
	{
		static Node encode(const Crystal::UUID& uuid)
		{
			Node node;
			node.push_back((uint64_t)uuid);
			return node;
		}

		static bool decode(const Node& node, Crystal::UUID& uuid)
		{
			uuid = node.as<int64_t>();
			return true;
		}
	};
}
namespace Crystal {
#define WRITE_SCRIPT_FIELD(FieldType, Type)           \
			case ScriptFieldType::FieldType:          \
				out << scriptField.GetValue<Type>();  \
				break

#define READ_SCRIPT_FIELD(FieldType, Type)             \
	case ScriptFieldType::FieldType:                   \
	{                                                  \
		Type data = scriptField["Data"].as<Type>();    \
		fieldInstance.SetValue(data);                  \
		break;                                         \
	}
	YAML::Emitter& operator<<(YAML::Emitter& out, const glm::vec3& v)
	{
		out << YAML::Flow;
		out << YAML::BeginSeq << v.x << v.y << v.z << YAML::EndSeq;
		return out;
	}

	YAML::Emitter& operator<<(YAML::Emitter& out, const glm::vec2& v)
	{
		out << YAML::Flow;
		out << YAML::BeginSeq << v.x << v.y << YAML::EndSeq;
		return out;
	}

	YAML::Emitter& operator<<(YAML::Emitter& out, const glm::vec4& v)
	{
		out << YAML::Flow;
		out << YAML::BeginSeq << v.x << v.y << v.z << v.w << YAML::EndSeq;
		return out;
	}

	SceneSerializer::SceneSerializer(const Ref<Scene>& scene)
		: m_Scene(scene)
	{
	}

	static std::string Rigidbody2DBodyTypeToString(Rigidbody2DComponent::BodyType bodyType)
	{
		switch (bodyType)
		{
		case Rigidbody2DComponent::BodyType::Static: return "Static";
		case Rigidbody2DComponent::BodyType::Dynamic: return "Dynamic";
		case Rigidbody2DComponent::BodyType::Kinematic: return "Kinematic";
		}
		CRYSTAL_CORE_ASSERT(false, "Rigidbody2DBodyTypeToString failed!");
		return NULL;
	}

	static Rigidbody2DComponent::BodyType RigidBody2DBodyTypeFromString(const std::string& bodyTypeString)
	{
		if (bodyTypeString == "Static") return Rigidbody2DComponent::BodyType::Static; 
		if (bodyTypeString == "Dynamic") return Rigidbody2DComponent::BodyType::Dynamic;
		if (bodyTypeString == "Kinematic") return Rigidbody2DComponent::BodyType::Kinematic;
		CRYSTAL_CORE_ASSERT(false, "RigidBody2DBodyTypeFromString failed!");
		return Rigidbody2DComponent::BodyType::Static;
	}

	static void SerializeEntity(YAML::Emitter& out, Entity entity)
	{
		CRYSTAL_CORE_ASSERT(entity.HasComponent<IDComponent>(), "Entity has no ID / UUID!");
		out << YAML::BeginMap; // Entity
		out << YAML::Key << "Entity" << YAML::Value << entity.GetUUID();

		if (entity.HasComponent<TagComponent>())
		{
			out << YAML::Key << "TagComponent";
			out << YAML::BeginMap; // TagComponent

			auto& tag = entity.GetComponent<TagComponent>().Tag;
			out << YAML::Key << "Tag" << YAML::Value << tag;

			out << YAML::EndMap; // TagComponent
		}

		if (entity.HasComponent<TransformComponent>())
		{
			out << YAML::Key << "TransformComponent";
			out << YAML::BeginMap; // TransformComponent

			auto& tc = entity.GetComponent<TransformComponent>();
			out << YAML::Key << "Translation" << YAML::Value << tc.Translation;
			out << YAML::Key << "Rotation" << YAML::Value << tc.Rotation;
			out << YAML::Key << "Scale" << YAML::Value << tc.Scale;

			out << YAML::EndMap; // TransformComponent
		}

		if (entity.HasComponent<Rigidbody2DComponent>())
		{
			out << YAML::Key << "Rigidbody2DComponent";
			out << YAML::BeginMap; // Rigidbody2DComponent

			auto& rbc = entity.GetComponent<Rigidbody2DComponent>();
			out << YAML::Key << "BodyType" << YAML::Value << Rigidbody2DBodyTypeToString(rbc.Type);
			out << YAML::Key << "FixedRotation" << YAML::Value << rbc.FixedRotation;

			out << YAML::EndMap; // Rigidbody2DComponent
		}

		if (entity.HasComponent<BoxCollider2DComponent>())
		{
			out << YAML::Key << "BoxCollider2DComponent";
			out << YAML::BeginMap; // BoxCollider2DComponent

			auto& b2dc = entity.GetComponent<BoxCollider2DComponent>();
			out << YAML::Key << "Offset" << YAML::Value << b2dc.Offset;
			out << YAML::Key << "Size" << YAML::Value << b2dc.Size;
			out << YAML::Key << "Density" << YAML::Value << b2dc.Density;
			out << YAML::Key << "GravityScale" << YAML::Value << b2dc.GravityScale;
			out << YAML::Key << "Friction" << YAML::Value << b2dc.Friction;
			out << YAML::Key << "Restitution" << YAML::Value << b2dc.Restitution;
			out << YAML::Key << "RestitutionThreshold" << YAML::Value << b2dc.RestitutionThreshold;

			out << YAML::EndMap; // BoxCollider2DComponent
		}

		if (entity.HasComponent<CircleCollider2DComponent>())
		{
			out << YAML::Key << "CircleCollider2DComponent";
			out << YAML::BeginMap; // CircleCollider2DComponent

			auto& cc2d = entity.GetComponent<CircleCollider2DComponent>();
			out << YAML::Key << "Offset" << YAML::Value << cc2d.Offset;
			out << YAML::Key << "Radius" << YAML::Value << cc2d.Radius;
			out << YAML::Key << "Density" << YAML::Value << cc2d.Density;
			out << YAML::Key << "GravityScale" << YAML::Value << cc2d.GravityScale;
			out << YAML::Key << "Friction" << YAML::Value << cc2d.Friction;
			out << YAML::Key << "Restitution" << YAML::Value << cc2d.Restitution;
			out << YAML::Key << "RestitutionThreshold" << YAML::Value << cc2d.RestitutionThreshold;

			out << YAML::EndMap; // CircleCollider2DComponent
		}

		if (entity.HasComponent<CircleRendererComponent>())
		{
			out << YAML::Key << "CircleRendererComponent";
			out << YAML::BeginMap; // CircleRendererComponent

			auto& circleRendererComponent = entity.GetComponent<CircleRendererComponent>();
			out << YAML::Key << "Color" << YAML::Value << circleRendererComponent.Color;
			out << YAML::Key << "Thickness" << YAML::Value << circleRendererComponent.Thickness;
			out << YAML::Key << "Fade" << YAML::Value << circleRendererComponent.Fade;

			out << YAML::EndMap; // CircleRendererComponent
		}

		if (entity.HasComponent<CameraComponent>())
		{
			out << YAML::Key << "CameraComponent";
			out << YAML::BeginMap; // CameraComponent

			auto& cameraComponent = entity.GetComponent<CameraComponent>();
			auto& camera = cameraComponent.Camera;

			out << YAML::Key << "Camera" << YAML::Value;
			out << YAML::BeginMap; // Camera
			out << YAML::Key << "ProjectionType" << YAML::Value << (int)camera.GetProjectionType();
			out << YAML::Key << "PerspectiveFOV" << YAML::Value << camera.GetPerspectiveVerticalFOV();
			out << YAML::Key << "PerspectiveNear" << YAML::Value << camera.GetPerspectiveNearClip();
			out << YAML::Key << "PerspectiveFar" << YAML::Value << camera.GetPerspectiveFarClip();
			out << YAML::Key << "OrthographicSize" << YAML::Value << camera.GetOrthographicSize();
			out << YAML::Key << "OrthographicNear" << YAML::Value << camera.GetOrthographicNearClip();
			out << YAML::Key << "OrthographicFar" << YAML::Value << camera.GetOrthographicFarClip();
			out << YAML::EndMap; // Camera

			out << YAML::Key << "Primary" << YAML::Value << cameraComponent.Primary;
			out << YAML::Key << "FixedAspectRatio" << YAML::Value << cameraComponent.FixedAspectRatio;

			out << YAML::EndMap; // CameraComponent
		}

		if (entity.HasComponent<SpriteRendererComponent>())
		{
			out << YAML::Key << "SpriteRendererComponent";
			out << YAML::BeginMap; // SpriteRendererComponent

			auto& spriteRendererComponent = entity.GetComponent<SpriteRendererComponent>();
			out << YAML::Key << "Color" << YAML::Value << spriteRendererComponent.Color;
			if (spriteRendererComponent.Texture)
				out << YAML::Key << "TexturePath" << YAML::Value << spriteRendererComponent.Texture->GetPath();

			out << YAML::Key << "TilingFactor" << YAML::Value << spriteRendererComponent.TilingFactor;

			if (spriteRendererComponent.SubTex) {
				if (spriteRendererComponent.IsAtlas)
				{
					out << YAML::Key << "AtlasX" << YAML::Value << spriteRendererComponent.texCoords.x;

					out << YAML::Key << "AtlasY" << YAML::Value << spriteRendererComponent.texCoords.y;

					out << YAML::Key << "CellX" << YAML::Value << spriteRendererComponent.cellSize.x;

					out << YAML::Key << "CellY" << YAML::Value << spriteRendererComponent.cellSize.y;

					out << YAML::Key << "IsAtlas" << YAML::Value << spriteRendererComponent.IsAtlas;
				}
			}

			out << YAML::EndMap; // SpriteRendererComponent
		}

		if (entity.HasComponent<LineRendererComponent>())
		{
			out << YAML::Key << "LineRendererComponent";
			out << YAML::BeginMap; // LineRendererComponent

			auto& lineRendererComponent = entity.GetComponent<LineRendererComponent>();
			out << YAML::Key << "Color" << lineRendererComponent.Color;
			out << YAML::Key << "Pos1" << lineRendererComponent.Pos1;
			out << YAML::Key << "Pos2" << lineRendererComponent.Pos2;

			out << YAML::EndMap; // LineRendererComponent
		}

		if (entity.HasComponent<ScriptComponent>())
		{
			auto& scriptComponent = entity.GetComponent<ScriptComponent>();

			out << YAML::Key << "ScriptComponent";
			out << YAML::BeginMap; // ScriptComponent
			out << YAML::Key << "ClassName" << YAML::Value << scriptComponent.ClassName;

			// Fields
			Ref<ScriptClass> entityClass = ScriptEngine::GetEntityClass(scriptComponent.ClassName);
			const auto& fields = entityClass->GetFields();
			if (fields.size() > 0)
			{
				out << YAML::Key << "ScriptFields" << YAML::Value;
				auto& entityFields = ScriptEngine::GetScriptFieldMap(entity);
				out << YAML::BeginSeq;
				for (const auto& [name, field] : fields)
				{
					if (entityFields.find(name) == entityFields.end())
						continue;

					out << YAML::BeginMap; // ScriptField
					out << YAML::Key << "Name" << YAML::Value << name;
					out << YAML::Key << "Type" << YAML::Value << Utils::ScriptFieldTypeToString(field.Type);

					out << YAML::Key << "Data" << YAML::Value;
					ScriptFieldInstance& scriptField = entityFields.at(name);

					switch (field.Type)
					{
						WRITE_SCRIPT_FIELD(Float, float);
						WRITE_SCRIPT_FIELD(Double, double);
						WRITE_SCRIPT_FIELD(Bool, bool);
						WRITE_SCRIPT_FIELD(Char, char);
						WRITE_SCRIPT_FIELD(Byte, int8_t);
						WRITE_SCRIPT_FIELD(Short, int16_t);
						WRITE_SCRIPT_FIELD(Int, int32_t);
						WRITE_SCRIPT_FIELD(Long, int64_t);
						WRITE_SCRIPT_FIELD(SByte, uint8_t);
						WRITE_SCRIPT_FIELD(UShort, uint16_t);
						WRITE_SCRIPT_FIELD(UInt, uint32_t);
						WRITE_SCRIPT_FIELD(ULong, uint64_t);
						WRITE_SCRIPT_FIELD(Vector2, glm::vec2);
						WRITE_SCRIPT_FIELD(Vector3, glm::vec3);
						WRITE_SCRIPT_FIELD(Vector4, glm::vec4);
						WRITE_SCRIPT_FIELD(Entity, UUID);
					}
					out << YAML::EndMap; // ScriptFields
				}
				out << YAML::EndSeq;
			}
			out << YAML::EndMap; // ScriptComponent
		}

		if (entity.HasComponent<AudioComponent>())
		{
			out << YAML::Key << "AudioComponent";
			out << YAML::BeginMap; // AudioComponent
			auto& audioComponent = entity.GetComponent<AudioComponent>();
			
			out << YAML::Key << "AudioFile" << YAML::Value << audioComponent.audioFilePath;

			out << YAML::Key << "VolumeMultiplier" << YAML::Value << audioComponent.volumeMultiplier;

			out << YAML::EndMap; // AudioComponent
		}

		if (entity.HasComponent<DistanceJoint2DComponent>())
		{
			out << YAML::Key << "DistanceJoint2DComponent";
			out << YAML::BeginMap; // DistanceJoint2DComponent
			auto& distanceJointComponent = entity.GetComponent<DistanceJoint2DComponent>();

			out << YAML::Key << "AnchorOffset1" << YAML::Value << distanceJointComponent.AnchorOffset1;
			out << YAML::Key << "AnchorOffset2" << YAML::Value << distanceJointComponent.AnchorOffset2;

			out << YAML::Key << "Attached" << YAML::Value << distanceJointComponent.Attached;
			out << YAML::Key << "AttachedStr" << YAML::Value << distanceJointComponent.AttachedStr;

			out << YAML::Key << "Damping" << YAML::Value << distanceJointComponent.Damping;
			out << YAML::Key << "MaxDistance" << YAML::Value << distanceJointComponent.MaxDistance;
			out << YAML::Key << "MinDistance" << YAML::Value << distanceJointComponent.MinDistance;
			out << YAML::Key << "RestLength" << YAML::Value << distanceJointComponent.RestLength;
			out << YAML::Key << "ShouldBodiesCollide" << YAML::Value << distanceJointComponent.ShouldBodiesCollide;
			out << YAML::Key << "Stiffness" << YAML::Value << distanceJointComponent.Stiffness;

			out << YAML::EndMap; // DistanceJoint2DComponent
		}

		if (entity.HasComponent<HingeJoint2DComponent>())
		{
			out << YAML::Key << "HingeJoint2DComponent";
			out << YAML::BeginMap; // HingeJoint2DComponent
			auto& hingeJointComponent = entity.GetComponent<HingeJoint2DComponent>();

			out << YAML::Key << "AnchorOffset1" << YAML::Value << hingeJointComponent.AnchorOffset1;
			out << YAML::Key << "AnchorOffset2" << YAML::Value << hingeJointComponent.AnchorOffset2;

			out << YAML::Key << "Attached" << YAML::Value << hingeJointComponent.Attached;
			out << YAML::Key << "AttachedStr" << YAML::Value << hingeJointComponent.AttachedStr;

			out << YAML::Key << "ShouldBodiesCollide" << YAML::Value << hingeJointComponent.ShouldBodiesCollide;
			out << YAML::Key << "EnableLimit" << YAML::Value << hingeJointComponent.EnableLimit;
			out << YAML::Key << "EnableMotor" << YAML::Value << hingeJointComponent.EnableMotor;

			out << YAML::Key << "LowerLimitAngle" << YAML::Value << hingeJointComponent.LowerLimitAngle;
			out << YAML::Key << "UpperLimitAngle" << YAML::Value << hingeJointComponent.UpperLimitAngle;

			out << YAML::Key << "MotorSpeed" << YAML::Value << hingeJointComponent.MotorSpeed;
			out << YAML::Key << "MaxMotorTorque" << YAML::Value << hingeJointComponent.MaxMotorTorque;

			out << YAML::EndMap; // HingeJoint2DComponent
		}

		if (entity.HasComponent<TextRendererComponent>())
		{
			out << YAML::Key << "TextRendererComponent";
			out << YAML::BeginMap; // TextRendererComponent
			auto& textRendererComponent = entity.GetComponent<TextRendererComponent>();

			out << YAML::Key << "TextString" << YAML::Value << textRendererComponent.TextString;
			out << YAML::Key << "Color" << YAML::Value << textRendererComponent.Color;
			out << YAML::Key << "Kerning" << YAML::Value << textRendererComponent.Kerning;
			out << YAML::Key << "LineSpacing" << YAML::Value << textRendererComponent.LineSpacing;

			out << YAML::EndMap; // TextRendererComponent
		}

		out << YAML::EndMap; // Entity
	}

	void SceneSerializer::Serialize(const std::string& filepath)
	{
		YAML::Emitter out;
		out << YAML::BeginMap;
		out << YAML::Key << "Scene" << YAML::Value << "Untitled";
		out << YAML::Key << "Entities" << YAML::Value << YAML::BeginSeq;
		m_Scene->m_Registry.each([&](auto entityID)
			{
				Entity entity = { entityID, m_Scene.get() };
				if (!entity)
					return;

				SerializeEntity(out, entity);
			});
		out << YAML::EndSeq;
		out << YAML::EndMap;

		std::ofstream fout(filepath);
		fout << out.c_str();
	}

	void SceneSerializer::SerializeRuntime(const std::string& filepath)
	{
		// Not implemented
		CRYSTAL_CORE_ASSERT(false, "broke");
	}

	bool SceneSerializer::Deserialize(const std::string& filepath)
	{
		YAML::Node data = YAML::LoadFile(filepath);
		if (!data["Scene"])
			return false;

		std::string sceneName = data["Scene"].as<std::string>();
		CRYSTAL_CORE_TRACE("Deserializing scene '{0}'", sceneName);

		auto entities = data["Entities"];
		if (entities)
		{
			for (auto entity : entities)
			{
				uint64_t uuid = entity["Entity"].as<uint64_t>();

				std::string name;
				auto tagComponent = entity["TagComponent"];
				if (tagComponent)
					name = tagComponent["Tag"].as<std::string>();

				CRYSTAL_CORE_TRACE("Deserialized entity with ID = {0}, name = {1}", uuid, name);

				Entity deserializedEntity = m_Scene->CreateEntityWithUUID(uuid, name);

				auto transformComponent = entity["TransformComponent"];
				if (transformComponent)
				{
					// Entities always have transforms
					auto& tc = deserializedEntity.GetComponent<TransformComponent>();
					tc.Translation = transformComponent["Translation"].as<glm::vec3>();
					tc.Rotation = transformComponent["Rotation"].as<glm::vec3>();
					tc.Scale = transformComponent["Scale"].as<glm::vec3>();
				}

				auto cameraComponent = entity["CameraComponent"];
				if (cameraComponent)
				{
					auto& cc = deserializedEntity.AddComponent<CameraComponent>();

					auto& cameraProps = cameraComponent["Camera"];
					cc.Camera.SetProjectionType((SceneCamera::ProjectionType)cameraProps["ProjectionType"].as<int>());

					cc.Camera.SetPerspectiveVerticalFOV(cameraProps["PerspectiveFOV"].as<float>());
					cc.Camera.SetPerspectiveNearClip(cameraProps["PerspectiveNear"].as<float>());
					cc.Camera.SetPerspectiveFarClip(cameraProps["PerspectiveFar"].as<float>());

					cc.Camera.SetOrthographicSize(cameraProps["OrthographicSize"].as<float>());
					cc.Camera.SetOrthographicNearClip(cameraProps["OrthographicNear"].as<float>());
					cc.Camera.SetOrthographicFarClip(cameraProps["OrthographicFar"].as<float>());

					cc.Primary = cameraComponent["Primary"].as<bool>();
					cc.FixedAspectRatio = cameraComponent["FixedAspectRatio"].as<bool>();
				}

				auto spriteRendererComponent = entity["SpriteRendererComponent"];
				if (spriteRendererComponent)
				{
					auto& src = deserializedEntity.AddComponent<SpriteRendererComponent>();
					src.Color = spriteRendererComponent["Color"].as<glm::vec4>();
					if (spriteRendererComponent["TexturePath"])
					{
						std::string texturePath = spriteRendererComponent["TexturePath"].as<std::string>();
						src.Texture = Texture2D::Create(Project::GetAssetFileSystemPath(texturePath).string());
					}
					if (spriteRendererComponent["IsAtlas"])
					{
						src.SubTex = SubTexture2D::CreateFromCoords(src.Texture, {
						spriteRendererComponent["AtlasX"].as<float>(),
						spriteRendererComponent["AtlasY"].as<float>() },
						{
						spriteRendererComponent["CellX"].as<float>(),
						spriteRendererComponent["CellY"].as<float>()
						});
						src.cellSize.x = spriteRendererComponent["CellX"].as<float>();
						src.cellSize.y = spriteRendererComponent["CellY"].as<float>();
						src.texCoords.x = spriteRendererComponent["AtlasX"].as<float>();
						src.texCoords.y = spriteRendererComponent["AtlasY"].as<float>();
						src.IsAtlas = true;
					}
					if(spriteRendererComponent["TilingFactor"])
						src.TilingFactor = spriteRendererComponent["TilingFactor"].as<float>();
				}

				auto lineRendererComponent = entity["LineRendererComponent"];
				if (lineRendererComponent)
				{
					auto& lrc = deserializedEntity.AddComponent<LineRendererComponent>();

					lrc.Color = lineRendererComponent["Color"].as<glm::vec4>();
					lrc.Pos1 = lineRendererComponent["Pos1"].as<glm::vec3>();
					lrc.Pos2 = lineRendererComponent["Pos2"].as<glm::vec3>();
				}

				auto circleRendererComponent = entity["CircleRendererComponent"];
				if (circleRendererComponent)
				{
					auto& crc = deserializedEntity.AddComponent<CircleRendererComponent>();
					crc.Color = circleRendererComponent["Color"].as<glm::vec4>();
					crc.Thickness = circleRendererComponent["Thickness"].as<float>();
					crc.Fade = circleRendererComponent["Fade"].as<float>();
				}

				auto rigidbody2DComponent = entity["Rigidbody2DComponent"];
				if (rigidbody2DComponent)
				{
					auto& rb2d = deserializedEntity.AddComponent<Rigidbody2DComponent>();
					rb2d.Type = RigidBody2DBodyTypeFromString(rigidbody2DComponent["BodyType"].as<std::string>());
					rb2d.FixedRotation = rigidbody2DComponent["FixedRotation"].as<bool>();
				}

				auto boxCollider2DComponent = entity["BoxCollider2DComponent"];
				if (boxCollider2DComponent)
				{
					auto& bc2d = deserializedEntity.AddComponent<BoxCollider2DComponent>();
					bc2d.Offset = boxCollider2DComponent["Offset"].as<glm::vec2>();
					bc2d.Size = boxCollider2DComponent["Size"].as<glm::vec2>();
					bc2d.Density = boxCollider2DComponent["Density"].as<float>();
					bc2d.GravityScale = boxCollider2DComponent["GravityScale"].as<float>();
					bc2d.Friction = boxCollider2DComponent["Friction"].as<float>();
					bc2d.Restitution = boxCollider2DComponent["Restitution"].as<float>();
					bc2d.RestitutionThreshold = boxCollider2DComponent["RestitutionThreshold"].as<float>();
				}

				auto circleCollider2DComponent = entity["CircleCollider2DComponent"];
				if (circleCollider2DComponent)
				{
					auto& cc2d = deserializedEntity.AddComponent<CircleCollider2DComponent>();
					cc2d.Offset = circleCollider2DComponent["Offset"].as<glm::vec2>();
					cc2d.Radius = circleCollider2DComponent["Radius"].as<float>();
					cc2d.Density = circleCollider2DComponent["Density"].as<float>();
					cc2d.GravityScale = circleCollider2DComponent["GravityScale"].as<float>();
					cc2d.Friction = circleCollider2DComponent["Friction"].as<float>();
					cc2d.Restitution = circleCollider2DComponent["Restitution"].as<float>();
					cc2d.RestitutionThreshold = circleCollider2DComponent["RestitutionThreshold"].as<float>();
				}

				auto scriptComponent = entity["ScriptComponent"];
				if (scriptComponent)
				{
					auto& sc = deserializedEntity.AddComponent<ScriptComponent>();
					sc.ClassName = scriptComponent["ClassName"].as<std::string>();

					auto scriptFields = scriptComponent["ScriptFields"];
					if (scriptFields)
					{
						Ref<ScriptClass> entityClass = ScriptEngine::GetEntityClass(sc.ClassName);
						if (!entityClass)
						{
							CRYSTAL_CORE_WARN("No Entity Class Found For Script Fields (Probably Invalid Class Name) On Entity: {}" + deserializedEntity.GetName());
							EDITOR_WARN("No Entity Class Found For Script Fields (Probably Invalid Class Name) On Entity: " + deserializedEntity.GetName());
						}
						else 
						{
							const auto& fields = entityClass->GetFields();
							auto& entityFields = ScriptEngine::GetScriptFieldMap(deserializedEntity);
							for (auto scriptField : scriptFields)
							{
								std::string name = scriptField["Name"].as<std::string>();
								std::string typeString = scriptField["Type"].as<std::string>();
								ScriptFieldType type = Utils::ScriptFieldTypeFromString(typeString);
								ScriptFieldInstance& fieldInstance = entityFields[name];
								CRYSTAL_CORE_ASSERT(fields.find(name) != fields.end(), "No Field for Deserialize");
								if (fields.find(name) == fields.end())
									continue;

								fieldInstance.Field = fields.at(name);
								switch (type)
								{
									READ_SCRIPT_FIELD(Float, float);
								}
								std::string data = scriptField["Data"].as<std::string>();
							}
						}
					}
				}

				auto audioComponent = entity["AudioComponent"];
				if (audioComponent)
				{
					auto& ac = deserializedEntity.AddComponent<AudioComponent>();
					std::string audioFilePath = audioComponent["AudioFile"].as<std::string>();
					ac.audioFilePath = Project::GetAssetFileSystemPath(audioFilePath).string();
					ac.volumeMultiplier = audioComponent["VolumeMultiplier"].as<float>();
				}

				auto distanceJointComponent = entity["DistanceJoint2DComponent"];
				if (distanceJointComponent)
				{
					auto& djc = deserializedEntity.AddComponent<DistanceJoint2DComponent>();
					djc.AnchorOffset1 = distanceJointComponent["AnchorOffset1"].as<glm::vec2>();
					djc.AnchorOffset2 = distanceJointComponent["AnchorOffset2"].as<glm::vec2>();
					djc.Attached = distanceJointComponent["Attached"].as<UUID>();
					djc.AttachedStr = distanceJointComponent["AttachedStr"].as<std::string>();
					djc.Damping = distanceJointComponent["Damping"].as<float>();
					djc.RestLength = distanceJointComponent["RestLength"].as<float>();
					djc.MaxDistance = distanceJointComponent["MaxDistance"].as<float>();
					djc.MinDistance = distanceJointComponent["MinDistance"].as<float>();
					djc.ShouldBodiesCollide = distanceJointComponent["ShouldBodiesCollide"].as<bool>();
					djc.Stiffness = distanceJointComponent["Stiffness"].as<float>();
				}

				auto hingeJointComponent = entity["HingeJoint2DComponent"];
				if (hingeJointComponent)
				{
					auto& hjc = deserializedEntity.AddComponent<HingeJoint2DComponent>();
					hjc.AnchorOffset1 = hingeJointComponent["AnchorOffset1"].as<glm::vec2>();
					hjc.AnchorOffset2 = hingeJointComponent["AnchorOffset2"].as<glm::vec2>();
					hjc.Attached = hingeJointComponent["Attached"].as<UUID>();
					hjc.AttachedStr = hingeJointComponent["AttachedStr"].as<std::string>();
					hjc.ShouldBodiesCollide = hingeJointComponent["ShouldBodiesCollide"].as<bool>();
					hjc.EnableLimit = hingeJointComponent["EnableLimit"].as<bool>();
					hjc.EnableMotor = hingeJointComponent["EnableMotor"].as<bool>();
					hjc.LowerLimitAngle = hingeJointComponent["LowerLimitAngle"].as<float>();
					hjc.UpperLimitAngle = hingeJointComponent["UpperLimitAngle"].as<float>();
					hjc.MotorSpeed = hingeJointComponent["MotorSpeed"].as<float>();
					hjc.MaxMotorTorque = hingeJointComponent["MaxMotorTorque"].as<float>();
				}

				auto textRendererComponent = entity["TextRendererComponent"];
				if (textRendererComponent)
				{
					auto& trc = deserializedEntity.AddComponent<TextRendererComponent>();
					trc.TextString = textRendererComponent["TextString"].as<std::string>();
					trc.FontAsset = Font::GetDefaultFont(); // TODO
					trc.Color = textRendererComponent["Color"].as<glm::vec4>();
					trc.Kerning = textRendererComponent["Kerning"].as<float>();
					trc.LineSpacing = textRendererComponent["LineSpacing"].as<float>();
				}
			}
		}

		return true;
	}

	bool SceneSerializer::DeserializeRuntime(const std::string& filepath)
	{
		// Not implemented
		CRYSTAL_CORE_ASSERT(false, "broke");
		return false;
	}

}
