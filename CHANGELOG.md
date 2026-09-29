# Changelog

Hardware and firmware are versioned separately.

## Unreleased
### Hardware
- Initial KiCad schematic: STM32G030F6P6, MCP73831 charger, 20 warm-white LEDs in two groups, side-push button.
### Firmware
- Dev board bring-up started.
### Tooling
- Lint with `clang-format` and `clang-tidy` (`tools/lint.ps1`). It runs in the pre-push gate and as a `lint` CI job, and covers `App/` and host test code only.
### Docs
- Repository structure, design decision log, power budget template.
