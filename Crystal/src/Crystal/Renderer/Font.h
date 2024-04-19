#pragma once

#include <filesystem>

namespace Crystal
{
	struct MSDFData;

	class Font
	{
	public:
		Font(const std::filesystem::path& filepath);

	private:
		std::unique_ptr<MSDFData> m_Glyphs;
	};
}
