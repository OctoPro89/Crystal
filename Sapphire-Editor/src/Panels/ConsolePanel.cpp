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
		if (ImGui::Button("Clear"))
			Clear();
		for(MsgType& msg : Messages)
		{
			if (msg.type == 1) /* Info */
				ImGui::TextColored({ 0.835f, 0.835f,1.0f,1.0f }, std::string("INFO: " + msg.msg).c_str());
			else if (msg.type == 2) /* Warn */
				ImGui::TextColored({ 0.725f, 0.661f, 0.176f, 1.0f }, std::string("WARNING: " + msg.msg).c_str());
			else if (msg.type == 3) /* Error */
				ImGui::TextColored({ 0.69f, 0.149f,0.149f,1.0f }, std::string("ERROR: " + msg.msg).c_str());
			else
				CRYSTAL_CORE_ASSERT(false, "Invalid Console Logging Type!");
		}
		ImGui::End();
	}

	void ConsolePanel::Log(std::string message)
	{
		Messages.emplace_back(1, std::move(message));
	}

	void ConsolePanel::Warn(std::string message)
	{
		Messages.emplace_back(2, std::move(message));
	}

	void ConsolePanel::Error(std::string message)
	{
		Messages.emplace_back(3, std::move(message));
	}
	void ConsolePanel::Clear()
	{
		Messages.clear();
	}
}
