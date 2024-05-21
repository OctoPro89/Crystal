#include "crystalpch.h"
#include "PreferenceSerializer.h"

#include <filesystem>
#include <fstream>

#include <Crystal/Utils/YAMLUtils.h>

namespace Crystal
{
	PreferenceSerializer::PreferenceSerializer(Ref<Preferences> preferences)
		: m_Preferences(preferences)
	{
	}

	bool PreferenceSerializer::Serialize(const std::filesystem::path& filepath)
	{
		YAML::Emitter out;
		out << YAML::BeginMap; // Root

		out << YAML::Key << "Preferences" << YAML::Value;
		out << YAML::BeginMap; // Project

		out << YAML::Key << "SphereColliderColor" << YAML::Value << m_Preferences->SphereColliderColor;
		out << YAML::Key << "QuadColliderColor" << YAML::Value << m_Preferences->QuadColliderColor;
		out << YAML::Key << "EntityOutlineColor" << YAML::Value << m_Preferences->EntityOutlineColor;
		out << YAML::Key << "LineThickness" << YAML::Value << m_Preferences->LineThickness;
		out << YAML::Key << "RenderingAPI" << YAML::Value << (int)m_Preferences->RenderingAPI;
		out << YAML::Key << "ColorScheme" << YAML::Value << m_Preferences->colorScheme;
		out << YAML::Key << "EditorFont" << YAML::Value << m_Preferences->editorFont;
		out << YAML::Key << "VSync" << YAML::Value << m_Preferences->VSync;
		out << YAML::Key << "ShowPhysicsColliders" << YAML::Value << m_Preferences->ShowPhysicsColliders;
		out << YAML::Key << "NearestFiltering" << YAML::Value << m_Preferences->NearestFiltering;

		out << YAML::EndMap; // Project

		out << YAML::EndSeq;
		out << YAML::EndMap; // Root

		std::ofstream fout(filepath);
		fout << out.c_str();

		return true;
	}

	bool PreferenceSerializer::Deserialize(const std::filesystem::path& filepath)
	{
		YAML::Node data;
		try
		{
			data = YAML::LoadFile(filepath.string());
		}
		catch (YAML::ParserException e)
		{
			CRYSTAL_CORE_ERROR("Failed to load preferences file \"{0}\"\n {1}!", filepath, e.what());
			return false;
		}

		auto preferencesNode = data["Preferences"];

		if (!preferencesNode)
		{
			CRYSTAL_CORE_ERROR("Invalid Preferences File!");
			return false;
		}

		m_Preferences->SphereColliderColor = preferencesNode["SphereColliderColor"].as<glm::vec4>();
		m_Preferences->QuadColliderColor = preferencesNode["QuadColliderColor"].as<glm::vec4>();
		m_Preferences->EntityOutlineColor = preferencesNode["EntityOutlineColor"].as<glm::vec4>();
		m_Preferences->LineThickness = preferencesNode["LineThickness"].as<float>();
		m_Preferences->RenderingAPI = (RendererAPI::API)preferencesNode["RenderingAPI"].as<int>();
		m_Preferences->colorScheme = (Preferences::ColorScheme)preferencesNode["ColorScheme"].as<int>();
		m_Preferences->editorFont = (Preferences::EditorFont)preferencesNode["EditorFont"].as<int>();
		m_Preferences->VSync = preferencesNode["VSync"].as<bool>();
		m_Preferences->ShowPhysicsColliders = preferencesNode["ShowPhysicsColliders"].as<bool>();
		m_Preferences->NearestFiltering = preferencesNode["NearestFiltering"].as<bool>();

		return true;
	}
}
