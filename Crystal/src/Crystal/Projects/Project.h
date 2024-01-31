#pragma once

#include <Crystal/Core/Core.h>
#include <string>
#include <filesystem>

namespace Crystal
{
	struct ProjectConfig
	{
		std::string Name = "Untitled";

		std::filesystem::path StartScene;

		std::filesystem::path AssetDirectory;
		std::filesystem::path ScriptModulePath;
	};

	class Project
	{
	public:
		inline static const std::filesystem::path& GetProjectDirectory()
		{
			CRYSTAL_CORE_ASSERT(s_ActiveProject, "Project does not exist!");
			return s_ActiveProject->m_ProjectDirectory;
		}

		inline static std::filesystem::path GetAssetDirectory()
		{
			CRYSTAL_CORE_ASSERT(s_ActiveProject, "Project does not exist!");
			return GetProjectDirectory() / s_ActiveProject->m_Config.AssetDirectory;
		}

		/* TODO: Move to asset manager */
		static std::filesystem::path GetAssetFileSystemPath(const std::filesystem::path& path)
		{
			CRYSTAL_CORE_ASSERT(s_ActiveProject, "Project does not exist!");
			return GetAssetDirectory() / path;
		}

		inline ProjectConfig& GetConfig() { return m_Config; }

		static Ref<Project> GetActive() { return s_ActiveProject; }

		static Ref<Project> New();
		static Ref<Project> Load(const std::filesystem::path& path);
		static bool SaveActive(const std::filesystem::path& path);
	private:
		ProjectConfig m_Config;
		std::filesystem::path m_ProjectDirectory;

		inline static Ref<Project> s_ActiveProject;
	};
}
