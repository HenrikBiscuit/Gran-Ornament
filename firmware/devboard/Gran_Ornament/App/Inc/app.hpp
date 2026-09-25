#pragma once

#ifdef __cplusplus
extern "C" 
{
#endif

/* Entry point for the application. Called once from main.c after
 * HAL_Init(), clock config and CubeMX peripheral init. Never returns. */
void app_run(void) __attribute__((noreturn));

#ifdef __cplusplus
}
#endif