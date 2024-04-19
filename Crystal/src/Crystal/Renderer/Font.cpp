#include <crystalpch.h>
#include "Font.h"

#ifdef CRYSTAL_BUILD_EDITOR
	#define EDITOR_INCLUDE #include "EditorLayer.h"
#endif

#undef INFINITE /* INFINITE needs to be undef'ed for MSDF */
#include <msdf-atlas-gen.h>

namespace Crystal
{
	struct MSDFData
	{
		std::vector<msdf_atlas::GlyphGeometry> Glyphs;
		msdf_atlas::FontGeometry Geometry;
	};

	Font::Font(const std::filesystem::path& filepath)
	{
		msdfgen::FreetypeHandle* ft = msdfgen::initializeFreetype();
		CRYSTAL_CORE_ASSERT(ft, "Freetype font failed to load!");

		std::string filestring = filepath.string();

		msdfgen::FontHandle* font = msdfgen::loadFont(ft, filestring.c_str());

		if (!font)
		{
			CRYSTAL_CORE_ERROR("Failed to load font: {}", filestring);

			EDITOR_ERROR("Failed to load font: " + filestring);
		}
	}
}
