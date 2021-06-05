/**
 * @file bsp_clk.h
 */

#ifndef __BSP_CLK_H
#define __BSP_CLK_H

#ifdef __cplusplus
extern "C" {
#endif

#include "stm32l1xx_hal.h"
#include "os.h"

/**
 * @brief This funtion is a blocking delay, OSSched won't be called.
 * 
 * @param[in] period_ms: in millisecond
 */
void Delay_Blocking(uint32_t period_ms);

/**
 * @brief This function is a non-blocking delay which means other tasks can
 * run during the delay. OSSched() is called to schedule which task run next.
 * 
 * @param[in] period_ms: in millisecond
 */
void Delay_NonBlocking(uint32_t period_ms);

/**
 * @brief This function is called when errors occur
 */
void Error_Handler(void);

/**
 * @brief This function is used to configurate system clock
 */
void SystemClock_Config(void);



#ifdef __cplusplus
}
#endif

#endif /* __BSP_CLK_H */
