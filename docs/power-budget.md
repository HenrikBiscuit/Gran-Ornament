# Power budget

All numbers in one place. "Calc" is from datasheets, "Meas" is from the bench.

## Assumptions

| Item | Value | Source |
|---|---|---|
| Battery capacity | *TBD* mAh | – |
| Usable capacity (4.2 V → cutoff) | ~85 % of rated | Typical LiPo, to be confirmed |
| LED current at full brightness | *TBD* mA per LED | LED datasheet / resistor choice |
| MCU run current | *TBD* mA | DS12991, run mode tables |
| Regulator quiescent current | *TBD* µA | Regulator datasheet |
| Charger reverse leakage (no input) | *TBD* µA | MCP73831 datasheet |

## Run time per charge

| Pattern | Avg duty | Avg LED current | MCU + reg. | Total | Hours per charge | Meas |
|---|---|---|---|---|---|---|
| Breathing | – | – | – | – | – | – |
| Twinkle | – | – | – | – | – | – |
| Candle draught | – | – | – | – | – | – |

Hours per charge = usable capacity (mAh) ÷ total average current (mA)

## Storage (11 months in a box)

| Source | Current | Over 12 months |
|---|---|---|
| MCU in Stop/Standby | – µA | – mAh |
| Regulator quiescent | – µA | – mAh |
| Charger + protection leakage | – µA | – mAh |
| LiPo self-discharge | ~2–3 %/month (typical, confirm with cell supplier) | – mAh |
| **Total** | | – mAh |

Target: a unit stored at ~50–60 % charge in January still turns on next December.

## How to measure

- Run current: multimeter in series with the battery, average over a full pattern cycle.
- Sleep current: µA range, or a tool like a Nordic PPK2 / µCurrent. Disconnect the debugger first.
