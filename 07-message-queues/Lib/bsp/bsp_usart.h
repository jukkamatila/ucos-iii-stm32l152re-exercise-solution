/**
 * @file bsp_usart.h
 */

#ifndef __BSP_USART_H
#define __BSP_USART_H

#ifdef __cplusplus
extern "C" {
#endif

#include "main.h"

extern UART_HandleTypeDef huart2;

/**
 * @brief This function initilizes USART2. 
 * 
 * @note This function should be called before 
 * using the UART_Transmit() function
 */
void UART_Init(void);

/**
 * @brief This function print a string to UART
 * 
 * @param[in] p_data pointer to a string
 * @param[in] size size of the string
 */
void UART_Transmit(uint8_t *p_data, uint16_t size);

#define USART_TX_Pin GPIO_PIN_2
#define USART_TX_GPIO_Port GPIOA
#define USART_RX_Pin GPIO_PIN_3
#define USART_RX_GPIO_Port GPIOA


#ifdef __cplusplus
}
#endif

#endif /* __BSP_USART_H */
