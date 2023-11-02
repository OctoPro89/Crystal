#include "crystalpch.h"
#include "TextEditorPanel.h"
// std
#include <iostream>
#include <fstream>

namespace Crystal
{

	void TextEditorPanel::OpenTextEditorWithFile(const char* path)
	{
		std::ifstream in(path);
	}

	void TextEditorPanel::OnImGuiRender()
	{
		ImGui::Begin("Crystal Code Editor");

		ImGui::End();
	}
}
