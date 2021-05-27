/**
 * @file bsp_max4466.h
 */

#ifndef __BSP_MAX4466_H
#define __BSP_MAX4466_H

#ifdef __cplusplus
extern "C"
{
#endif

#include <stdio.h>
#include <stdlib.h>
#include "stm32l1xx_hal.h"
#include "bsp_clk.h"

/**
 * @brief size of the buffer used by ADC 
 */
#define ADC_BUFFER_SIZE 4096

extern ADC_HandleTypeDef hadc;

/**
 * @brief This function reads analog signal and convert to voltage
 * 
 * @return adc_value [0-255] (0V-3.3V)
 */
uint8_t MAX4466_Read();

/**
 * @brief This function initializes ADC and DMA 
 * to convert MAX4466 ouput to voltage
 */
void MAX4466_Init();

/**
 * @brief This function print calculated voltage to USART
 * 
 * @param uart a pointer to UART handler
*/
void MAX4466_Print(UART_HandleTypeDef *uart);

/**
 * @brief This function initialize ADC
*/
void MX_ADC_Init(void);

/**
 * @brief This function initialize DMA to use with ADC
*/
void MX_DMA_Init(void);


#ifdef __cplusplus
}
#endif

#endif /* __BSP_MAX4466_H */
