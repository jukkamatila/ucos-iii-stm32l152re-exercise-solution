/**
 * @file bsp_led.c
 */

#include <bsp_led.h>

void LED_Init()
{
    GPIO_InitTypeDef GPIO_InitStruct = {0};

    if (LED_PORT == GPIOA)
        __HAL_RCC_GPIOA_CLK_ENABLE();
    if (LED_PORT == GPIOB)
        __HAL_RCC_GPIOB_CLK_ENABLE();
    if (LED_PORT == GPIOC)
        __HAL_RCC_GPIOC_CLK_ENABLE();

    HAL_GPIO_WritePin(LED_PORT, LED_PIN, GPIO_PIN_RESET);
    GPIO_InitStruct.Pin = LED_PIN;
    GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
    HAL_GPIO_Init(LED_PORT, &GPIO_InitStruct);
}

void LED_Off()
{
    HAL_GPIO_WritePin(LED_PORT, LED_PIN, GPIO_PIN_RESET);
}

void LED_On()
{
    HAL_GPIO_WritePin(LED_PORT, LED_PIN, GPIO_PIN_SET);
}

void LED_Toggle()
{
    HAL_GPIO_TogglePin(LED_PORT, LED_PIN);
}
