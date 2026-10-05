# 2026-09-24 · First blink

**Goal:** Program the board and blink LEDs

**Setup:** board, firmware commit, instruments

**What happened:** Time will tell

**Next:** Setting up git, then getting to it

# 2026-09-25 · First blink

**Goal:** Program the board and blink LEDs

**Setup:** board, firmware commit, instruments

**What happened:** The board blinked

**Next:** Implementing CI and Googletest

# 2026-09-25 · CI and Googletest

**Goal:** Have Github actions build project on multiple platforms and report any failures 

**Setup:** Github

**What happened:** Github was setup according to goals, and tested with fail cases

**Next:** Implementing PWM

# 2026-09-28 · PWM implementation

**Goal:** Have a dimable LED from A PWM signal

**Setup:** board, firmware commit, instruments

**What happened:** TIM3_CH1 was setup on pin PA6 (previously used for testing the LED), and is able to walk through a full duty cycle using internal timers (0-100% brigthness)

**Next:** Write useable function and setup tests for it

# 2026-09-29 · LED current and pin checks

**Goal:** Measure the real LED current on one band, and check that a released pin doesn't leak current through the LEDs

**Setup:** devboard, 4-LED test string (470 Ω per LED, anodes on a LiPo, cathodes sinking into PA6), multimeter

**What happened:** At VBAT 4.13 V with the pin held low: VF 2.70 V, 1.10 V across the resistor (2.34 mA per LED), 0.33 V on the pin at ~9.4 mA total. That works out to ~10 mA for a 4-LED band at 4.2 V, well inside the pin limits. A released pin floats up to 2.9–3.3 V. PB8 was tested with its shared-pad spare PB7 left in analog: the pad sits at VDD (3.32 V), the voltage across a resistor reads 0.000 V (under ~2 µA per LED) and there is no glow. Only one resistor has been measured so far.

**Next:** Drive every band pin from its own timer

# 2026-09-29 · All PWM channels

**Goal:** Drive every LED band from its own timer channel

**Setup:** devboard, 4-LED test string moved from pin to pin, firmware commit, multimeter

**What happened:** SYSCLK lowered to 16 MHz, all timers at period 4095 (12 bit, ~3.9 kHz), pins as alternate function open drain with polarity Low. A bench test steps through 0/25/50/100 % and then fades. PA11, PB6, PB0 (TIM1_CH2N), PB8, PA6, PA7 and PA4 all behave correctly, with no flash at reset. PB9/TIM17 could not be tested because the devboard's C14 header isn't connected to the chip (there is a 32.768 kHz crystal on PC14/PC15). PB9 is dropped, and the two star LEDs now share PA4.

**Next:** Button on PA0 (limit switch wired, NO)

# 2026-09-30 · Button wake and mode change

**Goal:** Wake from Standby with the button, change mode with a short press, switch off with a long press

**Setup:** devboard, limit switch (NO to PA0, common to GND), 4-LED test string, firmware commit

**What happened:** PA0 changed to input with pull-up (the switch goes to GND). Three placeholder modes: steady, breathe, dim. Short press = next mode, hold 1.5 s = off (Standby), press = wake in the same mode. Tried on the board and it works as expected. Host tests for the button and modes pass.

**Next:** Measure Standby current with the ST-Link unplugged. Later: handle a button that is stuck pressed (DD-010).

# 2026-10-01 · Light modes and F3 polarity

**Goal:** Replace the placeholder modes with real ones

**Setup:** devboard, limit switch on PA0, 4-LED test string, firmware commit

**What happened:** Four modes: high, low, breathe (16 s per breath) and candle (every channel flickers as its own flame). F3 (PB0, TIM1_CH2N) had CH2N Polarity High in CubeMX. Changed it to Low, and after some troubleshooting F3 now works like the other channels. Host tests for the modes and the candle pass.

**Next:** Measure Standby current with the ST-Link unplugged.

# 2026-10-05 · LED current vs. battery voltage

**Goal:** Measure how LED current falls with battery voltage, to set the brightness compensation and help choose the series resistor (DD-008)

**Setup:** devboard on a bench PSU instead of the LiPo, MCP1700 LDO, 10 LEDs wired as on the ornament (rows of 4, 3, 2 and 1 star LED, 470 Ω each), band pins held low (no PWM), ST-Link 3.3 V wire removed, PSU resolution 1 mA, 22 °C. Tables: [2026-10-05-led-current-sweep.md](2026-10-05-led-current-sweep.md), raw data: [2026-10-05-led-current-sweep.csv](2026-10-05-led-current-sweep.csv)

**What happened:** Stepped 4.2 V → 3.0 V in 0.1 V steps for six setups (all rows off, each row alone, all on). Baseline was 1 mA at every step. An all-LEDs re-check at 4.2 V at the end of the test read 26 mA, the same as at the start, so nothing drifted. With all rows on, the LED current is a straight line: I = (VBAT − 2.55 V) / 672 Ω, from 2.45 mA per LED at 4.2 V down to 0.67 mA at 3.0 V. No point is more than ~0.05 mA off the line. It agrees with the 2026-09-29 single-string test (2.34 mA at 4.13 V; the fit gives 2.35 mA). The ~200 Ω on top of the 470 Ω comes from the LED and the pin's output resistance (~35 Ω, shared by every LED on the pin). The star pin carries only one LED, so it should be ~15–20 % brighter than the 4-LED row. The per-row readings point that way, but at 1 mA resolution they are too coarse to confirm it. The 4-LED row draws ~9 mA and all ten LEDs ~25 mA at 4.2 V, well inside the pin limits. There's no knee above 3.0 V. Row 3 and the star read 1 mA high at 3.3 V; the all-on reading there is on the line, so this was probably a misread.

**Result:** Compensation uses V0 = 2.55 V, not a datasheet VF: duty = (3.5 − 2.55) / (VBAT − 2.55), capped at 100 %, which is ~58 % at 4.2 V. The full-to-floor ratio depends only on V0, so the resistor value does not change it. 680 Ω only makes everything dimmer (~1.9 mA per LED at 4.2 V, estimated) and saves battery (~19 mA instead of ~25 mA with all ten LEDs on).

**Watch out:** The MCP1700 drops out around 3.3–3.4 V, after which VDD follows the battery. A VBAT reading that uses VDD as the ADC reference will drift right where compensation matters most.

**Next:** Decide DD-008 by brightness and battery life, then implement the compensation.
