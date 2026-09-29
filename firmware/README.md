# Firmware

Target: **STM32G030F6P6** (Cortex-M0+, 32 KB flash, 8 KB RAM).
Toolchain: VS code (GCC). HAL/LL mix, kept small and readable.

## Layout

| Path | What it is |
|---|---|
| `devboard/` | VS code project for the dev board. Experiments, bring-up tests, pattern tuning. |
| `ornament/` | VS Code project for the real ornament board (created once rev A exists). |
| `tests/` | Host-side GoogleTest tests for the hardware-independent code in `devboard/Gran_Ornament/App/Inc`. |

## Checks

Every push is checked the same way locally and in CI
([`.github/workflows/firmware.yml`](../.github/workflows/firmware.yml)):

1. **Build** the dev-board firmware (`cmake --preset Release`).
2. **Lint** with [`tools/lint.ps1`](../tools/lint.ps1):
   - `clang-format` checks the layout of our own code (`devboard/*/App/**`, `tests/*.cpp`)
     against [`.clang-format`](../.clang-format).
   - `clang-tidy` runs the bug-finding checks in [`.clang-tidy`](../.clang-tidy) on the
     firmware `App/` sources. It uses the firmware's `compile_commands.json` and the ARM GCC
     headers, so it needs the configure step above.
   - CubeMX-generated `Core/` and vendor `Drivers/` are never formatted or linted, so
     regenerating from the `.ioc` stays clean.
3. **Test**: build and run the host tests with CTest.

Run all three locally with the pre-push gate:

```powershell
powershell -NoProfile -ExecutionPolicy Bypass -File tools/pre-push.ps1
```

To run it automatically on every `git push`, enable the repo's hooks once per clone:

```bash
git config core.hooksPath .githooks
```

To fix formatting, run `tools/lint.ps1 -Fix`, or turn on format-on-save in an editor that
uses clangd. Requires `clang-format` (LLVM 23, same major version as CI), `clang-tidy`, and
`arm-none-eabi-gcc` on `PATH`.

