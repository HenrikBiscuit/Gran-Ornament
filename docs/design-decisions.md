# Design decisions

# Design decisions

### DD-001 · MCU: STM32G030F6P6
**Date:** 2026-09
**Decision:** STM32G030F6P6 in TSSOP-20.
**Why:** Cheap, widely stocked at PCBA assemblers, enough timer channels for 7 independent LED channels, Stop/Standby modes for long sleep, hand-probeable package.
**Watch out:** Several TSSOP-20 pads carry more than one GPIO (pad 1 PB7/PB8, pad 2 PB9/PC14, pad 15 PA8/PB0/PB1/PB2, pad 20 PB3–PB6). Only one GPIO per pad can be used, and CubeMX only lets you assign one.
**Revisit if:** More independent LED channels are needed than the timers on the free pads can give.

### DD-002 · Battery
**Date:** – 2026-09
**Decision:** *TBD — capacity, cell size, protection circuit on cell or on board.*

### DD-003 · LED drive: per-LED resistor, MCU pin sinks
**Date:** 2026-09
**Decision:** All anodes on VBAT, one series resistor per LED on the cathode side, each band's resistors joined at one MCU pin in open-drain mode. No MOSFETs. Released pin = off.
**Why:** Fewest parts, no LED current in sleep, and the measured band current (~10 mA for 4 LEDs at 4.2 V) is inside the pin limits.
**Watch out:** A released pin floats up to about VDD, so band pins rely on 5 V tolerance. The pins are only 5 V tolerant when not in analog mode, and a pull-down would light the LEDs faintly.
**Revisit if:** Band current needs to go above ~15 mA per pin.

### DD-004 · Tree layout: 3 rows per side and a shared star
**Date:** 2026-09-29
**Decision:** Each side has 3 rows of 4-3-2 LEDs (bottom to top), each on its own PWM channel. The two star LEDs (one per side) share one channel. 7 channels in total.
**Why:** The planned 8th channel (PB9/TIM17) shares a pad with PC14 and can't be tested on the devboard. A shared star costs little: the star rarely needs to differ between sides.
**Watch out:** Patterns can't move the star from side to side. The charge gauge still has 4 steps (three rows, then the star).
**Revisit if:** A pattern really needs independent stars.

### DD-005 · LED pins and timers
**Date:** 2026-09-29, pins reassigned 2026-10-01
**Decision:** F1 PA11 (TIM1_CH4), F2 PB6 (TIM1_CH3), F3 PB8 (TIM16_CH1), B1 PA4 (TIM14_CH1), B2 PA6 (TIM3_CH1), B3 PA7 (TIM3_CH2), star PB0 (TIM1_CH2N). Front is side 2 and back is side 1 in the schematic. All alternate function open drain, no pull, polarity Low (duty = on-time).
**Why:** The combination CubeMX accepts on separate pads. On 2026-10-01 the schematic moved the star to PB0, B1 to PA4 and F3 to PB8, and the firmware channel table followed.
**Watch out:** PB0 is a complementary output. It's started with HAL_TIMEx_PWMN_Start, and its polarity is set by OCNPolarity, not OCPolarity. It must be `TIM_OCNPOLARITY_LOW` (CubeMX: TIM1 → CH2N Polarity), otherwise the star runs inverted. B1 (4 LEDs, ~10 mA) is now on PA4, an FT_a pin, which has a higher output-low voltage than FT_f, so B1 may look slightly dimmer than F1. Check it on the first PCB. It was High at first and was fixed on 2026-10-01. TIM17 and PB9 are unused. No LSE crystal is possible, because PB9 shares pad 2 with PC14.
**Revisit if:** The PCB layout wants a different pin order.

### DD-006 · Pin safety for LED pads
**Date:** 2026-09-29
**Decision:** Band pins are never put in analog mode and never get a pull-down, including in sleep. Unused GPIOs on shared pads are left unassigned, so CubeMX's "free pins as analog" puts them in analog mode.
**Why:** The bench test with PB7 in analog mode next to a released PB8 showed no measurable leakage (under ~2 µA per LED, no glow).
**Watch out:** The multimeter can only resolve ~2 µA per LED. Confirm with the sleep current on the first PCB.
**Revisit if:** Sleep current on the PCB is higher than the power budget allows.

### DD-007 · Clock and PWM resolution
**Date:** 2026-09-29
**Decision:** SYSCLK 16 MHz from HSI, no PLL. All LED timers at prescaler 0, period 4095: 12 bit at ~3.9 kHz.
**Why:** Lower run current than 64 MHz, which matters next to ~10 mA of LED current. 3.9 kHz is high enough to avoid camera flicker.
**Watch out:** Battery compensation uses about half the range at full battery, so the dimmest fades have fewer steps.
**Revisit if:** A slow fade near off shows visible steps. Then add dithering or go to 32 MHz for 13 bit.

### DD-008 · LED series resistor
**Date:** – 2026-09
**Decision:** *TBD — 470 Ω or 680 Ω. 470 Ω is electrically safe; choose by look at the compensation floor (~60 % duty at full battery).*

### DD-009 · Button: one press to wake, short to change mode, long to switch off
**Date:** 2026-09-29
**Decision:** The button on PA0 goes to GND, with the internal pull-up, so a press reads low. While on, the main loop polls it every ~1 ms (no EXTI). Debounce 20 ms. A short press moves to the next mode, and holding for 1.5 s switches off. The press that wakes the ornament is ignored until it is released.
**Why:** Only one button, so it has to do everything. Polling is simple and easy to test on a PC (`Button` class, host tests). Wake-up from Standby doesn't need EXTI, it uses the WKUP1 pin (DD-010). The long press fires while the button is still held, so you see the LEDs go off without letting go.
**Watch out:** The devboard uses a limit switch (NO to PA0, common to GND). Pressed, the pull-up draws ~80 µA; released, nothing.
**Revisit if:** The loop gets too busy to poll every few ms, or the modes need a double press.

### DD-010 · Off means Standby, mode kept in a backup register
**Date:** 2026-09-29
**Decision:** Switching off (long press or the 4 h auto-off) turns the LEDs off, waits for the button to be released, and enters Standby. PA0 is WKUP1 and wakes the MCU on a falling edge. The PWR pull-up keeps PA0 high in Standby. Waking is a reset, and the current mode is read back from TAMP backup register 0 (`0x4752'0000 | mode`, anything else means the default mode).
**Why:** Standby is the lowest-current mode on the G030 (sub-µA, to be measured). Every other GPIO goes Hi-Z, so the open-drain LED pins are released and the LEDs stay off with no pull-down on the pads (DD-006). The backup register survives Standby and NRST, so the ornament comes back in the same mode.
**Watch out:** The debugger drops when the MCU enters Standby: press the button (or connect under reset) to flash again. The mode is lost when the battery is disconnected. If the button is stuck pressed (e.g. squeezed in the box), the ornament wakes, stays on for 4 h, then waits forever for the release with the CPU running. Not handled yet.
**Revisit if:** Measured Standby current is higher than the power budget allows, or the ornament must stay off after the battery is first connected.
