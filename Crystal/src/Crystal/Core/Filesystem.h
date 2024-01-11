#pragma once
#include <filesystem>
#include <Crystal/Core/Buffer.h>

namespace Crystal::Utils
{
	class FileSystem
	{
	public:
		static Buffer ReadBytes(const std::filesystem::path& filepath);
	};
}
