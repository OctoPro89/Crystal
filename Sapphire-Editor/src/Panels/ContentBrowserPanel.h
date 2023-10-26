#pragma once
#include <filesystem>
#include "Crystal/Renderer/Texture.h"

namespace Crystal
{
	class ContentBrowserPanel
	{
	public:
		ContentBrowserPanel();

		void OnImGuiRender();
	private:
		std::filesystem::path m_CurrentDirectory;

 		Ref<Texture2D> m_DirectoryIcon;
		Ref<Texture2D> m_ArrowIcon;
		Ref<Texture2D> m_FileIcon;
		Ref<Texture2D> m_FileIconHPP;
		Ref<Texture2D> m_FileIconCPP;
		Ref<Texture2D> m_FileIconCS;
		Ref<Texture2D> m_FileIconLUA;
		Ref<Texture2D> m_FileIconGLSL;
		Ref<Texture2D> m_FileIconHLSL;
	};
}
