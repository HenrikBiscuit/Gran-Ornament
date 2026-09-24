# Gran Ornament

A small, rechargeable, two-sided LED Christmas ornament. Twenty warm-white LEDs, split across two independently controlled sides, play slow and calm light patterns: breathing, soft twinkles, a candle catching a draught. One button turns it on, it switches itself off after four hours, and it sleeps quietly between Christmases.

Made by hand in small batches in Denmark. This repository is the open build log: hardware, firmware, and the notes behind the decisions.

> **Status:** 🛠 Early prototyping. Schematic in progress, firmware being brought up on a dev board. Nothing here is production-ready yet.

<!-- Add a photo here once you have one: ![Prototype](docs/images/prototype-v0.1.jpg) -->

## What's in here

| Folder | Contents |
|---|---|
| [`hardware/`](hardware/) | KiCad project, datasheet links, fabrication outputs (Gerbers, BOM, CPL) |
| [`firmware/`](firmware/) | STM32G030 firmware: dev-board experiments and the ornament firmware |
| [`docs/`](docs/) | Design decisions, power budget, pinout, bring-up notes, images |
| [`production/`](production/) | Programming and test process for each batch (later) |
| [`CHANGELOG.md`](CHANGELOG.md) | What changed, per hardware and firmware version |

## Key parts (current choices)

| Function | Part | Notes |
|---|---|---|
| MCU | STM32G030F6P6 | Cortex-M0+, 64 MHz, 32 KB flash, 8 KB RAM, TSSOP-20 |
| LEDs | 20 × XL-1608WWC-06 | Warm white, 1608 metric (0603 imperial) |
| Charger | MCP73831 (SOT-23-5) | Linear single-cell Li-ion/LiPo charger, 4.20 V variant |
| Button | Alps SKRTLAE010 | Side-push SMD tact switch |
| Battery | Single-cell LiPo, *capacity TBD* | With protection circuit |

Full design rationale: [`docs/design-decisions.md`](docs/design-decisions.md)

## Design goals

1. **Works next Christmas.** Every choice favours longevity: deep sleep, over-discharge protection, a battery that survives 11 months in a box.
2. **Calm light.** Gamma-corrected, high-resolution, flicker-free PWM. Nothing jumpy, nothing "disco".
3. **Safe to hang where children and pets are.** LiPo safety and robustness are requirements, not extras.
4. **Buildable by one or two people** in batches of about 150.

## Roadmap

- [x] Initial KiCad schematic
- [ ] Dev board bring-up: blink, PWM fade, button wake, Stop/Standby current
- [ ] First light patterns on the dev board
- [ ] Battery measurement via VREFINT
- [ ] Schematic review and PCB layout (rev A)
- [ ] First prototype boards
- [ ] Power budget measured against calculated
- [ ] Production test jig
- [ ] Compliance groundwork (CE, GPSR, Battery Regulation)

Open tasks are tracked in [Issues](../../issues).

## Licence

Gran Ornament is **source-available for non-commercial use**. You're welcome to build one for yourself, as a gift, or with a school or makerspace, and to share improvements.

**Selling it requires permission.** That covers selling assembled ornaments, kits, or boards made from these files. Get in touch at *your@email.dk* and we'll work something out.

| What | Licence |
|---|---|
| Hardware (`hardware/`), documentation and images (`docs/`) | [CC BY-NC-SA 4.0](LICENSE-CC-BY-NC-SA) |
| Firmware (`firmware/`) | [PolyForm Noncommercial 1.0.0](LICENSE) |

The name "Gran Ornament" and any logo are not covered by these licences. Please don't use them for your own builds.
