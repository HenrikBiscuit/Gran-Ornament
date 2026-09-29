# Changelog

Hardware and firmware are versioned separately.

## Unreleased
### Hardware
- Initial KiCad schematic: STM32G030F6P6, MCP73831 charger, 20 warm-white LEDs in two groups, side-push button.
- LED layout changed to 3 rows per side plus a shared star, 7 PWM channels (DD-004, DD-005).
### Firmware
- Dev board bring-up started.
- PWM on all 7 LED channels (TIM1, TIM3, TIM14, TIM16), open drain, 12 bit at ~3.9 kHz.
- SYSCLK lowered to 16 MHz.
- Bench test in `app_run()`: fixed steps and a fade on every channel.
### Tooling
- Lint with `clang-format` and `clang-tidy` (`tools/lint.ps1`). It runs in the pre-push gate and as a `lint` CI job, and covers `App/` and host test code only.
### Docs
- Repository structure, design decision log, power budget template.
- Pinout filled in.
- Design decisions DD-003 to DD-008.
- Bring-up log: LED current, pin leakage and PWM channel tests.