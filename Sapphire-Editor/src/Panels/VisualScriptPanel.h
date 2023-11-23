#pragma once
#include "TextEditorPanel.h"
#include "ConsolePanel.h"

namespace Crystal
{
	class VisualScriptPanel
	{
	private:
		enum class VScriptTemplate
		{
			RBMove = 0,
			TransMove = 1,
		};
		enum class VScriptInfo
		{
		};
		std::string scriptName;
		std::string script;
		VScriptTemplate crntTemplate;
		bool useTemplate;
		void GenerateCSharpScript(VScriptTemplate templ, bool UseTemplate, ConsolePanel& panel);//, VScriptInfo info);
	public:
		void OpenTemplate(VScriptTemplate templ, TextEditorPanel& tEditPanel);
		void OnImGuiRender(TextEditorPanel& tEditPanel, ConsolePanel& panel);
	};
}
