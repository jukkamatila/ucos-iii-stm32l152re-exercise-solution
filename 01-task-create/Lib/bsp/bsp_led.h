#ifndef __BSP_LED_H
#define __BSP_LED_H

#ifdef __cplusplus
extern "C"
{
#endif

#include "stm32l1xx_hal.h"

#define LED_PORT    GPIOA
#define LED_PIN     GPIO_PIN_5

/**
 * @brief This function is used to initialize GPIO pins that used for LEDs.
 * 
 * @note To make sure LED_On() and LED_Off() work correctly LED (+) pins should
 * be connected to GPIO pins and LED (-) pins should be connected to GND.
 */
void LED_Init();

/**
 * @brief This function is used to turn LED off
 * 
 */
void LED_Off();

/**
 * @brief This function is used to turn LED on
 * 
 */
void LED_On();

/**
 * @brief This function is used to toggle LED 
 */
void LED_Toggle();

#ifdef __cplusplus
}
#endif

#endif /* __BSP_LED_H */
