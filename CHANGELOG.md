# Changelog

All notable changes to the Tiresias Firmware project will be documented in this file.

The format is based on [Keep a Changelog](https://keepachangelog.com/en/1.0.0/),
and this project adheres to [Semantic Versioning](https://semver.org/spec/v2.0.0.html).

## [Unreleased]

## [0.1.2] - 2024-07-29

### Added
- Automated release process with GitHub Actions
- Version bumping script for easier version management
- Release script for automating tag creation and pushing
- Comprehensive versioning documentation
- Pull request template with versioning guidelines

## [0.1.0] - 2024-09-20

### Added
- Initial project structure
- Core architecture implementation with modular thread-based design
- Storage module with NVS-based persistence for user settings
- Peripheral module for buttons and LEDs with callback-based event handling
- ADAU1787 driver implementation with I2C communication
- I2S control for audio data transfer
- Audio codec service with task-based architecture
- Version tracking system

[Unreleased]: https://github.com/felipepimentab/tiresias-fw/compare/v0.1.2...HEAD
[0.1.2]: https://github.com/felipepimentab/tiresias-fw/compare/v0.1.0...v0.1.2
[0.1.0]: https://github.com/felipepimentab/tiresias-fw/releases/tag/v0.1.0
