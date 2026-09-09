#pragma once
#include <string>
#include <iostream>
#include "SoundEngine.h"

using namespace std;


class SoundManager
{
public:
	SoundManager() = default;
	SoundManager(std::string filename, float volume, int soundType, SoundEngine* engine_ptr);

	void playSound();
	void stopSound() const;
	bool isPlaying() const;
	void changeVolume() const;

	bool is_playing = false;

	SoundManager(SoundManager&& op) noexcept;
	SoundManager& operator=(SoundManager&& op) noexcept;
	~SoundManager();

private:
	std::string filename;
	float volume = 0.0f;
	int soundType = 0; // 0 = SFX, 1 = Menu Music, 2 = Game Music
	unsigned int buffer = 0;
	unsigned int source = 0;

	SoundEngine* engine_ptr = nullptr;
	// Disable copy to prevent OpenAL resource issues
	SoundManager(const SoundManager&) = delete;
	SoundManager& operator=(const SoundManager&) = delete;
};
