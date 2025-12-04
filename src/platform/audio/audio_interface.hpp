// Copyright (c) 2025 David SPORN
// SPDX-License-Identifier: AGPL-3.0-or-later

#pragma once

#include "sound_effect.hpp"
#include <string>
#include <cstdint>

namespace wreckingball {

/**
 * @brief Platform-agnostic audio playback interface
 *
 * Abstracts audio operations to enable future Android porting.
 * SDL2_mixer implementation will be in src/sdl/audio/sdl_audio_player.hpp
 */
class IAudioPlayer {
public:
    virtual ~IAudioPlayer() = default;

    /**
     * @brief Initialize audio system
     * @return true on success, false on failure (game continues silently)
     */
    virtual bool Initialize() = 0;

    /**
     * @brief Shutdown audio system
     */
    virtual void Shutdown() = 0;

    /**
     * @brief Play background music (loops continuously)
     * @param music_file Path to music file
     */
    virtual void PlayMusic(const std::string& music_file) = 0;

    /**
     * @brief Stop background music
     */
    virtual void StopMusic() = 0;

    /**
     * @brief Play sound effect (one-shot, non-blocking)
     * @param effect Sound effect type to play
     */
    virtual void PlaySoundEffect(SoundEffect effect) = 0;

    /**
     * @brief Set master volume (0-100)
     * @param volume Volume level (0 = silent, 100 = max)
     */
    virtual void SetMasterVolume(uint8_t volume) = 0;

    /**
     * @brief Set music volume (0-100)
     * @param volume Volume level (0 = silent, 100 = max)
     */
    virtual void SetMusicVolume(uint8_t volume) = 0;

    /**
     * @brief Set sound effects volume (0-100)
     * @param volume Volume level (0 = silent, 100 = max)
     */
    virtual void SetSFXVolume(uint8_t volume) = 0;

    /**
     * @brief Check if audio device is available
     * @return true if audio device present, false if silent mode
     */
    virtual bool IsAudioAvailable() const = 0;
};

} // namespace wreckingball
