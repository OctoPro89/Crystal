#include "VisualScriptPanel.h"
#include "TextEditorPanel.h"
#include <imgui/imgui.h>
#include <fstream>

namespace Crystal
{
	void VisualScriptPanel::OpenTemplate(VScriptTemplate templ, TextEditorPanel& tEditPanel)
	{
		if (templ == VScriptTemplate::RBMove) {
			tEditPanel.OpenTextEditorWithFile("Resources/Examples/RigidbodyPlayerMove.cs");
			script = tEditPanel.GetBuffer();
		}
		else if (templ == VScriptTemplate::TransMove) {
			tEditPanel.OpenTextEditorWithFile("Resources/Examples/TransformPlayerMove.cs");
			script = tEditPanel.GetBuffer();
		}
	}
	void VisualScriptPanel::OnImGuiRender(TextEditorPanel& tEditPanel, ConsolePanel& panel)
	{
		ImGui::Begin("Crystal Visual Scripting");
		ImGui::Checkbox("Use Template", &useTemplate);
		if (useTemplate)
		{
			ImGui::InputText("Script Name:", (char*)scriptName.c_str(), sizeof(scriptName));
			if (ImGui::TreeNodeEx("Movement"))
			{
				bool isSelected;
				if (ImGui::MenuItem("Player Movement (Rigidbody)")) {
					isSelected = true;
					OpenTemplate(VScriptTemplate::RBMove, tEditPanel);
					crntTemplate = VScriptTemplate::RBMove;
				}
				if (ImGui::MenuItem("Player Movement (Transform)")) {
					isSelected = true;
					OpenTemplate(VScriptTemplate::TransMove, tEditPanel);
					crntTemplate = VScriptTemplate::TransMove;
				}
				else
					isSelected = false;
				ImGui::TreePop();
			}
			if (ImGui::Button("Generate Script"))
			{
				GenerateCSharpScript(crntTemplate, useTemplate, panel);
			}
		}
		ImGui::End();
	}
	void VisualScriptPanel::GenerateCSharpScript(VScriptTemplate templ, bool UseTemplate, ConsolePanel& panel)
	{
		std::fstream fstream;
		std::string path = "assets/Scripts/VisualScript.cs";
		fstream.open(path, std::fstream::out);
		fstream.write(script.c_str(), script.length());
		panel.Log("Visual Script Created:" + path);
	}
}
