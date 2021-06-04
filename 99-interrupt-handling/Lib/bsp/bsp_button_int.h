/**
 * @file bsp_button_int.h
 */

#ifndef __BSP_BUTTON_INT_H
#define __BSP_BUTTON_INT_H

#ifdef __cplusplus
extern "C" {
#endif

#include "main.h"

/**
 * @brief This function initialize button and EXTI interrupt
 */
void Button_Init(void);

/**
  * @brief This function handles button interrupt
  */
void EXTI15_10_IRQHandler(void);

#ifdef __cplusplus
}
#endif

#endif /* __BSP_BUTTON_INT_H */
