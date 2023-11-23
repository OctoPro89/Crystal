#include "crystalpch.h"
#include "TextEditorPanel.h"
// std
#include <iostream>

namespace Crystal
{

	void TextEditorPanel::OpenTextEditorWithFile(const char* path)
	{
		std::ifstream file;
		std::string line;
		file.open(path);
		std::ifstream in(path, std::ios::in | std::ios::binary); // ifstream closes itself due to RAII
		if (in)
		{
			in.seekg(0, std::ios::end);
			size_t size = in.tellg();
			if (size != -1)
			{
				buffer.resize(size);
				in.seekg(0, std::ios::beg);
				in.read(&buffer[0], size);
			}
		}
		shouldOpen = true;
	}

	std::string& TextEditorPanel::GetBuffer()
	{
		return buffer;
	}

	void TextEditorPanel::OnImGuiRender()
	{
		ImGui::Begin("Crystal Code Editor");
		ImGui::Text(buffer.c_str());
		ImGui::End();
	}
}
