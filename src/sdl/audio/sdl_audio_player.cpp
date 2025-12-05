// Copyright (c) 2025 David SPORN
// SPDX-License-Identifier: AGPL-3.0-or-later

#include "sdl_audio_player.hpp"
#include "../../core/logging/error_logger.hpp"
#include <SDL2/SDL.h>

namespace wreckingball {

SDLAudioPlayer::SDLAudioPlayer()
    : audio_initialized_(false),
      master_volume_(100),
      music_volume_(100),
      sfx_volume_(100) {
}

SDLAudioPlayer::~SDLAudioPlayer() {
    Shutdown();
}

bool SDLAudioPlayer::Initialize() {
    // Try to initialize SDL audio subsystem
    if (SDL_InitSubSystem(SDL_INIT_AUDIO) < 0) {
        ErrorLogger::GetInstance().LogError("Failed to initialize SDL audio: " + std::string(SDL_GetError()));
        ErrorLogger::GetInstance().LogError("Game will run in silent mode");
        audio_initialized_ = false;
        return false;  // Gracefully handle no audio device (T055)
    }

    audio_initialized_ = true;
    return true;
}

void SDLAudioPlayer::Shutdown() {
    if (audio_initialized_) {
        SDL_QuitSubSystem(SDL_INIT_AUDIO);
        audio_initialized_ = false;
    }
}

void SDLAudioPlayer::PlayMusic(const std::string& music_file) {
    // Stub: No music playback in MVP
    (void)music_file;
}

void SDLAudioPlayer::StopMusic() {
    // Stub: No music playback in MVP
}

void SDLAudioPlayer::PlaySoundEffect(SoundEffect effect) {
    // Stub: No sound effects in MVP
    (void)effect;
}

void SDLAudioPlayer::SetMasterVolume(uint8_t volume) {
    master_volume_ = volume > 100 ? 100 : volume;
}

void SDLAudioPlayer::SetMusicVolume(uint8_t volume) {
    music_volume_ = volume > 100 ? 100 : volume;
}

void SDLAudioPlayer::SetSFXVolume(uint8_t volume) {
    sfx_volume_ = volume > 100 ? 100 : volume;
}

bool SDLAudioPlayer::IsAudioAvailable() const {
    return audio_initialized_;
}

} // namespace wreckingball
