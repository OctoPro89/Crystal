#pragma once
#include <imgui/imgui.h>
#include <string>

namespace Crystal
{
	class TextEditorPanel {
	public:
		void OpenTextEditorWithFile(const char* path);
		void OnImGuiRender();
		std::string& GetBuffer();
	private:
		std::string& buffer = std::string("");
		bool shouldOpen = false;
	};
}
