#include "crystalpch.h"
#define MINIAUDIO_IMPLEMENTATION
#include <miniaudio.h>
#include "SoundSystem.h"
#include <Crystal/Core/Core.h>
#include <Crystal/Core/Log.h>
static ma_result result;
static ma_engine engine;

namespace Crystal
{
	void SoundSystem::Init()
	{
		result = ma_engine_init(NULL, &engine);
		if (result != MA_SUCCESS) {
			CRYSTAL_CORE_ASSERT(false, "Audio System Failed to initialize!");
		}
	}
	void SoundSystem::PlayAudio(const char* fileLocation)
	{
		if (fileLocation != "") { ma_engine_play_sound(&engine, "D:/Programming/C++/Crystal/Sapphire-Editor/assets/Sounds/emotionaldamage.mp3", NULL); }
	}
	void SoundSystem::SetVolume(float volume)
	{
		ma_engine_set_volume(&engine, volume);
	}
	void SoundSystem::SetVolumeMultiplier(float volumeMultiplier)
	{
		ma_engine_set_volume(&engine, ma_engine_get_volume(&engine) * volumeMultiplier);
	}
	void SoundSystem::Shutdown()
	{
		ma_engine_uninit(&engine);
	}
}
