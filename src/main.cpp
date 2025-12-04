// Copyright (c) 2025 David SPORN
// SPDX-License-Identifier: AGPL-3.0-or-later

#include <iostream>
#include <SDL2/SDL.h>

int main() {
    std::cout << "Sporniket's Wrecking Ball - v0.1.0" << std::endl;
    std::cout << "Copyright (c) 2025 David SPORN" << std::endl;
    std::cout << "Licensed under AGPL-3.0-or-later" << std::endl;
    std::cout << std::endl;

    // Initialize SDL2
    if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO) < 0) {
        std::cerr << "SDL initialization failed: " << SDL_GetError() << std::endl;
        return 1;
    }

    std::cout << "SDL2 initialized successfully!" << std::endl;
    std::cout << "Game will be implemented in Phase 2 and beyond." << std::endl;

    // Cleanup
    SDL_Quit();

    return 0;
}
