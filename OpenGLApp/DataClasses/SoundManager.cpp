#include "SoundManager.h"
#include "SoundEngine.h"
#include <iostream>
#include <utility>
#include <vector>
#include <sndfile.h> // per caricare WAV/OGG

SoundManager::SoundManager(std::string filename, float volume, int soundType, SoundEngine* engine_ptr)
{
    this->filename = std::move(filename);
    this->volume = volume;
    this->soundType = soundType;
    this->engine_ptr = engine_ptr;
    this->buffer = 0;
    this->source = 0;
    this->is_playing = false;
}

void SoundManager::playSound()
{
    if (engine_ptr == nullptr) return;

    SF_INFO sfInfo;
    SNDFILE* sndFile = sf_open(filename.c_str(), SFM_READ, &sfInfo);
    if (!sndFile) {
        std::cerr << "[OpenAL] Errore: impossibile aprire file audio " << filename << "\n";
        return;
    }

    std::vector<short> samples(sfInfo.frames * sfInfo.channels);
    sf_readf_short(sndFile, samples.data(), sfInfo.frames);
    sf_close(sndFile);

    alGenBuffers(1, &buffer);
    alBufferData(buffer, sfInfo.channels == 1 ? AL_FORMAT_MONO16 : AL_FORMAT_STEREO16,
                 samples.data(), samples.size() * sizeof(short), sfInfo.samplerate);

    alGenSources(1, &source);
    alSourcei(source, AL_BUFFER, buffer);
    if (soundType == 1 || soundType == 2) {
        alSourcei(source, AL_LOOPING, AL_TRUE);
    }

    float vol = 0.0f;
    if (soundType == 1) vol = engine_ptr->volMenuMusic / 100.0f;
    else if (soundType == 2) vol = engine_ptr->volGameMusic / 100.0f;
    else vol = engine_ptr->volSound / 100.0f;
    
    alSourcef(source, AL_GAIN, vol);
    alSourcePlay(source);

    is_playing = true;
}

void SoundManager::stopSound() const
{
    if (is_playing) {
        // We cast away const to modify these members since the function was declared const in the header modification
        alSourceStop(source);
        alDeleteSources(1, const_cast<ALuint*>(&source));
        alDeleteBuffers(1, const_cast<ALuint*>(&buffer));
        const_cast<SoundManager*>(this)->is_playing = false;
    }
}

bool SoundManager::isPlaying() const
{
    ALint state;
    alGetSourcei(source, AL_SOURCE_STATE, &state);
    return (state == AL_PLAYING);
}

void SoundManager::changeVolume() const {
    if (source != 0) {
        float newVolume = 0.0f;
        if (soundType == 1) newVolume = engine_ptr->volMenuMusic / 100.0f;
        else if (soundType == 2) newVolume = engine_ptr->volGameMusic / 100.0f;
        else newVolume = engine_ptr->volSound / 100.0f;
        
        alSourcef(source, AL_GAIN, newVolume);
    }
}

SoundManager::SoundManager(SoundManager&& op) noexcept
{
    this->engine_ptr = op.engine_ptr;
    this->filename = std::move(op.filename);
    this->volume = op.volume;
    this->soundType = op.soundType;
    this->buffer = op.buffer;
    this->source = op.source;
    this->is_playing = op.is_playing;

    op.buffer = 0;
    op.source = 0;
    op.is_playing = false;
}

SoundManager& SoundManager::operator=(SoundManager&& op) noexcept
{
    if (this == &op) return *this;
    
    stopSound();

    this->engine_ptr = op.engine_ptr;
    this->filename = std::move(op.filename);
    this->volume = op.volume;
    this->soundType = op.soundType;
    
    this->buffer = op.buffer;
    this->source = op.source;
    this->is_playing = op.is_playing;

    op.buffer = 0;
    op.source = 0;
    op.is_playing = false;

    return *this;
}

SoundManager::~SoundManager()
{
    stopSound();
}
