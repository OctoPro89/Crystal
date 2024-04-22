#pragma once

#undef INFINITE /* INFINITE needs to be undef'ed for MSDF */
#include <msdf-atlas-gen.h>

#include <vector>

namespace Crystal
{
	struct MSDFData
	{
		std::vector<msdf_atlas::GlyphGeometry> Glyphs;
		msdf_atlas::FontGeometry Geometry;
	};
}
