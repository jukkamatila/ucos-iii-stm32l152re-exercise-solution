#include "bsp_led.h"

void LED_Init(GPIO_TypeDef *led_port, uint16_t led_pin, GPIO_PinState led_state)
{
    GPIO_InitTypeDef GPIO_InitStruct = {0};

    if (led_port == GPIOA)
        __HAL_RCC_GPIOA_CLK_ENABLE();
    if (led_port == GPIOB)
        __HAL_RCC_GPIOB_CLK_ENABLE();
    if (led_port == GPIOC)
        __HAL_RCC_GPIOC_CLK_ENABLE();

    HAL_GPIO_WritePin(led_port, led_pin, led_state);
    GPIO_InitStruct.Pin = led_pin;
    GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
    HAL_GPIO_Init(led_port, &GPIO_InitStruct);
}

void LED_Off(GPIO_TypeDef *led_port, uint16_t led_pin)
{
    HAL_GPIO_WritePin(led_port, led_pin, GPIO_PIN_RESET);
}

void LED_On(GPIO_TypeDef *led_port, uint16_t led_pin)
{
    HAL_GPIO_WritePin(led_port, led_pin, GPIO_PIN_SET);
}

void LED_Toggle(GPIO_TypeDef *led_port, uint16_t led_pin)
{
    HAL_GPIO_TogglePin(led_port, led_pin);
}
