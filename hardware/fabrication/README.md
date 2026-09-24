# Fabrication outputs

One folder per board revision, for example:

```
fabrication/
└── revA/
    ├── gerbers/           Gerber + drill files
    ├── bom-revA.csv       Bill of materials in the assembler's format
    ├── cpl-revA.csv       Component placement (pick-and-place)
    └── notes.md           Assembler, order date, quantity, anything special
```

The same files are zipped and attached to the matching GitHub Release (`hw-vA`).
