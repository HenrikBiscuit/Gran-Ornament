# Pinout — STM32G030F6P6 (TSSOP-20)

| Pin | Name | Function | Net | Notes |
|---|---|---|---|---|
| 1 | PB8 (PB7) | TIM16_CH1, AF open drain | LED_B1 | Back bottom row, 4 LEDs. FT_f. PB7 unassigned |
| 2 | PB9 / PC14 | – | – | Unused. Shares pad with PC14-OSC32_IN, no LSE crystal |
| 3 | PC15 | – | – | Unused |
| 4 | VDD/VDDA | Supply | +3V3 | From 3.3 V LDO |
| 5 | VSS/VSSA | Ground | GND | |
| 6 | PF2-NRST | Reset | NRST | Test pad for pogo jig |
| 7 | PA0 | GPIO input, pull-up | BUTTON | WKUP1. Side-push button to GND (limit switch on devboard) |
| 8 | PA1 | ADC1_IN1 | VBAT_SENSE | Planned: VBAT divider |
| 9 | PA2 | – | – | Free. Candidate for charger STAT |
| 10 | PA3 | – | – | Free |
| 11 | PA4 | TIM14_CH1, AF open drain | LED_STAR | Both star LEDs. FT_a |
| 12 | PA5 | – | – | Free. Candidate for divider switch |
| 13 | PA6 | TIM3_CH1, AF open drain | LED_B2 | Back middle row, 3 LEDs. FT_ea |
| 14 | PA7 | TIM3_CH2, AF open drain | LED_B3 | Back top row, 2 LEDs. FT_a |
| 15 | PB0 (PA8, PB1, PB2) | TIM1_CH2N, AF open drain | LED_F3 | Front top row, 2 LEDs. Complementary output, polarity Low. Others unassigned |
| 16 | PA11 [PA9] | TIM1_CH4, AF open drain | LED_F1 | Front bottom row, 4 LEDs. FT_fa |
| 17 | PA12 [PA10] | – | – | Unused |
| 18 | PA13 | SWDIO | SWDIO | Test pad for pogo jig |
| 19 | PA14-BOOT0 | SWCLK | SWCLK | Test pad for pogo jig |
| 20 | PB6 (PB3, PB4, PB5) | TIM1_CH3, AF open drain | LED_F2 | Front middle row, 3 LEDs. Others unassigned |

Fixed requirements: SWDIO + SWCLK + NRST + VDD + GND brought to test pads for the pogo-pin jig.
LED pads: never analog, never a pull-down (see DD-006).