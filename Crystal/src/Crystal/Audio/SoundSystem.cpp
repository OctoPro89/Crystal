#include "crystalpch.h"
#define MINIAUDIO_IMPLEMENTATION
#include <miniaudio.h>
#include "SoundSystem.h"
#include <Crystal/Core/Core.h>
#include <Crystal/Core/Log.h>

static ma_result result;
static ma_engine engine;
static float originalVolume;

namespace Crystal
{
	void SoundSystem::Init()
	{
		result = ma_engine_init(NULL, &engine);
		if (result != MA_SUCCESS)
			CRYSTAL_CORE_ASSERT(false, "Audio System Failed to initialize!");
		originalVolume = ma_engine_get_volume(&engine);
	}

	void SoundSystem::PlayAudio(const char* fileLocation)
	{
		if (fileLocation != "") { ma_engine_play_sound(&engine, fileLocation, NULL); }
	}

	void SoundSystem::Stop()
	{
		ma_engine_uninit(&engine);
		ma_engine_init(NULL, &engine);
	}

	void SoundSystem::SetVolume(float volume)
	{
		ma_engine_set_volume(&engine, volume);
	}

	void SoundSystem::SetVolumeMultiplier(float volumeMultiplier)
	{
		ma_engine_set_volume(&engine, originalVolume * volumeMultiplier);
	}

	void SoundSystem::Shutdown()
	{
		ma_engine_uninit(&engine);
	}
}
