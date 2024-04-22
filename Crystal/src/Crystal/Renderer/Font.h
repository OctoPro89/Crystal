#pragma once

#include <Crystal/Core/Core.h>
#include <Crystal/Renderer/Texture.h>

#include <filesystem>

namespace Crystal
{
	struct MSDFData;

	class Font
	{
	public:
		Font(const std::filesystem::path& filepath);
		~Font();

		inline MSDFData* GetMSDFData() const { return m_Data; }
		inline Ref<Texture2D> GetAtlasTexture() const { return m_AtlasTexture; }

		static Ref<Font> GetDefaultFont();
	private:
		MSDFData* m_Data;
		Ref<Texture2D> m_AtlasTexture;
	};
}
