# Gran Ornament

A small, rechargeable, two-sided LED Christmas ornament. Twenty warm-white LEDs in seven channels (three rows per side and a shared star) play slow and calm light: steady high or low, a slow breath, or a flickering candle. One button turns it on, it fades in and out, switches itself off after four hours, and sleeps quietly between Christmases.

Made by hand in small batches in Denmark. This repository is the open build log: hardware, firmware, and the notes behind the decisions.

> **Status:** 🛠 Prototyping. First batch of Rev 1 boards ordered. Firmware runs on a dev board: four light modes, button, Standby, auto-off, battery compensation and low-battery cutoff. Nothing here is production-ready yet.

<!-- Add a photo here once you have one: ![Prototype](docs/images/prototype-v0.1.jpg) -->

## Using it

| Do this | What happens |
|---|---|
| Press the button | It fades in, in the mode it had last time (high if the battery has been disconnected) |
| Short press | Next mode: high → low → breathe → candle |
| Hold for 1.5 s | It fades out and switches off |
| Leave it on | It fades out and switches off by itself after 4 hours |
| Battery runs low | It fades out, pulses softly three times ("charge me") and switches off |
| Charge | USB-C |

The brightness stays the same while the battery drains, down to the 3.6 V cutoff.

## What's in here

| Folder | Contents |
|---|---|
| [`hardware/`](hardware/) | KiCad project, datasheet links, fabrication outputs (Gerbers, BOM, CPL). Each ordered revision is a [Release](../../releases), e.g. [`hw-v1`](../../releases/tag/hw-v1) |
| [`firmware/`](firmware/) | STM32G030 firmware. Runs on the dev board for now. How to build, test and lint: [`firmware/README.md`](firmware/README.md) |
| [`firmware/tests/`](firmware/tests/) | Host-side GoogleTest tests for the firmware logic, run in CI and by the pre-push gate |
| [`docs/`](docs/) | Design decisions, power budget, pinout, bring-up notes, images |
| `production/` | Programming and test process for each batch (not created yet) |
| [`CHANGELOG.md`](CHANGELOG.md) | What changed, per hardware and firmware version |

## Key parts (current choices)

| Function | Part | Notes |
|---|---|---|
| MCU | STM32G030F6P6 | Cortex-M0+, 64 MHz, 32 KB flash, 8 KB RAM, TSSOP-20 |
| LEDs | 20 × XL-1608WWC-06 | Warm white, 1608 metric (0603 imperial) |
| Charger | MCP73831 (SOT-23-5) | Linear single-cell Li-ion/LiPo charger, 4.20 V variant, 100 mA, USB-C input |
| Protection | DW01A + FS8205A | Over-discharge, over-charge and over-current protection on the board |
| Regulator | TPS7A0233 | 3.3 V LDO, ~25 nA quiescent current |
| Button | Alps SKRTLAE010 | Side-push SMD tact switch |
| Battery | Single-cell LiPo, 250 mAh planned | Cell size *TBD* (DD-002) |

Full design rationale: [`docs/design-decisions.md`](docs/design-decisions.md)

## Design goals

1. **Works next Christmas.** Every choice favours longevity: deep sleep, over-discharge protection, a battery that survives 11 months in a box.
2. **Calm light.** Gamma-corrected, high-resolution, flicker-free PWM. Nothing jumpy, nothing "disco".
3. **Safe to hang where children and pets are.** LiPo safety and robustness are requirements.
4. **Buildable by one or two people** in batches of about 150.

## Roadmap

- [x] Initial KiCad schematic
- [x] Dev board bring-up: PWM on all 7 channels, button, Standby and wake
- [x] Light modes: high, low, breathe, candle
- [x] Battery measurement, brightness compensation and low-battery cutoff (DD-011)
- [x] Fade in and out when switching on and off
- [x] PCB layout and fabrication files (Rev 1)
- [x] First batch of Rev 1 boards ordered
- [ ] Measure Standby current on the dev board
- [ ] Gamma-corrected brightness (smooth fades at the dim end)
- [ ] Stay off when a battery is first connected (saves ~100 mAh, see the power budget)
- [ ] VDD via VREFINT (battery reading stays accurate below ~3.4 V)
- [ ] First prototype boards (Rev 1) built and brought up
- [ ] Power budget measured against calculated
- [ ] Production test jig
- [ ] Compliance groundwork (CE, GPSR, Battery Regulation)

Open tasks are tracked in [Issues](../../issues).

## Licence

Gran Ornament is **source-available for non-commercial use**. You're welcome to build one for yourself, as a gift, or with a school or makerspace, and to share improvements.

**Selling it requires permission.** That covers selling assembled ornaments, kits, or boards made from these files. Get in touch at *henrik@smitt.dk* and we'll work something out.

| What | Licence |
|---|---|
| Hardware (`hardware/`), documentation and images (`docs/`) | [CC BY-NC-SA 4.0](LICENSE-CC-BY-NC-SA) |
| Firmware (`firmware/`) | [PolyForm Noncommercial 1.0.0](LICENSE) |

The name "Gran Ornament" and any logo are not covered by these licences. Please don't use them for your own builds.
