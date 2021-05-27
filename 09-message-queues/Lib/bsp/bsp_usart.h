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
 * @brief This function initializes USART2
 */
void MX_USART2_UART_Init(void);

#define USART_TX_Pin GPIO_PIN_2
#define USART_TX_GPIO_Port GPIOA
#define USART_RX_Pin GPIO_PIN_3
#define USART_RX_GPIO_Port GPIOA


#ifdef __cplusplus
}
#endif

#endif /* __BSP_USART_H */
