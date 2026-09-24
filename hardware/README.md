# Hardware

## Layout

| Path | What goes here |
|---|---|
| `kicad/` | The KiCad project (`.kicad_pro`, `.kicad_sch`, `.kicad_pcb`) plus project-local symbol and footprint libraries |
| `kicad/lib/` | Custom symbols (`.kicad_sym`) and footprints (`.pretty/`) used by this project only |
| `fabrication/<rev>/` | Exported Gerbers, drill files, BOM and CPL for each board revision sent to the assembler |
| `datasheets/` | Links to the datasheets of every part on the board |

## Revisions

| Rev | Date | Status | Notes |
|---|---|---|---|
| A | – | Schematic in progress | First design |

Each revision that is sent to fabrication gets a Git tag (`hw-vA`, `hw-vB`, ...) and a
GitHub Release with the fabrication zip attached, so it is always possible to see exactly
what was built.

## Viewing without KiCad

A PDF export of the schematic lives in `docs/schematic-revX.pdf` (File → Plot in the
schematic editor). Re-export it whenever the schematic changes meaningfully, so visitors
can follow along without installing KiCad.

## Before a design leaves the house

- [ ] ERC clean (or every remaining warning explained in a comment)
- [ ] DRC clean against the assembler's design rules
- [ ] Every part has an LCSC/manufacturer part number field
- [ ] BOM and CPL exported and spot-checked for rotation
- [ ] Title block: revision and date updated, no private address
- [ ] Schematic PDF exported to `docs/`
- [ ] Tagged `hw-vX` and fabrication zip attached to a Release
