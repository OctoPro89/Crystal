#include "crystalpch.h"
#include "ContentBrowserPanel.h"
#include <imgui/imgui.h>

namespace Crystal
{
	// Assets directory
	extern const std::filesystem::path g_AssetPath = "assets";

	ContentBrowserPanel::ContentBrowserPanel()
		: m_CurrentDirectory(g_AssetPath)
	{
		m_DirectoryIcon = Texture2D::Create("Resources/Icons/ContentBrowser/FolderIcon.png");
		m_FileIcon = Texture2D::Create("Resources/Icons/FileIcons/FileIcon.png");
		m_FileIconHPP = Texture2D::Create("Resources/Icons/FileIcons/HPPIcon.png");
		m_FileIconCPP = Texture2D::Create("Resources/Icons/FileIcons/CPPIcon.png");
		m_FileIconCS = Texture2D::Create("Resources/Icons/FileIcons/CSIcon.png");
		m_FileIconLUA = Texture2D::Create("Resources/Icons/FileIcons/LUAIcon.png");
		m_FileIconGLSL = Texture2D::Create("Resources/Icons/FileIcons/GLSLIcon.png");
		m_FileIconHLSL = Texture2D::Create("Resources/Icons/FileIcons/HLSLIcon.png");
		m_ArrowIcon = Texture2D::Create("Resources/Icons/arrow.png");
	}
	
	void ContentBrowserPanel::OnImGuiRender()
	{
		ImGui::Begin("Assets");

		if (m_CurrentDirectory != std::filesystem::path(g_AssetPath))
		{
			ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0, 0, 0, 0));
			ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(0.0f, 0.0f, 0.0f, 0.0f));
			ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(0.8f, 0.8f, 0.8f, 0.8f));
			ImGui::ImageButton((ImTextureID)m_ArrowIcon->GetRendererID(), { 32, 32 }, { 0,1 }, { 1, 0 });
			if (ImGui::IsItemHovered() && ImGui::IsMouseDoubleClicked(0))
			{
				m_CurrentDirectory = m_CurrentDirectory.parent_path();
			}
			ImGui::PopStyleColor(3);
		}
 
 		static float padding = 16.0f;
 		static float thumbnailSize = 128.0f;
 		float cellSize = thumbnailSize + padding;
 
 		float panelWidth = ImGui::GetContentRegionAvail().x;
 		int columnCount = (int)(panelWidth / cellSize);
 		if (columnCount < 1)
 			columnCount = 1;

		ImGui::Columns(columnCount, 0, false);
 
		for (auto& directoryEntry : std::filesystem::directory_iterator(m_CurrentDirectory))
 		{
 			const auto& path = directoryEntry.path();
			auto& extension = path.extension();
			std::string filenameString = path.filename().string();

			ImGui::PushID(filenameString.c_str());
 			Ref<Texture2D> icon = directoryEntry.is_directory() ? m_DirectoryIcon : m_FileIcon;
			if (!directoryEntry.is_directory())
			{
				if (extension == ".hpp") { icon = m_FileIconHPP; }
				else if (extension == ".cpp") { icon = m_FileIconCPP; }
				else if (extension == ".cs") { icon = m_FileIconCS; }
				else if (extension == ".lua") { icon = m_FileIconLUA; }
				else if (extension == ".glsl") { icon = m_FileIconGLSL; }
				else if (extension == ".hlsl") { icon = m_FileIconHLSL; }
				else { icon = m_FileIcon; }
			}
			else
				icon = m_DirectoryIcon;
			ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0, 0, 0, 0));
			ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(0.0f, 0.0f, 0.0f, 0.0f));
			ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(0.8f, 0.8f, 0.8f, 0.8f));
			ImGui::ImageButton((ImTextureID)icon->GetRendererID(), { thumbnailSize, thumbnailSize }, { 0,1 }, { 1, 0 });

			if (ImGui::BeginDragDropSource())
			{
				auto relativePath = std::filesystem::relative(path, g_AssetPath);
				const wchar_t* itemPath = relativePath.c_str();
				ImGui::SetDragDropPayload("CONTENT_BROWSER_ITEM", itemPath, (wcslen(itemPath) + 1) * sizeof(wchar_t), ImGuiCond_Once);
				ImGui::EndDragDropSource();
			}

			ImGui::PopStyleColor();
			ImGui::PopStyleColor();
			ImGui::PopStyleColor();
 			if (ImGui::IsItemHovered() && ImGui::IsMouseDoubleClicked(0))
 			{
				if (directoryEntry.is_directory())
 					m_CurrentDirectory /= path.filename();
 			}
 			ImGui::TextWrapped(filenameString.c_str());
 			ImGui::NextColumn();
			ImGui::PopID();
 		}
 
 		ImGui::Columns(1);

		ImGui::End();
	}
}
