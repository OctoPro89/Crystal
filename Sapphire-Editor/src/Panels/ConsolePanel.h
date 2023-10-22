#pragma once
#include "crystalpch.h"

namespace Crystal
{
	class ConsolePanel
	{
	public:
		ConsolePanel();

		void OnImGuiRender();
		void Log(std::string message);
		void Warn(std::string message);
		void Error(std::string message);
	private:
		std::vector<std::string> Logs =
		{

		};
		std::vector<std::string> Warns =
		{

		};
		std::vector<std::string> Errors =
		{

		};
	};
}
