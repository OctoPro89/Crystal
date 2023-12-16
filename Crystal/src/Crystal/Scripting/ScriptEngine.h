#pragma once

#include "Crystal/Scene/Scene.h"
#include "Crystal/Scene/Entity.h"

#include <filesystem>
#include <string>
#include <map>

class b2Contact;

extern "C" {
	typedef struct _MonoClass MonoClass;
	typedef struct _MonoObject MonoObject;
	typedef struct _MonoMethod MonoMethod;
	typedef struct _MonoAssembly MonoAssembly;
	typedef struct _MonoImage MonoImage;
	typedef struct _MonoClassField MonoClassField;
}

namespace Crystal {

	enum class ScriptFieldType
	{
		None = 0,
		Float, Double, 
		Vector2, Vector3, Vector4,
		Bool, Char,
		Int, UInt, UShort, Short, Long, ULong,
		Byte, SByte,
		Entity
	};

	struct ScriptField
	{
		ScriptFieldType Type;
		std::string Name;
		MonoClassField* ClassField;
	};


	/* ScriptField + Data Storage*/
	struct ScriptFieldInstance
	{
		ScriptField Field;

		ScriptFieldInstance()
		{
			memset(m_Buffer, 0, sizeof(m_Buffer));
		}

		template<typename T>
		T GetValue()
		{
			static_assert(sizeof(T) <= 16, "Type too large!");
			return *(T*)m_Buffer;
		}

		template<typename T>
		void SetValue(T value)
		{
			static_assert(sizeof(T) <= 16, "Type too large!");
			memcpy(m_Buffer, &value, sizeof(T));
		}
	private:
		uint8_t m_Buffer[16];

		friend class ScriptInstance;
		friend class ScriptEngine;
	};

	using ScriptFieldMap = std::unordered_map<std::string, ScriptFieldInstance>;

	class ScriptClass
	{
	public:
		ScriptClass() = default;
		ScriptClass(const std::string& classNamespace, const std::string& className, bool isCore = false);

		MonoObject* Instantiate();
		MonoMethod* GetMethod(const std::string& name, int parameterCount);
		MonoObject* InvokeMethod(MonoObject* instance, MonoMethod* method, void** params = nullptr);

		const std::map<std::string, ScriptField>& GetFields() const { return m_Fields; }
	private:
		std::string m_ClassNamespace;
		std::string m_ClassName;

		std::map<std::string, ScriptField> m_Fields;

		MonoClass* m_MonoClass = nullptr;

		friend class ScriptEngine;
	};

	class ScriptInstance
	{
	public:
		ScriptInstance(Ref<ScriptClass> scriptClass, Entity entity);

		void InvokeOnCreate();
		void InvokeOnUpdate(float ts);
		void InvokeOnCollisionEnter(Entity entity, b2Contact* contact);
		void InvokeOnCollisionExit(Entity entity, b2Contact* contact);

		inline Ref<ScriptClass> GetScriptClass() { return m_ScriptClass; };

		template<typename T>
		inline T GetFieldValue(const std::string& name)
		{
			static_assert(sizeof(T) <= 16, "Type too large!");
			bool success = GetFieldValueInternal(name, s_FieldValueBuffer);
			if (!success)
				return T();
			return *(T*)s_FieldValueBuffer;
		}

		template<typename T>
		inline void SetFieldValue(const std::string& name, T value)
		{
			static_assert(sizeof(T) <= 16, "Type too large!");
			SetFieldValueInternal(name, &value);
		}

		MonoObject* GetManagedObject() { return m_Instance; }
	private:
		bool GetFieldValueInternal(const std::string& name, void* buffer);
		bool SetFieldValueInternal(const std::string& name, const void* value);
	private:
		Ref<ScriptClass> m_ScriptClass;

		MonoObject* m_Instance = nullptr;
		MonoMethod* m_Constructor = nullptr;
		MonoMethod* m_OnCreateMethod = nullptr;
		MonoMethod* m_OnUpdateMethod = nullptr;
		MonoMethod* m_OnCollisionEnterMethod = nullptr;
		MonoMethod* m_OnCollisionExitMethod = nullptr;

		inline static char s_FieldValueBuffer[16];

		friend class ScriptEngine;
		friend struct ScriptFieldInstance;
	};

	class ScriptEngine
	{
	public:
		static void Init();
		static void Shutdown();

		static void LoadAssembly(const std::filesystem::path& filepath);
		static void LoadAppAssembly(const std::filesystem::path& filepath);
		static void ReloadAssembly();

		static void OnRuntimeStart(Scene* scene);
		static void OnRuntimeStop();

		static bool EntityClassExists(const std::string& fullClassName);
		static void OnCreateEntity(Entity entity);
		static void OnUpdateEntity(Entity entity, Timestep ts);
		static void OnCollisionEnter(Entity entity, Entity entity2, b2Contact* contact);
		static void OnCollisionExit(Entity entity, Entity entity2, b2Contact* contact);

		static Scene* GetSceneContext();
		static Ref<ScriptClass> GetEntityClass(const std::string& name);
		static std::unordered_map<std::string, Ref<ScriptClass>> GetEntityClasses();
		static ScriptFieldMap& GetScriptFieldMap(Entity entity);
		static Ref<ScriptInstance> GetEntityScriptInstance(UUID uuid);

		static MonoImage* GetCoreAssemblyImage();

		static MonoObject* GetManagedInstance(UUID uuid);
	private:
		static void InitMono();
		static void ShutdownMono();

		static MonoObject* InstantiateClass(MonoClass* monoClass);
		static void LoadAssemblyClasses();

		friend class ScriptClass;
		friend class ScriptGlue;
	};

	namespace Utils {
		inline const char* ScriptFieldTypeToString(ScriptFieldType type)
		{
			switch (type)
			{
			case Crystal::ScriptFieldType::None:		return "None";	  break;
			case Crystal::ScriptFieldType::Float:		return "Float";	  break;
			case Crystal::ScriptFieldType::Double:		return "Double";  break;
			case Crystal::ScriptFieldType::Bool:		return "Bool";	  break;
			case Crystal::ScriptFieldType::Char:		return "Char";	  break;
			case Crystal::ScriptFieldType::Byte:		return "Byte";    break;
			case Crystal::ScriptFieldType::Short:		return "Short";	  break;
			case Crystal::ScriptFieldType::Int:			return "Int";     break;
			case Crystal::ScriptFieldType::Long:		return "Long";    break;
			case Crystal::ScriptFieldType::SByte:		return "SByte";	  break;
			case Crystal::ScriptFieldType::UShort:		return "UShort";  break;
			case Crystal::ScriptFieldType::UInt:		return "UInt";	  break;
			case Crystal::ScriptFieldType::ULong:		return "ULong";	  break;
			case Crystal::ScriptFieldType::Vector2:		return "Vector2"; break;
			case Crystal::ScriptFieldType::Vector3:		return "Vector3"; break;
			case Crystal::ScriptFieldType::Vector4:		return "Vector4"; break;
			}

			CRYSTAL_CORE_ASSERT(false, "Unknown script field type");
			return "Unknown";
		}

		inline ScriptFieldType ScriptFieldTypeFromString(std::string_view type)
		{
			if (type == "None")		return Crystal::ScriptFieldType::None;
			if (type == "Float")	return Crystal::ScriptFieldType::Float;
			if (type == "Double")	return Crystal::ScriptFieldType::Double;
			if (type == "Bool")		return Crystal::ScriptFieldType::Bool;
			if (type == "Char")		return Crystal::ScriptFieldType::Char;
			if (type == "Byte")		return Crystal::ScriptFieldType::Byte;
			if (type == "Short")	return Crystal::ScriptFieldType::Short;
			if (type == "Int")		return Crystal::ScriptFieldType::Int;
			if (type == "Long")		return Crystal::ScriptFieldType::Long;
			if (type == "SByte")	return Crystal::ScriptFieldType::SByte;
			if (type == "UShort")	return Crystal::ScriptFieldType::UShort;
			if (type == "UInt")		return Crystal::ScriptFieldType::UInt;
			if (type == "ULong")	return Crystal::ScriptFieldType::ULong;
			if (type == "Vector2")	return Crystal::ScriptFieldType::Vector2;
			if (type == "Vector3")	return Crystal::ScriptFieldType::Vector3;
			if (type == "Vector4")	return Crystal::ScriptFieldType::Vector4;

			CRYSTAL_CORE_ASSERT(false, "Unknown ScriptFieldType!");
			return ScriptFieldType::None;
		}
	}
}
