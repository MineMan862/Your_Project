#include "SoundEngine.h"
#include <iostream>

// === COSTRUTTORI ===
SoundEngine::SoundEngine()
{
    this->volMenuMusic = 50.0f;
    this->volGameMusic = 50.0f;
    this->volSound = 50.0f;
}

SoundEngine::SoundEngine(const float volMenuMusic, const float volSound)
{
    this->volMenuMusic = volMenuMusic;
    this->volGameMusic = volMenuMusic; // Fallback
    this->volSound = volSound;
    this->device = nullptr;
    this->context = nullptr;
}

// === INIZIALIZZAZIONE AUDIO ===
int SoundEngine::startEngine()
{
    device = alcOpenDevice(nullptr);
    if (!device)
    {
        std::cerr << "[OpenAL] Errore: impossibile aprire il dispositivo audio.\n";
        return 0;
    }

    context = alcCreateContext(device, nullptr);
    if (!context || !alcMakeContextCurrent(context))
    {
        std::cerr << "[OpenAL] Errore: impossibile creare il contesto audio.\n";
        return 0;
    }

    std::cout << "[OpenAL] Inizializzazione completata.\n";
    return 1;
}

// === CHIUSURA AUDIO ===
void SoundEngine::endEngine() const {
    alcMakeContextCurrent(nullptr);
    if (context) alcDestroyContext(context);
    if (device) alcCloseDevice(device);
    std::cout << "[OpenAL] Audio chiuso correttamente.\n";
}

// === VOLUME ===
void SoundEngine::setVolMenuMusic(const float newVolume)
{
    volMenuMusic = newVolume;
}

void SoundEngine::setVolGameMusic(const float newVolume)
{
    volGameMusic = newVolume;
}

void SoundEngine::setVolSuono(const float newVolume)
{
    volSound = newVolume;
}

void SoundEngine::setVolMaster(const float newVolume)
{
    masterVolume = newVolume / 100.0f;
    alListenerf(AL_GAIN, masterVolume);
}

// === OPERATORE ASSEGNAMENTO ===
SoundEngine& SoundEngine::operator=(const SoundEngine &op)
{
    this->volMenuMusic = op.volMenuMusic;
    this->volGameMusic = op.volGameMusic;
    this->volSound = op.volSound;
    return *this;
}
