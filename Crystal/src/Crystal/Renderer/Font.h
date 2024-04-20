#pragma once

#include <Crystal/Core/Core.h>

#include <filesystem>

namespace Crystal
{
	struct MSDFData;

	class Font
	{
	public:
		Font(const std::filesystem::path& filepath);
		~Font();

	private:
		MSDFData* m_Data;
	};
}
