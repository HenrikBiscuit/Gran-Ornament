# Changelog

Hardware and firmware are versioned separately.

## Unreleased
### Hardware
- Initial KiCad schematic: STM32G030F6P6, MCP73831 charger, 20 warm-white LEDs in two groups, side-push button.
- LED layout changed to 3 rows per side plus a shared star, 7 PWM channels (DD-004, DD-005).
- Global labels for the 7 LED timer channels and PROG_PWR.
### Firmware
- Dev board bring-up started.
- PWM on all 7 LED channels (TIM1, TIM3, TIM14, TIM16), open drain, 12 bit at ~3.9 kHz.
- SYSCLK lowered to 16 MHz.
- Bench test in `app_run()`: fixed steps and a fade on every channel.
- Button on PA0 (to GND, pull-up): short press = next mode, hold 1.5 s = off, press = wake (DD-009).
- Off = Standby, woken by WKUP1 on PA0. The mode survives Standby in a backup register (DD-010).
- Three placeholder modes (steady, breathe, dim) replace the bench test. 4 h auto-off is active.
- Host tests for the button and the modes.
- Four modes replace the placeholders: high, low, breathe (now 16 s per breath, 8 s up and 8 s down) and candle.
- Candle mode: every channel flickers as its own flame, generated from Perlin-style noise (`candle.hpp`), with host tests.
- Fixed: F3 (PB0, TIM1_CH2N) ran inverted because its CH2N polarity was High. It is now Low (DD-005).
### Tooling
- Lint with `clang-format` and `clang-tidy` (`tools/lint.ps1`). It runs in the pre-push gate and as a `lint` CI job, and covers `App/` and host test code only.
### Docs
- Repository structure, design decision log, power budget template.
- Pinout filled in.
- Design decisions DD-003 to DD-010.
- Bring-up log: LED current, pin leakage and PWM channel tests, button wake and mode change, light modes and the F3 polarity fix.
- DD-005 corrected: CH2N needs OCNPolarity Low.
