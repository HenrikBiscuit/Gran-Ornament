# Dev board firmware

**Board:** WeAct STM32G030F6P6

## Bring-up checklist

Each step is a small, committed milestone. Tick it off and note results in
[`../../docs/bring-up/`](../../docs/bring-up/).

- [ ] Blink an LED at 1 Hz (toolchain + flashing works)
- [ ] One LED fading via timer PWM, 16-bit resolution, > 1 kHz
- [ ] Gamma-corrected fade: smooth at the very lowest levels, no steps visible
- [ ] Two independent PWM channels (side A / side B)
- [ ] Button: debounce, short press on/off
- [ ] Enter Stop mode and wake on button (EXTI)
- [ ] Measure sleep current (ST-LINK disconnected, jumpers noted)
- [ ] Read VDD via VREFINT
- [ ] 4-hour auto-off (tested with a shortened timer)
