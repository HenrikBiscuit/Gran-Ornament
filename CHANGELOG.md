# Changelog

Hardware and firmware are versioned separately.

## Unreleased
### Hardware
- Initial KiCad schematic: STM32G030F6P6, MCP73831 charger, 20 warm-white LEDs in two groups, side-push button.
- LED layout changed to 3 rows per side plus a shared star, 7 PWM channels (DD-004, DD-005).
- Global labels for the 7 LED timer channels and PROG_PWR.
- LED pins reassigned: star on PB0 (TIM1_CH2N), back bottom row on PA4 (TIM14_CH1), front top row on PB8 (TIM16_CH1) (DD-005).
- Rev 1 PCB layout. Power path: USB-C, MCP73831 charger (100 mA), DW01A + FS8205A protection, TPS7A0233 LDO. 470 Ω per LED, 1M/1M VBAT divider on PA1.
- Rev 1 fabrication files for JLCPCB (Gerbers, BOM, CPL) in `hardware/fabrication/Rev1/`. First batch ordered.
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
- Channel table follows the new LED pins: star on PB0, B1 on PA4, F3 on PB8.
- Battery measurement on PA1 (ADC1_IN1, `VBAT_SENSE`) through a 1M/1M divider, read once a second (`battery.hpp`, `battery.cpp`).
- Brightness compensation: every mode looks as bright as at 3.6 V, at any battery voltage (DD-011).
- Low-battery cutoff at 3.6 V: fade out, three soft "charge me" pulses, then Standby.
- Fade in at switch-on and fade out at switch-off, 1 s each (`fade.hpp`).
- Dev board battery calibration (+3.5 %), measured on the bench.
- Main loop sleeps (`__WFI`) until the next 1 ms tick instead of busy-waiting in `HAL_Delay`.
- Host tests for the battery maths, the cutoff and the fades.
### Tooling
- Lint with `clang-format` and `clang-tidy` (`tools/lint.ps1`). It runs in the pre-push gate and as a `lint` CI job, and covers `App/` and host test code only.
### Docs
- Repository structure, design decision log, power budget template.
- Pinout filled in.
- Design decisions DD-003 to DD-010.
- Bring-up log: LED current, pin leakage and PWM channel tests, button wake and mode change, light modes and the F3 polarity fix.
- DD-005 corrected: CH2N needs OCNPolarity Low.
- DD-011 battery measurement and compensation. DD-007, DD-008 and DD-010 updated to match.
- Power budget: storage drain calculated for a 250 mAh cell.
- Pinout: PA1 is VBAT_SENSE.
- Bring-up log: battery measurement and cutoff.
- All Markdown files brought up to date: README status, parts and roadmap, hardware revisions, datasheet list, firmware module overview, dev-board checklist. DD-002 and DD-008 decided. The bring-up log renders as Markdown again.
