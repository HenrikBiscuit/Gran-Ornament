# Power budget

All numbers in one place. "Calc" is from datasheets, "Meas" is from the bench.

## Assumptions

| Item | Value | Source |
|---|---|---|
| Battery capacity | 250 mAh (planned) | DD-002 |
| Usable capacity (4.2 V → cutoff) | ~85 % of rated | Typical LiPo, to be confirmed |
| LED current at full brightness | ~2.3 mA per LED at 4.1 V (470 Ω), scaled to its 3.6 V level by compensation | Bring-up 2026-09-29, DD-011 |
| MCU run current | *TBD* mA | DS12991, run mode tables |
| Regulator quiescent current | ~0.025 µA typ | TPS7A02 datasheet (to confirm) |
| Charger reverse leakage (no input) | ~0.15 µA typ, ~2 µA max | MCP73831 datasheet (to confirm) |
| Protection IC (DW01A) | ~3 µA typ, ~6 µA max | DW01A datasheet (to confirm) |
| VBAT divider (1M/1M) | ~2 µA | VBAT / 2 MΩ |
| Low-battery cutoff | 3.6 V | DD-011 |

## Run time per charge

| Pattern | Avg duty | Avg LED current | MCU + reg. | Total | Hours per charge | Meas |
|---|---|---|---|---|---|---|
| High | – | – | – | – | – | – |
| Low | – | – | – | – | – | – |
| Breathe | – | – | – | – | – | – |
| Candle | – | – | – | – | – | – |

Hours per charge = usable capacity (mAh) ÷ total average current (mA)

## Storage (11 months in a box)

| Source | Current | Over 12 months |
|---|---|---|
Calc, typical (worst case in brackets). Every 1 µA drawn all the time costs 8.76 mAh a year.

| Source | Current | Over 12 months |
|---|---|---|
| MCU in Standby | ~0.3 µA (~1 µA) | ~3 mAh (~9 mAh) |
| Regulator quiescent | ~0.025 µA | ~0.2 mAh |
| Charger reverse leakage | ~0.15 µA (~2 µA) | ~1 mAh (~18 mAh) |
| Protection IC (DW01A) | ~3 µA (~6 µA) | ~26 mAh (~53 mAh) |
| VBAT divider | ~2 µA | ~18 mAh |
| LiPo self-discharge | ~2–3 %/month (typical, confirm with cell supplier) | ~60–90 mAh of 250 mAh |
| **Total** | ~5.5 µA (~11 µA) + self-discharge | **~110–140 mAh (~180 mAh)** |

At the 3.6 V cutoff roughly 25–50 mAh (10–20 %) is left, less than a year of storage needs. A 3.8 V cutoff
(~50 %) would cover the typical case. Biggest levers: a lower-current protection IC, a switched divider,
and confirming the cell's real self-discharge (some datasheets already include the protection IC in it).

**Watch out:** connecting the battery boots the ornament into its mode, and it stays on until a long press or
the 4 h auto-off. That first run costs up to ~100 mAh. Not handled yet.

Target: a unit stored at ~50–60 % charge in January still turns on next December.

## How to measure

- Run current: multimeter in series with the battery, average over a full pattern cycle.
- Sleep current: µA range, or a tool like a Nordic PPK2 / µCurrent. Disconnect the debugger first.
