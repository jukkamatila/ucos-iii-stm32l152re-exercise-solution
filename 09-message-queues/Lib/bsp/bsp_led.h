/**
 * @file bsp_led.h
 */

#ifndef __BSP_LED_H
#define __BSP_LED_H

#ifdef __cplusplus
extern "C"
{
#endif

#include "stm32l1xx_hal.h"

/**
 * @brief This function is used to initialize GPIO pins that used for LEDs.
 * 
 * @note To make sure LED_On() and LED_Off() work correctly LED (+) pins should
 * be connected to GPIO pins and LED (-) pins should be connected to GND.
 * 
 * @param[in] led_port: GPIO port
 * @param[in] led_pin: GPIO pin
 * @param[in] led_state: GPIO state (SET/RESET)
 */
void LED_Init(GPIO_TypeDef *led_port, uint16_t led_pin, GPIO_PinState led_state);

/**
 * @brief This function is used to turn LED off
 * 
 * @param[in] led_port: GPIO port
 * @param[in] led_pin: GPIO pin
 */
void LED_Off(GPIO_TypeDef *led_port, uint16_t led_pin);

/**
 * @brief This function is used to turn LED on
 * 
 * @param[in] led_port: GPIO port
 * @param[in] led_pin: GPIO pin
 */
void LED_On(GPIO_TypeDef *led_port, uint16_t led_pin);

/**
 * @brief This function is used to toggle LED 
 * 
 * @param[in] led_port: GPIO port
 * @param[in] led_pin: GPIO pin
 */
void LED_Toggle(GPIO_TypeDef *led_port, uint16_t led_pin);

#ifdef __cplusplus
}
#endif

#endif /* __BSP_LED_H */
