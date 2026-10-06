# Fabrication outputs

One folder per board revision, exported from KiCad for JLCPCB, for example:

```
fabrication/
└── Rev1/
    ├── PCB-*.gbr, PCB-*.drl      Gerber and drill files
    ├── PCB-job.gbrjob            Gerber job file
    ├── PCB-BOM-JLC.csv           Bill of materials in JLC's format (LCSC part numbers)
    ├── PCB-CPL-JLC.csv           Component placement (pick-and-place) in JLC's format
    ├── PCB-top.pos, -bottom.pos  KiCad placement files
    └── Rev1.zip                  Everything above, as uploaded to JLC
```

The same zip is attached to the matching GitHub Release (`hw-v1`).
