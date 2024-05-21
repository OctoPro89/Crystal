#include "crystalpch.h"
#include "Crystal/Utils/PlatformUtils.h"

#include <commdlg.h>
#include <GLFW/glfw3.h>
#define GLFW_EXPOSE_NATIVE_WIN32
#include <GLFW/glfw3native.h>

#include "Crystal/Core/Application.h"

#include <filesystem>

namespace Crystal
{
	std::string FileDialogs::OpenFile(const char* filter)
	{
		OPENFILENAMEA ofn;
		CHAR szFile[260]{ 0 };
		ZeroMemory(&ofn, sizeof(OPENFILENAME));
		ofn.lStructSize = sizeof(OPENFILENAME);
		ofn.hwndOwner = glfwGetWin32Window((GLFWwindow*)Application::Get().GetWindow().GetNativeWindow());
		ofn.lpstrFile = szFile;
		ofn.nMaxFile = sizeof(szFile);
		ofn.lpstrFilter = filter;
		ofn.nFilterIndex = 1;
		ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST | OFN_NOCHANGEDIR;
		if (GetOpenFileNameA(&ofn) == TRUE)
		{
			return ofn.lpstrFile;
		}

		return std::string();
	}

	std::string FileDialogs::SaveFile(const char* filter)
	{
		OPENFILENAMEA ofn;
		CHAR szFile[260]{ 0 };
		ZeroMemory(&ofn, sizeof(OPENFILENAME));
		ofn.lStructSize = sizeof(OPENFILENAME);
		ofn.hwndOwner = glfwGetWin32Window((GLFWwindow*)Application::Get().GetWindow().GetNativeWindow());
		ofn.lpstrFile = szFile;
		ofn.nMaxFile = sizeof(szFile);
		ofn.lpstrFilter = filter;
		ofn.nFilterIndex = 1;
		ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST | OFN_NOCHANGEDIR;
		if (GetSaveFileNameA(&ofn) == TRUE)
		{
			return ofn.lpstrFile;
		}

		return std::string();
	}

	bool ExecuteCommandAsync(std::string& command)
	{
		STARTUPINFOA si;
		PROCESS_INFORMATION pi;

		// Initialize process information structures
		ZeroMemory(&si, sizeof(si));
		si.cb = sizeof(si);
		ZeroMemory(&pi, sizeof(pi));

		// Create a mutable copy of the command string
		// CreateProcessA requires the command string to be mutable
		char* commandMutable = new char[command.length() + 1];
		strcpy_s(commandMutable, command.length() + 1, command.c_str());

		// Create the process
		if (!CreateProcessA(NULL,    // No module name (use command line)
			commandMutable,  // Command line
			NULL,    // Process handle not inheritable
			NULL,    // Thread handle not inheritable
			FALSE,   // Set handle inheritance to FALSE
			0,       // No creation flags
			NULL,    // Use parent's environment block
			NULL,    // Use parent's starting directory
			&si,     // Pointer to STARTUPINFOA structure
			&pi))    // Pointer to PROCESS_INFORMATION structure
		{
			std::cerr << "CreateProcess failed (" << GetLastError() << ")" << std::endl;
			delete[] commandMutable; // Free the allocated memory
			return false;
		}

		// Close process and thread handles
		CloseHandle(pi.hProcess);
		CloseHandle(pi.hThread);

		delete[] commandMutable; // Free the allocated memory
		return true;
	}

	void FileDialogs::OpenVSSolutionAndFile(const char* solutionPath, const char* filePath, int lineNumber)
	{
		/* TODO: Let user pick vs2022 path */
		std::string devenvPath = "\"C:\\Program Files\\Microsoft Visual Studio\\2022\\Community\\Common7\\IDE\\devenv.exe\"";

		/* Put together the command to execute */
		std::string command = devenvPath + " \"" + solutionPath + "\" /Command \"File.OpenFile " + std::filesystem::absolute(filePath).string();// + ":" + std::to_string(lineNumber) + "\"";

		/* Execute the command */
		int result = ExecuteCommandAsync(command);

		/* Check if the command was executed successfully */
		if (result == -1)
		{
			CRYSTAL_CORE_ERROR("Failed to open Visual Studio 2022 Solution!");
		}
	}
}
