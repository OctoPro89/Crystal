#pragma once

#include <string>

namespace Crystal::Utils
{
	enum class Platform
	{
		Windows
	};

	class LauncherUtils
	{
	public:
		static void CopyDirectory(std::string src, std::string dst, Platform platform);
		static void OpenSapphireEditor(std::string args);
		static void SetWorkingDirectory(std::string directory);
	};
}
