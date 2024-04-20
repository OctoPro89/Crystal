#include <crystalpch.h>
#include "Font.h"

// #define CRYSTAL_NO_EDITOR
#include <Helpers/EditorHelper.h>

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
		: m_Data(new MSDFData())
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

		struct CharsetRange
		{
			uint32_t Begin, End;
		};

		/* From ImGui (imgui_draw.cpp) */
		static const CharsetRange charsetRanges[] =
		{
			{ 0x0020, 0x00FF } /* Basic Latin + Latin Supplement */
		};

		msdf_atlas::Charset charset;

		for (CharsetRange range : charsetRanges)
		{
			for (uint32_t c = range.Begin; c <= range.End; c++)
				charset.add(c);
		}

		double fontScale = 1.0;
		m_Data->Geometry = msdf_atlas::FontGeometry(&m_Data->Glyphs);
		int glyphsLoaded = m_Data->Geometry.loadCharset(font, fontScale, charset);
		CRYSTAL_CORE_INFO("Loaded {} glyphs from font (out of {})", glyphsLoaded, charset.size());

		double emSize = 40.0;

		msdf_atlas::TightAtlasPacker atlasPacker;
		atlasPacker.setPixelRange(2.0);
		atlasPacker.setMiterLimit(1.0);
		atlasPacker.setPadding(0);
		atlasPacker.setScale(emSize);

		int remaining = atlasPacker.pack(m_Data->Glyphs.data(), (int)m_Data->Glyphs.size());

		CRYSTAL_CORE_ASSERT(remaining == 0, "Glyphs still remaining!");

		

		msdfgen::destroyFont(font);
		msdfgen::deinitializeFreetype(ft);
	}

	Font::~Font()
	{
		delete m_Data;
	}
}
