#include "crystalpch.h"
#include "ContentBrowserPanel.h"

#include <Crystal/Projects/Project.h>
#include <Crystal/Utils/PlatformUtils.h>

#include <imgui/imgui.h>

namespace Crystal
{
	ContentBrowserPanel::ContentBrowserPanel()
		: m_BaseDirectory(Project::GetAssetDirectory()), m_CurrentDirectory(m_BaseDirectory)
	{
		m_DirectoryIcon = Texture2D::Create("Resources/Icons/ContentBrowser/FolderIcon.png");
		m_FileIcon = Texture2D::Create("Resources/Icons/FileIcons/FileIcon.png");
		m_FileIconHPP = Texture2D::Create("Resources/Icons/FileIcons/HPPIcon.png");
		m_FileIconCPP = Texture2D::Create("Resources/Icons/FileIcons/CPPIcon.png");
		m_FileIconCS = Texture2D::Create("Resources/Icons/FileIcons/CSIcon.png");
		m_FileIconLUA = Texture2D::Create("Resources/Icons/FileIcons/LUAIcon.png");
		m_FileIconGLSL = Texture2D::Create("Resources/Icons/FileIcons/GLSLIcon.png");
		m_FileIconHLSL = Texture2D::Create("Resources/Icons/FileIcons/HLSLIcon.png");
		m_FileIconScene = Texture2D::Create("Resources/Icons/FileIcons/CrystalIcon.png");
		m_FileIconMP3 = Texture2D::Create("Resources/Icons/FileIcons/MP3Icon.png");
		m_FileIconWAV = Texture2D::Create("Resources/Icons/FileIcons/WAVIcon.png");
		m_ArrowIcon = Texture2D::Create("Resources/Icons/arrow.png");
	}
	
	void ContentBrowserPanel::OnImGuiRender()
	{
		ImGui::Begin("Assets");

		if (m_CurrentDirectory != std::filesystem::path(m_BaseDirectory))
		{
			ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0, 0, 0, 0));
			ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(0.0f, 0.0f, 0.0f, 0.0f));
			ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(0.8f, 0.8f, 0.8f, 0.8f));
			ImGui::ImageButton((ImTextureID)(uint64_t)m_ArrowIcon->GetRendererID(), { 32, 32 }, { 0,1 }, { 1, 0 });
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
				else if (extension == ".mp3") { icon = m_FileIconMP3; }
				else if (extension == ".wav") { icon = m_FileIconWAV; }
				else if (extension == ".crystal") { icon = m_FileIconScene; }
				else { icon = m_FileIcon; }
			}
			else
				icon = m_DirectoryIcon;
			ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0, 0, 0, 0));
			ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(0.0f, 0.0f, 0.0f, 0.0f));
			ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(0.8f, 0.8f, 0.8f, 0.8f));
			ImGui::ImageButton((ImTextureID)(uint64_t)icon->GetRendererID(), { thumbnailSize, thumbnailSize }, { 0,1 }, { 1, 0 });

			if (ImGui::BeginDragDropSource())
			{
				const wchar_t* itemPath = path.c_str();
				ImGui::SetDragDropPayload("CONTENT_BROWSER_ITEM", itemPath, (wcslen(itemPath) + 1) * sizeof(wchar_t), ImGuiCond_Once);
				ImGui::EndDragDropSource();
			}

			ImGui::PopStyleColor();
			ImGui::PopStyleColor();
			ImGui::PopStyleColor();
 			if (ImGui::IsItemHovered() && ImGui::IsMouseDoubleClicked(0))
 			{
				if (directoryEntry.is_directory())
				{
					m_CurrentDirectory /= path.filename();
				}
				else
				{
					if (extension == ".cs")
					{
						/* Open the C# file in Visual Studio 2022 */
						//std::string command = "\"\"C:\\Program Files\\Microsoft Visual Studio\\2022\\Community\\Common7\\IDE\\devenv.exe\" \"" + path.string() + "\"\"";
						//system(command.c_str());
						FileDialogs::OpenVSSolutionAndFile("D:/CrystalGames/Proj/assets/Scripts/Project.sln", path.string().c_str());
					}
				}
 			}
 			ImGui::TextWrapped(filenameString.c_str());
 			ImGui::NextColumn();
			ImGui::PopID();
 		}
 
 		ImGui::Columns(1);

		ImGui::End();
	}
}
