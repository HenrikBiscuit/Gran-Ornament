#include "power.hpp"

#include "main.h"

namespace gran::power {

void init()
{
    // Flags survive the wake-up reset. A stale WUF would make the next
    // Standby entry wake up straight away.
    PWR->SCR = PWR_SCR_CSBF | PWR_SCR_CWUF;

    // Standby pull config stays active after wake-up until cleared. GPIO init
    // already gives PA0 its pull-up, so hand control back to the GPIO block.
    HAL_PWREx_DisablePullUpPullDownConfig();

    // TAMP (and its backup registers) is clocked from RTCAPB, and writes need
    // backup-domain access.
    __HAL_RCC_RTCAPB_CLK_ENABLE();
    HAL_PWR_EnableBkUpAccess();
}

std::uint32_t read_backup()
{
    return TAMP->BKP0R;
}

void write_backup(std::uint32_t value)
{
    TAMP->BKP0R = value;
}

void enter_standby()
{
    // Without this PA0 floats in Standby: the switch is normally open and the
    // GPIO pull-up is gone, so noise could wake the ornament.
    HAL_PWREx_EnableGPIOPullUp(PWR_GPIO_A, PWR_GPIO_BIT_0);
    HAL_PWREx_EnablePullUpPullDownConfig();

    // Set the polarity before clearing the flag: changing it can set WUF.
    HAL_PWR_DisableWakeUpPin(PWR_WAKEUP_PIN1);
    HAL_PWR_EnableWakeUpPin(PWR_WAKEUP_PIN1_LOW);
    PWR->SCR = PWR_SCR_CWUF;

    HAL_PWR_EnterSTANDBYMode();

    // Not reached: waking from Standby is a reset.
    while (true) {
    }
}

} // namespace gran::power
