/**
 * @file bsp_max4466
 */

#ifndef __BSP_MAX4466_H
#define __BSP_MAX4466_H

#ifdef __cplusplus
extern "C"
{
#endif

#include "main.h"

#define ADC_BUFFER_SIZE 4096

extern ADC_HandleTypeDef hadc;

double MAX4466_Read();
void MAX4466_Init();
void MAX4466_Print(UART_HandleTypeDef *uart);

void MX_ADC_Init(void);
void MX_DMA_Init(void);


#ifdef __cplusplus
}
#endif

#endif /* __BSP_MAX4466_H */
