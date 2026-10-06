# Bring-up log

Dated lab notes from the bench, newest at the bottom.

## 2026-09-24 · First blink

**Goal:** Program the board and blink LEDs
**Setup:** board, firmware commit, instruments
**What happened:** Time will tell
**Next:** Setting up git, then getting to it

## 2026-09-25 · First blink

**Goal:** Program the board and blink LEDs
**Setup:** board, firmware commit, instruments
**What happened:** The board blinked
**Next:** Implementing CI and Googletest

## 2026-09-25 · CI and Googletest

**Goal:** Have Github actions build project on multiple platforms and report any failures 
**Setup:** Github
**What happened:** Github was setup according to goals, and tested with fail cases
**Next:** Implementing PWM

## 2026-09-28 · PWM implementation

**Goal:** Have a dimable LED from A PWM signal
**Setup:** board, firmware commit, instruments
**What happened:** TIM3_CH1 was setup on pin PA6 (previously used for testing the LED), and is able to walk through a full duty cycle using internal timers (0-100% brigthness)
**Next:** Write useable function and setup tests for it

## 2026-09-29 · LED current and pin checks

**Goal:** Measure the real LED current on one band, and check that a released pin doesn't leak current through the LEDs
**Setup:** devboard, 4-LED test string (470 Ω per LED, anodes on a LiPo, cathodes sinking into PA6), multimeter
**What happened:** At VBAT 4.13 V with the pin held low: VF 2.70 V, 1.10 V across the resistor (2.34 mA per LED), 0.33 V on the pin at ~9.4 mA total. That works out to ~10 mA for a 4-LED band at 4.2 V, well inside the pin limits. A released pin floats up to 2.9–3.3 V. PB8 was tested with its shared-pad spare PB7 left in analog: the pad sits at VDD (3.32 V), the voltage across a resistor reads 0.000 V (under ~2 µA per LED) and there is no glow. Only one resistor has been measured so far.
**Next:** Drive every band pin from its own timer

## 2026-09-29 · All PWM channels

**Goal:** Drive every LED band from its own timer channel
**Setup:** devboard, 4-LED test string moved from pin to pin, firmware commit, multimeter
**What happened:** SYSCLK lowered to 16 MHz, all timers at period 4095 (12 bit, ~3.9 kHz), pins as alternate function open drain with polarity Low. A bench test steps through 0/25/50/100 % and then fades. PA11, PB6, PB0 (TIM1_CH2N), PB8, PA6, PA7 and PA4 all behave correctly, with no flash at reset. PB9/TIM17 could not be tested because the devboard's C14 header isn't connected to the chip (there is a 32.768 kHz crystal on PC14/PC15). PB9 is dropped, and the two star LEDs now share PA4.
**Next:** Button on PA0 (limit switch wired, NO)


## 2026-09-30 · Button wake and mode change

**Goal:** Wake from Standby with the button, change mode with a short press, switch off with a long press
**Setup:** devboard, limit switch (NO to PA0, common to GND), 4-LED test string, firmware commit
**What happened:** PA0 changed to input with pull-up (the switch goes to GND). Three placeholder modes: steady, breathe, dim. Short press = next mode, hold 1.5 s = off (Standby), press = wake in the same mode. Tried on the board and it works as expected. Host tests for the button and modes pass.
**Next:** Measure Standby current with the ST-Link unplugged. Later: handle a button that is stuck pressed (DD-010).


## 2026-10-01 · Light modes and F3 polarity

**Goal:** Replace the placeholder modes with real ones
**Setup:** devboard, limit switch on PA0, 4-LED test string, firmware commit
**What happened:** Four modes: high, low, breathe (16 s per breath) and candle (every channel flickers as its own flame). F3 (PB0, TIM1_CH2N) had CH2N Polarity High in CubeMX. Changed it to Low, and after some troubleshooting F3 now works like the other channels. Host tests for the modes and the candle pass.
**Next:** Measure Standby current with the ST-Link unplugged.

## 2026-10-06 · Battery measurement and cutoff

**Goal:** Read the battery voltage, keep brightness constant as it drains, and switch off before the cell runs flat
**Setup:** devboard, 1M/1M divider with 0.1 µF on PA1 (taped and heatshrunk, not probeable), bench supply on VBAT, multimeter, firmware commit
**What happened:** ADC1_IN1 at 160.5 cycles, calibrated at boot. A blink-rate test (faster at higher VBAT) showed the reading follows the supply. Brightness compensation holds every mode at its 3.6 V brightness, and the cutoff fades out, pulses three times and goes to Standby. Uncalibrated, the 3.6 V cutoff tripped at 3.76 V in High and 3.73 V in Low: ~30 mV is lost in the bench leads under load, ~0.8 % comes from VDD (3.326 V), the rest from the divider. A multimeter on PA1 (500 kΩ source) pulled it down ~5 % and tripped the cutoff while measuring. With a +3.5 % calibration the cutoff trips at 3.63 V in High and 3.61 V in Low.
**Next:** Check the divider and calibration on the first PCB (1 % resistors). Measure Standby current with the divider in place.
