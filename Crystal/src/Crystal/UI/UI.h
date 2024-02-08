#pragma once

#include <imgui/imgui.h>

namespace Crystal::UI
{
	struct ScopedStyleColor
	{
		ScopedStyleColor() = default;

		ScopedStyleColor(ImGuiCol idx, ImVec4 color, bool predicate = false)
			: m_Set(predicate)
		{
			if(predicate)
				ImGui::PushStyleColor(idx, color);
		}

		ScopedStyleColor(ImGuiCol idx, ImU32 color, bool predicate = false)
			: m_Set(predicate)
		{
			if(predicate)
				ImGui::PushStyleColor(idx, color);
		}

		~ScopedStyleColor()
		{
			if(m_Set)
				ImGui::PopStyleColor();
		}
	private:
		bool m_Set = false;
	};
}
