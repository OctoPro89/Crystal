#pragma once

#include <Crystal/Utils/Preferences.h>

namespace Crystal
{
	class PreferenceSerializer
	{
	public:
		PreferenceSerializer(Ref<Preferences> preferences);

		bool Serialize(const std::filesystem::path& filepath);
		bool Deserialize(const std::filesystem::path& filepath);
	private:
		Ref<Preferences> m_Preferences;
	};
}
