#include "crystalpch.h"
#include "ScriptGlue.h"
#include "ScriptEngine.h"

#include "Crystal/Core/UUID.h"
#include "Crystal/Core/KeyCodes.h"
#include "Crystal/Core/Input.h"

#include "Crystal/Scene/Scene.h"
#include "Crystal/Scene/Entity.h"

#include "mono/metadata/object.h"
#include "mono/metadata/reflection.h"

#include "box2d/b2_body.h"

namespace Crystal {

	static std::unordered_map<MonoType*, std::function<bool(Entity)>> s_EntityHasComponentFuncs;

#define CRYSTAL_ADD_INTERNAL_CALL(Name) mono_add_internal_call("Crystal.InternalCalls::"#Name, Name)

	static void NativeLog(MonoString* string, int parameter)
	{
		char* cStr = mono_string_to_utf8(string);
		std::string str(cStr);
		mono_free(cStr);
		std::cout << str << ", " << parameter << std::endl;
	}

	static void NativeLog_Vector(glm::vec3* parameter, glm::vec3* outResult)
	{
		*outResult = glm::normalize(*parameter);
	}

	static float NativeLog_VectorDot(glm::vec3* parameter)
	{
		return glm::dot(*parameter, *parameter);
	}

	static bool Entity_HasComponent(UUID entityID, MonoReflectionType* componentType)
	{
		Scene* scene = ScriptEngine::GetSceneContext();
		CRYSTAL_CORE_ASSERT(scene, "No Scene!");
		Entity entity = scene->GetEntityByUUID(entityID);
		CRYSTAL_CORE_ASSERT(entity, "No Entity!");

		MonoType* managedType = mono_reflection_type_get_type(componentType);
		CRYSTAL_CORE_ASSERT(s_EntityHasComponentFuncs.find(managedType) != s_EntityHasComponentFuncs.end(), "s_EntityHasComponentFuncs is false!");
		return s_EntityHasComponentFuncs.at(managedType)(entity);
	}

	static void TransformComponent_GetTranslation(UUID entityID, glm::vec3* outTranslation)
	{
		Scene* scene = ScriptEngine::GetSceneContext();
		CRYSTAL_CORE_ASSERT(scene, "No Scene");
		Entity entity = scene->GetEntityByUUID(entityID);
		CRYSTAL_CORE_ASSERT(entity, "No Entity!");

		*outTranslation = entity.GetComponent<TransformComponent>().Translation;
	}

	static void TransformComponent_SetTranslation(UUID entityID, glm::vec3* translation)
	{
		Scene* scene = ScriptEngine::GetSceneContext();
		CRYSTAL_CORE_ASSERT(scene, "No Scene");
		Entity entity = scene->GetEntityByUUID(entityID);
		CRYSTAL_CORE_ASSERT(entity, "No Entity!");

		entity.GetComponent<TransformComponent>().Translation = *translation;
	}

	static void Rigidbody2DComponent_ApplyLinearImpulse(UUID entityID, glm::vec2* impulse, glm::vec2* point, bool wake)
	{
		Scene* scene = ScriptEngine::GetSceneContext();
		CRYSTAL_CORE_ASSERT(scene, "No Scene");
		Entity entity = scene->GetEntityByUUID(entityID);
		CRYSTAL_CORE_ASSERT(entity, "No Entity!");

		auto& rb2d = entity.GetComponent<Rigidbody2DComponent>();
		b2Body* body = (b2Body*)rb2d.RuntimeBody;
		body->ApplyLinearImpulse(b2Vec2(impulse->x, impulse->y), b2Vec2(point->x, point->y), wake);
	}

	static void Rigidbody2DComponent_ApplyLinearImpulseToCenter(UUID entityID, glm::vec2* impulse, bool wake)
	{
		Scene* scene = ScriptEngine::GetSceneContext();
		CRYSTAL_CORE_ASSERT(scene, "No Scene");
		Entity entity = scene->GetEntityByUUID(entityID);
		CRYSTAL_CORE_ASSERT(entity, "No Entity!");

		auto& rb2d = entity.GetComponent<Rigidbody2DComponent>();
		b2Body* body = (b2Body*)rb2d.RuntimeBody;
		body->ApplyLinearImpulseToCenter(b2Vec2(impulse->x, impulse->y), wake);
	}

	static void SpriteRendererComponent_GetColor(UUID entityID, glm::vec4* outColor)
	{
		Scene* scene = ScriptEngine::GetSceneContext();
		CRYSTAL_CORE_ASSERT(scene, "No Scene");
		Entity entity = scene->GetEntityByUUID(entityID);
		CRYSTAL_CORE_ASSERT(entity, "No Entity!");

		*outColor = entity.GetComponent<SpriteRendererComponent>().Color;
	}

	static void SpriteRendererComponent_SetColor(UUID entityID, glm::vec4* outColor)
	{
		Scene* scene = ScriptEngine::GetSceneContext();
		CRYSTAL_CORE_ASSERT(scene, "No Scene");
		Entity entity = scene->GetEntityByUUID(entityID);
		CRYSTAL_CORE_ASSERT(entity, "No Entity!");

		entity.GetComponent<SpriteRendererComponent>().Color = *outColor;
	}

	static void SpriteRendererComponent_SetTexIndex(UUID entityID, glm::vec2* index)
	{
		Scene* scene = ScriptEngine::GetSceneContext();
		CRYSTAL_CORE_ASSERT(scene, "No Scene");
		Entity entity = scene->GetEntityByUUID(entityID);
		CRYSTAL_CORE_ASSERT(entity, "No Entity!");

		SpriteRendererComponent& spriteRenderer = entity.GetComponent<SpriteRendererComponent>();
		spriteRenderer.SubTex = SubTexture2D::CreateFromCoords(spriteRenderer.Texture, *index, spriteRenderer.cellSize);
	}

	static void SpriteRendererComponent_GetTexIndex(UUID entityID, glm::vec2* outTexIndex)
	{
		Scene* scene = ScriptEngine::GetSceneContext();
		CRYSTAL_CORE_ASSERT(scene, "No Scene");
		Entity entity = scene->GetEntityByUUID(entityID);
		CRYSTAL_CORE_ASSERT(entity, "No Entity!");
		*outTexIndex = *entity.GetComponent<SpriteRendererComponent>().SubTex->GetTexCoords();
	}

	static bool Input_IsKeyDown(KeyCode keycode)
	{
		return Input::IsKeyPressed(keycode);
	}

	template<typename... Component>
	static void RegisterComponent()
	{
		([]()
			{
				std::string_view typeName = typeid(Component).name();
				size_t pos = typeName.find_last_of(':');
				std::string_view structName = typeName.substr(pos + 1);
				std::string managedTypename = fmt::format("Crystal.{}", structName);

				MonoType* managedType = mono_reflection_type_from_name(managedTypename.data(), ScriptEngine::GetCoreAssemblyImage());
				if (!managedType)
				{
					CRYSTAL_CORE_ERROR("Could not find component type {}", managedTypename);
					return;
				}
				s_EntityHasComponentFuncs[managedType] = [](Entity entity) { return entity.HasComponent<Component>(); };
			}(), ...);
	}

	template<typename... Component>
	static void RegisterComponent(ComponentGroup<Component...>)
	{
		RegisterComponent<Component...>();
	}

	void ScriptGlue::RegisterComponents()
	{
		RegisterComponent(AllComponents{});
	}

	void ScriptGlue::RegisterFunctions()
	{
		CRYSTAL_ADD_INTERNAL_CALL(NativeLog);
		CRYSTAL_ADD_INTERNAL_CALL(NativeLog_Vector);
		CRYSTAL_ADD_INTERNAL_CALL(NativeLog_VectorDot);

		CRYSTAL_ADD_INTERNAL_CALL(Entity_HasComponent);
		CRYSTAL_ADD_INTERNAL_CALL(TransformComponent_GetTranslation);
		CRYSTAL_ADD_INTERNAL_CALL(TransformComponent_SetTranslation);

		
		CRYSTAL_ADD_INTERNAL_CALL(SpriteRendererComponent_GetColor);
		CRYSTAL_ADD_INTERNAL_CALL(SpriteRendererComponent_SetColor);
		CRYSTAL_ADD_INTERNAL_CALL(SpriteRendererComponent_GetTexIndex);
		CRYSTAL_ADD_INTERNAL_CALL(SpriteRendererComponent_SetTexIndex);
		

		CRYSTAL_ADD_INTERNAL_CALL(Rigidbody2DComponent_ApplyLinearImpulse);
		CRYSTAL_ADD_INTERNAL_CALL(Rigidbody2DComponent_ApplyLinearImpulseToCenter);

		CRYSTAL_ADD_INTERNAL_CALL(Input_IsKeyDown);
	}
}
