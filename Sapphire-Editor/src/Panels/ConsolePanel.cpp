#include "crystalpch.h"
#include <imgui/imgui.h>
#include "ConsolePanel.h"

namespace Crystal
{
	ConsolePanel::ConsolePanel()
	{

	}

	void ConsolePanel::OnImGuiRender()
	{
		ImGui::Begin("Console");
		for (int i = 0; i < Logs.size(); i++)
		{
			ImGui::TextColored({ 0.835f, 0.835f,1.0f,1.0f }, std::string("INFO: " + Logs[i]).c_str());
		}
		for (int i = 0; i < Warns.size(); i++)
		{
			ImGui::TextColored({ 0.725f, 0.761f, 0.176f, 1.0f }, std::string("WARNING: " + Warns[i]).c_str());
		}
		for (int i = 0; i < Errors.size(); i++)
		{
			ImGui::TextColored({ 0.69f, 0.149f,0.149f,1.0f }, std::string("ERROR!: " + Errors[i]).c_str());
		}
		ImGui::End();
	}

	void ConsolePanel::Log(std::string message)
	{
		Logs.push_back(message);
	}

	void ConsolePanel::Warn(std::string message)
	{
		Warns.push_back(message);
	}

	void ConsolePanel::Error(std::string message)
	{
		Errors.push_back(message);
	}
}
