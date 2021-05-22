/**
 * @file bsp_clk.h
 */

#ifndef __BSP_CLK_H
#define __BSP_CLK_H

#ifdef __cplusplus
extern "C" {
#endif

#include "main.h"

/**
 * @brief This function is used to configurate system clock
 */
void SystemClock_Config(void);

/**
 * @brief This function is called when errors occur
 */
void Error_Handler(void);

#ifdef __cplusplus
}
#endif

#endif /* __BSP_CLK_H */
