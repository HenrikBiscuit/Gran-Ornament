# Firmware

Target: **STM32G030F6P6** (Cortex-M0+, 32 KB flash, 8 KB RAM).
Toolchain: VS code (GCC). HAL/LL mix, kept small and readable.

## Layout

| Path | What it is |
|---|---|
| `devboard/` | VS code project for the dev board. Experiments, bring-up tests, pattern tuning. |
| `ornament/` | STM32CuVS codebeIDE project for the real ornament board (created once rev A exists). |

