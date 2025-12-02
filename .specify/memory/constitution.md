<!--
Sync Impact Report:
Version: 0.0.0 → 1.0.0 (Initial constitution establishment)
Rationale: MAJOR bump - Initial ratification establishing core governance principles

Modified Principles: N/A (Initial creation)
Added Sections:
  - Core Principles (I-V)
  - Legal & Licensing Requirements
  - Documentation Standards
  - Governance

Templates Requiring Updates:
  ✅ .specify/templates/plan-template.md - Compatible (no changes needed)
  ✅ .specify/templates/spec-template.md - Compatible (no changes needed)
  ✅ .specify/templates/tasks-template.md - Compatible (no changes needed)

Follow-up TODOs: None - all placeholders resolved
-->

# Wrecking Ball Constitution

## Core Principles

### I. Cross-Platform Foundation

The game MUST be developed as a native Linux desktop application first. All core game logic, rendering systems, and user interfaces MUST be designed with portability in mind to support future Android OS deployment. Platform-specific code MUST be isolated from core game mechanics through abstraction layers.

**Rationale**: Starting with Linux ensures desktop development velocity while maintaining architectural discipline for mobile adaptation. Clean separation between platform-agnostic game logic and platform-specific I/O prevents technical debt during Android porting.

### II. Legal Compliance (NON-NEGOTIABLE)

All source code, documentation, and assets MUST:
- Include copyright notice following this exact pattern: "Copyright (c) [years] David SPORN" where [years] is a comma-separated list from 2025 to current year (e.g., "Copyright (c) 2025 David SPORN" or "Copyright (c) 2025, 2026 David SPORN")
- Be licensed under GNU Affero General Public License v3.0 or later (AGPL-3.0-or-later)
- Include full license text in repository root (LICENSE file)
- Include SPDX license identifiers in source file headers where applicable

**Rationale**: Legal requirements are non-negotiable and must be enforced at repository creation and in every file. AGPL-3.0-or-later ensures copyleft protection including network use scenarios. The copyright notice format ensures proper legal attribution with year tracking.

### III. Brick Breaker Genre Fidelity

The game MUST implement core brick breaker mechanics:
- Paddle controlled by player (keyboard/mouse on Linux, touch on future Android)
- Ball physics with collision detection and realistic bounce angles
- Brick layouts with destructible bricks
- Score tracking and progression systems
- Power-ups and special effects (as defined in feature specifications)

**Rationale**: Genre requirements define the game's identity and ensure feature development stays aligned with player expectations for brick breaker gameplay.

### IV. Performance & Responsiveness

The game MUST maintain:
- Minimum 60 FPS (frames per second) during active gameplay on target hardware
- Input latency below 16ms (one frame at 60 FPS)
- Smooth ball physics without stuttering or frame drops
- Efficient memory usage suitable for both desktop and future mobile constraints

**Rationale**: Arcade-style games demand responsive controls and fluid motion. Performance degradation breaks immersion and playability, especially for reflex-based brick breaker mechanics.

### V. Testing & Quality Assurance

All game systems MUST be testable:
- Unit tests for game logic (physics, collision detection, scoring)
- Integration tests for system interactions (input → game state → rendering)
- Manual playtesting checklists for user experience validation
- Performance profiling to verify FPS and latency requirements

**Rationale**: Automated testing ensures game mechanics remain consistent through refactoring. Manual playtesting validates fun factor and user experience elements that cannot be easily automated.

## Legal & Licensing Requirements

### Copyright & Attribution

Every source file MUST include a header block containing:
```
Copyright (c) 2025 David SPORN
SPDX-License-Identifier: AGPL-3.0-or-later
```

As years progress, the copyright notice MUST be updated to reflect the range of contribution years, for example:
```
Copyright (c) 2025, 2026 David SPORN
SPDX-License-Identifier: AGPL-3.0-or-later
```

The repository root MUST contain:
- `LICENSE` file with complete AGPL-3.0 license text
- `COPYRIGHT` file (if applicable) with detailed copyright notices

### Third-Party Dependencies

Any third-party libraries or assets MUST:
- Have licenses compatible with AGPL-3.0-or-later
- Be documented in a `THIRD-PARTY-NOTICES.md` file
- Include original copyright notices as required by their licenses

Incompatible licenses (e.g., proprietary, GPL-incompatible) MUST be rejected during dependency selection.

## Documentation Standards

All documentation MUST adhere to:
- **Language**: English (en-US or en-GB consistently)
- **Units**: Metric system (meters, kilograms, Celsius, etc.)
- **Dates**: ISO 8601 format (YYYY-MM-DD)
- **Time**: ISO 8601 format (HH:MM:SS or HH:MM:SS.sss with UTC offset when relevant)

Examples:
- Date: 2025-12-02 (not 12/02/2025 or 02-12-2025)
- Measurement: "Ball speed: 5.5 m/s" (not "18 fps" or "12 mph")
- Timestamp: "2025-12-02T14:30:00+01:00"

**Rationale**: International standards ensure documentation clarity across global contributors and prevent ambiguity in technical specifications.

## Governance

### Amendment Process

This constitution may be amended through:
1. Documented proposal describing change rationale
2. Impact analysis on existing code, templates, and workflows
3. Version bump following semantic versioning:
   - **MAJOR**: Backward-incompatible principle removals or redefinitions
   - **MINOR**: New principles or materially expanded guidance
   - **PATCH**: Clarifications, wording improvements, typo fixes
4. Update of all dependent templates and documentation

### Compliance Reviews

All design documents (specifications, plans, tasks) MUST include a "Constitution Check" section verifying compliance with applicable principles.

Pull requests and code reviews MUST verify:
- Legal headers present and correct
- Documentation follows language/units/date standards
- Performance requirements met (if applicable to changeset)
- Test coverage adequate for changes

### Complexity Justification

Any deviation from constitutional principles (e.g., performance below 60 FPS in specific edge case, third-party dependency evaluation) MUST be documented in the "Complexity Tracking" section of the implementation plan with:
- Description of the violation
- Justification for why it is necessary
- Explanation of why simpler alternatives were rejected

**Version**: 1.0.0 | **Ratified**: 2025-12-02 | **Last Amended**: 2025-12-02
