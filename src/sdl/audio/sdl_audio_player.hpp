// Copyright (c) 2025 David SPORN
// SPDX-License-Identifier: AGPL-3.0-or-later

#pragma once

#include "../../platform/audio/audio_interface.hpp"

namespace wreckingball {

/**
 * @brief SDL2 stub implementation of IAudioPlayer
 *
 * Silent audio player for MVP - no sound effects or music playback.
 * Full audio implementation will be added in a future phase.
 */
class SDLAudioPlayer : public IAudioPlayer {
public:
    /**
     * @brief Constructor
     */
    SDLAudioPlayer();

    /**
     * @brief Destructor
     */
    ~SDLAudioPlayer() override;

    // Prevent copying
    SDLAudioPlayer(const SDLAudioPlayer&) = delete;
    SDLAudioPlayer& operator=(const SDLAudioPlayer&) = delete;

    // IAudioPlayer interface (stub implementations)
    bool Initialize() override;
    void Shutdown() override;
    void PlayMusic(const std::string& music_file) override;
    void StopMusic() override;
    void PlaySoundEffect(SoundEffect effect) override;
    void SetMasterVolume(uint8_t volume) override;
    void SetMusicVolume(uint8_t volume) override;
    void SetSFXVolume(uint8_t volume) override;
    bool IsAudioAvailable() const override;

private:
    bool audio_initialized_;
    uint8_t master_volume_;
    uint8_t music_volume_;
    uint8_t sfx_volume_;
};

} // namespace wreckingball
