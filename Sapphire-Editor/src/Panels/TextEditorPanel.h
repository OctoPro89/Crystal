#pragma once
#include <imgui/imgui.h>

namespace Crystal
{
	class TextEditorPanel {
	public:
		void OpenTextEditorWithFile(const char* path);
		void OnImGuiRender();
	};
}
