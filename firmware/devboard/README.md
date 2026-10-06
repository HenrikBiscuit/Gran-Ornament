# Dev board firmware

**Board:** WeAct STM32G030F6P6

## Bring-up checklist

Each step is a small, committed milestone. Tick it off and note results in
[`../../docs/bring-up/`](../../docs/bring-up/).

- [x] Blink an LED (toolchain + flashing works)
- [x] PWM on all 7 LED channels, 12 bit at ~3.9 kHz (DD-007)
- [x] Button: debounce, short press = next mode, long press = off (DD-009)
- [x] Standby and wake on the button via WKUP1 (DD-010)
- [x] Light modes: high, low, breathe, candle
- [x] Battery measurement on PA1, brightness compensation, 3.6 V cutoff (DD-011)
- [x] Fade in and out when switching on and off, "charge me" pulses
- [ ] 4-hour auto-off tested on the bench with a shortened timer (host-tested so far)
- [ ] Measure Standby current (ST-LINK disconnected, divider in place)
- [ ] Gamma-corrected fades: smooth at the very lowest levels, no steps visible
- [ ] Read VDD via VREFINT
