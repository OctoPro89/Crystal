#pragma once

namespace Crystal
{
	class SoundSystem
	{
	public:
		static void Init();
		static void PlayAudio(const char* fileLocation);
		static void Stop();
		static void SetVolume(float volume);
		static void SetVolumeMultiplier(float volumeMultiplier);
		static void Shutdown();
	};
}
