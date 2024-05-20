#include "LauncherUtils.h"
#include <cstdlib>

namespace Crystal::Utils
{
	void LauncherUtils::CopyDirectory(std::string src, std::string dst, Platform platform)
	{
		switch (platform)
		{
			case Platform::Windows:
			{
				std::string command = "Xcopy /E /I \"" + src + "\" \"" + dst + "\"";

				system(command.c_str());
			}
		}
	}

	void LauncherUtils::OpenSapphireEditor(std::string args)
	{
		std::string command = "start ../Sapphire-Editor/Sapphire-Editor.exe " + args;

		system(command.c_str());
	}

	void LauncherUtils::SetWorkingDirectory(std::string directory)
	{
		std::string command = "chdir \"" + directory + "\"";

		system(command.c_str());
	}
}
