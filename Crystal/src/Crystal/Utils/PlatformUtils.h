#pragma once
#include <string>

namespace Crystal {
	class FileDialogs
	{
	public:
		//Returns empty if failed/canceled
		static std::string OpenFile(const char* filter);
		static std::string SaveFile(const char* filter);
		static void OpenVSSolutionAndFile(const char* solutionPath, const char* filePath, int lineNumber = 0);
	};
}
