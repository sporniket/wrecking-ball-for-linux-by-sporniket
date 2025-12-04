// Copyright (c) 2025 David SPORN
// SPDX-License-Identifier: AGPL-3.0-or-later

#include "core/application.hpp"
#include "core/logging/error_logger.hpp"
#include <iostream>

int main() {
    wreckingball::Application app;

    // Initialize application
    if (!app.Initialize()) {
        std::cerr << "[CRITICAL] Failed to initialize application" << std::endl;
        return 1;
    }

    // Run main game loop
    app.Run();

    // Cleanup
    app.Shutdown();

    return 0;
}
