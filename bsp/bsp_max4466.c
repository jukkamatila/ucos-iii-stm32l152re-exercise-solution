#include "bsp_max4466.h"

ADC_HandleTypeDef hadc;
DMA_HandleTypeDef hdma_adc;
uint8_t adc_buff[ADC_BUFFER_SIZE];
uint8_t adc_value = 0;
uint8_t loudness = 0;

void MAX4466_Init()
{
    MX_DMA_Init();
    MX_ADC_Init();
}

uint8_t MAX4466_Read()
{
    HAL_ADC_Start_DMA(&hadc, (uint32_t *)adc_buff, ADC_BUFFER_SIZE);
    return loudness;
}

void MAX4466_Print(UART_HandleTypeDef *uart)
{
    uint8_t msg[15];
    sprintf((char *)msg, "\rADC = %d\n\r", adc_value);
    HAL_UART_Transmit(uart, msg, sizeof(msg), 100);
}

void MX_ADC_Init(void)
{
  ADC_ChannelConfTypeDef sConfig = {0};
  hadc.Instance = ADC1;
  hadc.Init.ClockPrescaler = ADC_CLOCK_ASYNC_DIV4;
  hadc.Init.Resolution = ADC_RESOLUTION_8B;
  hadc.Init.DataAlign = ADC_DATAALIGN_RIGHT;
  hadc.Init.ScanConvMode = ADC_SCAN_DISABLE;
  hadc.Init.EOCSelection = ADC_EOC_SEQ_CONV;
  hadc.Init.LowPowerAutoWait = ADC_AUTOWAIT_DISABLE;
  hadc.Init.LowPowerAutoPowerOff = ADC_AUTOPOWEROFF_DISABLE;
  hadc.Init.ChannelsBank = ADC_CHANNELS_BANK_A;
  hadc.Init.ContinuousConvMode = ENABLE;
  hadc.Init.NbrOfConversion = 1;
  hadc.Init.DiscontinuousConvMode = DISABLE;
  hadc.Init.ExternalTrigConv = ADC_SOFTWARE_START;
  hadc.Init.ExternalTrigConvEdge = ADC_EXTERNALTRIGCONVEDGE_NONE;
  hadc.Init.DMAContinuousRequests = DISABLE;
  if (HAL_ADC_Init(&hadc) != HAL_OK)
  {
    Error_Handler();
  }
  sConfig.Channel = ADC_CHANNEL_0;
  sConfig.Rank = ADC_REGULAR_RANK_1;
  sConfig.SamplingTime = ADC_SAMPLETIME_16CYCLES;
  if (HAL_ADC_ConfigChannel(&hadc, &sConfig) != HAL_OK)
  {
    Error_Handler();
  }
}

void HAL_ADC_MspInit(ADC_HandleTypeDef *adcHandle)
{

  GPIO_InitTypeDef GPIO_InitStruct = {0};
  if (adcHandle->Instance == ADC1)
  {
    /* ADC1 clock enable */
    __HAL_RCC_ADC1_CLK_ENABLE();

    __HAL_RCC_GPIOA_CLK_ENABLE();

    GPIO_InitStruct.Pin = GPIO_PIN_0;
    GPIO_InitStruct.Mode = GPIO_MODE_ANALOG;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

    /* ADC1 DMA Init */
    /* ADC Init */
    hdma_adc.Instance = DMA1_Channel1;
    hdma_adc.Init.Direction = DMA_PERIPH_TO_MEMORY;
    hdma_adc.Init.PeriphInc = DMA_PINC_DISABLE;
    hdma_adc.Init.MemInc = DMA_MINC_ENABLE;
    hdma_adc.Init.PeriphDataAlignment = DMA_PDATAALIGN_BYTE;
    hdma_adc.Init.MemDataAlignment = DMA_MDATAALIGN_BYTE;
    hdma_adc.Init.Mode = DMA_NORMAL;
    hdma_adc.Init.Priority = DMA_PRIORITY_LOW;
    if (HAL_DMA_Init(&hdma_adc) != HAL_OK)
    {
      Error_Handler();
    }

    __HAL_LINKDMA(adcHandle, DMA_Handle, hdma_adc);
  }
}

void HAL_ADC_MspDeInit(ADC_HandleTypeDef *adcHandle)
{

  if (adcHandle->Instance == ADC1)
  {
    /* Peripheral clock disable */
    __HAL_RCC_ADC1_CLK_DISABLE();

    /**ADC GPIO Configuration
    PB15     ------> ADC_IN21
    */
    HAL_GPIO_DeInit(GPIOA, GPIO_PIN_0);

    /* ADC1 DMA DeInit */
    HAL_DMA_DeInit(adcHandle->DMA_Handle);
  }
}

void MX_DMA_Init(void)
{

  /* DMA controller clock enable */
  __HAL_RCC_DMA1_CLK_ENABLE();

  /* DMA interrupt init */
  /* DMA1_Channel1_IRQn interrupt configuration */
  HAL_NVIC_SetPriority(DMA1_Channel1_IRQn, 0, 0);
  HAL_NVIC_EnableIRQ(DMA1_Channel1_IRQn);

}

void DMA1_Channel1_IRQHandler(void)
{
    HAL_DMA_IRQHandler(&hdma_adc);
    HAL_ADC_Stop_DMA(&hadc);
    uint8_t max = 0;
    uint8_t min = 255; 
    for(int i = 0; i < ADC_BUFFER_SIZE; i++)
    {
        if(adc_buff[i] > max)
        {
        max = adc_buff[i];
        }
        else if(adc_buff[i] < min)
        {
        min = adc_buff[i];
        }
    }
    adc_value = (max-min);

    /* MAX4466 Output = 1/2 input voltage when slient 
     * Only take value > 1/2 input voltage = 255/2 = 127 */
    if(adc_value > 127)
    {
      loudness = adc_value - 127;
    }
    else
    {
      loudness = 0;
    }
}