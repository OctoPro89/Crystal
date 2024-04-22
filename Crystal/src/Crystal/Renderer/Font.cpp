#include <crystalpch.h>
#include "Font.h"

// #define CRYSTAL_NO_EDITOR
#include <Helpers/EditorHelper.h>
#include <Crystal/UI/MSDFData.h>

#undef INFINITE /* INFINITE needs to be undef'ed for MSDF */
#include <msdf-atlas-gen.h>

namespace Crystal
{


	template<typename T, typename S, int N, msdf_atlas::GeneratorFunction<S, N> GenFunc>
	static Ref<Texture2D> CreateAndCacheAtlas(const std::string& fontName, float fontSize, const std::vector<msdf_atlas::GlyphGeometry>& glyphs, const msdf_atlas::FontGeometry& fontGeometry, uint32_t width, uint32_t height)
	{
		msdf_atlas::GeneratorAttributes attributes;
		attributes.config.overlapSupport = true;
		attributes.scanlinePass = true;

		msdf_atlas::ImmediateAtlasGenerator<S, N, GenFunc, msdf_atlas::BitmapAtlasStorage<T, N>> generator(width, height);
		generator.setAttributes(attributes);
		generator.setThreadCount(8);
		generator.generate(glyphs.data(), (int)glyphs.size());

		msdfgen::BitmapConstRef<T, N> bitmap = (msdfgen::BitmapConstRef<T, N>)generator.atlasStorage();

		TextureSpecification spec;
		spec.Width = bitmap.width;
		spec.Height = bitmap.height;
		spec.Format = ImageFormat::RGB8;
		spec.GenerateMips = false;

		Ref<Texture2D> texture = Texture2D::Create(spec);
		texture->SetData((void*)bitmap.pixels, bitmap.width * bitmap.height * 3);

		return texture;
	}

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

		int width, height;
		atlasPacker.getDimensions(width, height);
		emSize = atlasPacker.getScale();

		/* If msdf or mtsdf */

		#define DEFAULT_ANGLE_THRESHOLD 3.0
		#define LCG_MULTIPLIER 6364136223846793005ull
		#define LCG_INCREMENT 1442695040888963407ull
		#define THREAD_COUNT 8

		uint64_t coloringSeed = 0;
		bool expensiveColoring = false;
		if (expensiveColoring)
		{
			msdf_atlas::Workload([&glyphs = m_Data->Glyphs, &coloringSeed](int i, int threadNo) -> bool
			{
				unsigned long long glyphSeed = (LCG_MULTIPLIER * (coloringSeed ^ i) + LCG_INCREMENT) * !!coloringSeed;
				glyphs[i].edgeColoring(msdfgen::edgeColoringInkTrap, DEFAULT_ANGLE_THRESHOLD, glyphSeed);
				return true;
			}, (int)m_Data->Glyphs.size()).finish(THREAD_COUNT);
		}
		else
		{
			unsigned long long glyphSeed = coloringSeed;

			for (msdf_atlas::GlyphGeometry& glyph : m_Data->Glyphs)
			{
				glyphSeed *= LCG_MULTIPLIER;
				glyph.edgeColoring(msdfgen::edgeColoringInkTrap, DEFAULT_ANGLE_THRESHOLD, glyphSeed);
			}
		}

		m_AtlasTexture = CreateAndCacheAtlas<uint8_t, float, 3, msdf_atlas::msdfGenerator>("Test", (float)emSize, m_Data->Glyphs, m_Data->Geometry, width, height);

		msdfgen::destroyFont(font);
		msdfgen::deinitializeFreetype(ft);
	}

	Font::~Font()
	{
		delete m_Data;
	}
	Ref<Font> Font::GetDefaultFont()
	{
		static Ref<Font> DefaultFont;

		if (!DefaultFont)
			DefaultFont = CreateRef<Font>("Resources/Fonts/OpenSans/OpenSans-Regular.ttf");

		return DefaultFont;
	}
}
