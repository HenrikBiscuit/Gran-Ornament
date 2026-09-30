#pragma once
#include <cstdint>

namespace gran::power {

// Call once at boot, after the CubeMX init. Clears the Standby/wake-up flags,
// drops the Standby pull config, and unlocks the backup registers.
void init();

// TAMP backup register 0. Kept through Standby and NRST, lost on power loss.
std::uint32_t read_backup();
void write_backup(std::uint32_t value);

// Enter Standby. Wakes on a falling edge on PA0 (WKUP1, button to GND),
// which restarts the firmware from reset. The caller turns the LEDs off and
// waits for the button to be released first. In Standby every GPIO is Hi-Z,
// so the open-drain LED pins are released (off) and only PA0 keeps its pull-up.
[[noreturn]] void enter_standby();

} // namespace gran::power
