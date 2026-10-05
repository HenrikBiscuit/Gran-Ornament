# 2026-10-05 · LED current vs. battery voltage

Results from the devboard bench test. Lab notes are in [README.md](README.md), raw data in [2026-10-05-led-current-sweep.csv](2026-10-05-led-current-sweep.csv).

## Setup

| | |
|---|---|
| Supply | Bench PSU in place of the LiPo, 4.2 V → 3.0 V in 0.1 V steps |
| MCU supply | MCP1700 LDO, ST-Link 3.3 V wire removed |
| LEDs | 10, wired as on the ornament: rows of 4, 3 and 2, plus 1 star LED |
| Series resistor | 470 Ω per LED |
| Band pins | Held low continuously (no PWM) |
| PSU resolution | 1 mA |
| Room temperature | 22 °C |

## PSU readings (mA)

| VBAT (V) | All off | Row 1 (4 LEDs) | Row 2 (3) | Row 3 (2) | Star (1) | All on |
|---:|---:|---:|---:|---:|---:|---:|
| 4.2 | 1 | 10 | 9 | 6 | 4 | 26 |
| 4.1 | 1 | 10 | 8 | 6 | 3 | 24 |
| 4.0 | 1 | 9 | 8 | 5 | 3 | 22 |
| 3.9 | 1 | 9 | 7 | 5 | 3 | 21 |
| 3.8 | 1 | 8 | 6 | 5 | 3 | 20 |
| 3.7 | 1 | 8 | 6 | 4 | 3 | 18 |
| 3.6 | 1 | 7 | 5 | 4 | 3 | 17 |
| 3.5 | 1 | 6 | 5 | 4 | 2 | 15 |
| 3.4 | 1 | 5 | 4 | 3 | 2 | 13 |
| 3.3 | 1 | 5 | 4 | 4 | 3 | 12 |
| 3.2 | 1 | 5 | 4 | 3 | 2 | 11 |
| 3.1 | 1 | 4 | 4 | 3 | 2 | 9 |
| 3.0 | 1 | 4 | 3 | 3 | 2 | 8 |

An all-LEDs re-check at 4.2 V at the end of the test read 26 mA again. The 3.3 V readings for row 3 and the star are 1 mA high, probably a misread; the all-on reading at 3.3 V is on the line.

## Current per LED and compensation

Measured = (all on − all off) ÷ 10. Fit: **I = (VBAT − 2.55 V) / 672 Ω**. Duty is what holds the brightness at the 3.5 V level: duty = (3.5 − 2.55) / (VBAT − 2.55), capped at 100 %.

| VBAT (V) | Measured (mA/LED) | Fit (mA/LED) | Compensation duty |
|---:|---:|---:|---:|
| 4.2 | 2.50 | 2.46 | 58 % |
| 4.1 | 2.30 | 2.31 | 61 % |
| 4.0 | 2.10 | 2.16 | 66 % |
| 3.9 | 2.00 | 2.01 | 70 % |
| 3.8 | 1.90 | 1.86 | 76 % |
| 3.7 | 1.70 | 1.71 | 83 % |
| 3.6 | 1.60 | 1.56 | 90 % |
| 3.5 | 1.40 | 1.41 | 100 % |
| 3.4 | 1.20 | 1.26 | 100 % |
| 3.3 | 1.10 | 1.12 | 100 % |
| 3.2 | 1.00 | 0.97 | 100 % |
| 3.1 | 0.80 | 0.82 | 100 % |
| 3.0 | 0.70 | 0.67 | 100 % |

## 470 Ω vs. 680 Ω

The full-to-floor ratio depends only on V0 = 2.55 V, so both resistors need ~58 % duty at 4.2 V. The 680 Ω values are estimated with the same ~200 Ω of LED and pin resistance.

| | 470 Ω (measured) | 680 Ω (estimated) |
|---|---:|---:|
| Per LED at 4.2 V | 2.46 mA | ~1.9 mA |
| Per LED at 3.5 V | 1.41 mA | ~1.1 mA |
| 4-LED row at 4.2 V | ~9 mA | ~7.5 mA |
| All ten LEDs at 4.2 V | ~25 mA | ~19 mA |
| Duty at 4.2 V for a 3.5 V floor | 58 % | 58 % |
