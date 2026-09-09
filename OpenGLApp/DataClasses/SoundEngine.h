#pragma once
#ifdef __APPLE__
#include <OpenAL/al.h>
#include <OpenAL/alc.h>
#else
#include <AL/al.h>
#include <AL/alc.h>
#endif

class SoundEngine
{
public:
	SoundEngine();
	SoundEngine( float volMusic, float volSound);

	int startEngine();
	void endEngine() const;

	void setVolMaster(float newVolume);
	void setVolMenuMusic(float newVolume);
	void setVolGameMusic(float newVolume);
	void setVolSuono(float newVolume);

	SoundEngine &operator=(const SoundEngine &op);

	float masterVolume = 100.0f;
	float volMenuMusic = 50.0f;
	float volGameMusic = 50.0f;
	float volSound = 50.0f;

private:
	ALCdevice* device = nullptr;
	ALCcontext* context = nullptr;

};
