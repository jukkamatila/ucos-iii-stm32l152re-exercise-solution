/**
 * @file bsp_ssd1306_cfg.c
 * @brief contains functions that should be modified when porting to differrent
 * target devices
 */

#include "bsp_ssd1306_cfg.h"

int8_t SSD1306_WriteCommand(uint8_t cmd)
{
    int8_t err;
    HAL_GPIO_WritePin(SSD1306_CS_PORT, SSD1306_CS_PIN, GPIO_PIN_RESET);
    HAL_GPIO_WritePin(SSD1306_DC_PORT, SSD1306_DC_PIN, GPIO_PIN_RESET);
    err = HAL_SPI_Transmit(SSD1306_SPI_HANDLER, &cmd, 1, HAL_MAX_DELAY);
    HAL_GPIO_WritePin(SSD1306_CS_PORT, SSD1306_CS_PIN, GPIO_PIN_SET);
    if (err == HAL_OK)
        return SSD1306_OK;
    else
        return SSD1306_WRITECMD_FAILED;
}

int8_t SSD1306_WriteData(uint8_t *data, uint8_t length)
{
    int8_t err;
    HAL_GPIO_WritePin(SSD1306_CS_PORT, SSD1306_CS_PIN, GPIO_PIN_RESET);
    HAL_GPIO_WritePin(SSD1306_DC_PORT, SSD1306_DC_PIN, GPIO_PIN_SET);
    err = HAL_SPI_Transmit(SSD1306_SPI_HANDLER, data, length, HAL_MAX_DELAY);
    HAL_GPIO_WritePin(SSD1306_CS_PORT, SSD1306_CS_PIN, GPIO_PIN_SET);
    if (err == HAL_OK)
        return SSD1306_OK;
    else
        return SSD1306_WRITEDATA_FAILED;
}

void SSD1306_Delay(uint32_t ms) { Delay_Blocking(ms); }

void SSD1306_CSPinOff(void)
{
    HAL_GPIO_WritePin(SSD1306_CS_PORT, SSD1306_CS_PIN, GPIO_PIN_RESET);
}

void SSD1306_CSPinOn(void)
{
    HAL_GPIO_WritePin(SSD1306_DC_PORT, SSD1306_DC_PIN, GPIO_PIN_SET);
}

void SSD1306_DCPinOff(void)
{
    HAL_GPIO_WritePin(SSD1306_DC_PORT, SSD1306_DC_PIN, GPIO_PIN_RESET);
}

void SSD1306_DCPinOn(void)
{
    HAL_GPIO_WritePin(SSD1306_DC_PORT, SSD1306_DC_PIN, GPIO_PIN_SET);
}

void SSD1306_ResetPinOff(void)
{
    HAL_GPIO_WritePin(SSD1306_RESET_PORT, SSD1306_RESET_PIN, GPIO_PIN_RESET);
}
void SSD1306_ResetPinOn(void)
{
    HAL_GPIO_WritePin(SSD1306_RESET_PORT, SSD1306_RESET_PIN, GPIO_PIN_SET);
}

void MX_SPI2_Init(void)
{
  hspi2.Instance = SPI2;
  hspi2.Init.Mode = SPI_MODE_MASTER;
  hspi2.Init.Direction = SPI_DIRECTION_2LINES;
  hspi2.Init.DataSize = SPI_DATASIZE_8BIT;
  hspi2.Init.CLKPolarity = SPI_POLARITY_HIGH;
  hspi2.Init.CLKPhase = SPI_PHASE_2EDGE;
  hspi2.Init.NSS = SPI_NSS_SOFT;
  hspi2.Init.BaudRatePrescaler = SPI_BAUDRATEPRESCALER_4;
  hspi2.Init.FirstBit = SPI_FIRSTBIT_MSB;
  hspi2.Init.TIMode = SPI_TIMODE_DISABLE;
  hspi2.Init.CRCCalculation = SPI_CRCCALCULATION_DISABLE;
  hspi2.Init.CRCPolynomial = 10;
  if (HAL_SPI_Init(&hspi2) != HAL_OK)
  {
    Error_Handler();
  }
}

void HAL_SPI_MspInit(SPI_HandleTypeDef* spiHandle)
{

  GPIO_InitTypeDef GPIO_InitStruct = {0};
  if(spiHandle->Instance==SPI2)
  {
    /* SPI2 clock enable */
    __HAL_RCC_SPI2_CLK_ENABLE();

    __HAL_RCC_GPIOB_CLK_ENABLE();
    /**SPI2 GPIO Configuration
    PB13     ------> SPI2_SCK
    PB14     ------> SPI2_MISO
    PB15     ------> SPI2_MOSI
    */
    GPIO_InitStruct.Pin = GPIO_PIN_13|GPIO_PIN_14|GPIO_PIN_15;
    GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
    GPIO_InitStruct.Alternate = GPIO_AF5_SPI2;
    HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);
  }
}

void HAL_SPI_MspDeInit(SPI_HandleTypeDef* spiHandle)
{

  if(spiHandle->Instance==SPI2)
  {
    /* Peripheral clock disable */
    __HAL_RCC_SPI2_CLK_DISABLE();

    /**SPI2 GPIO Configuration
    PB13     ------> SPI2_SCK
    PB14     ------> SPI2_MISO
    PB15     ------> SPI2_MOSI
    */
    HAL_GPIO_DeInit(GPIOB, GPIO_PIN_13|GPIO_PIN_14|GPIO_PIN_15);

  }
}

void SSD1306_CtrlPinInit(void)
{

  GPIO_InitTypeDef GPIO_InitStruct = {0};

  /* GPIO Ports Clock Enable */
  __HAL_RCC_GPIOA_CLK_ENABLE();

  HAL_GPIO_WritePin(GPIOA, SSD1306_DC_PIN|SSD1306_CS_PIN, GPIO_PIN_RESET);
  HAL_GPIO_WritePin(SSD1306_RESET_PORT, SSD1306_RESET_PIN, GPIO_PIN_SET);

  GPIO_InitStruct.Pin = SSD1306_DC_PIN|SSD1306_RESET_PIN|SSD1306_CS_PIN;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);
}

void SSD1306_Init(void)
{
    SSD1306_CtrlPinInit();
    MX_SPI2_Init();
    SSD1306_Delay(100); /* Wait until Vcc is statble */
    SSD1306_Reset();

    SSD1306_SetSegRemap(SSD1306_SEG_REMAP_INVERSE);
    SSD1306_SetChargeBumpSetting(SSD1306_CHARGE_BUMP_ENABLE);
    SSD1306_DispOnOff(SSD1306_DISP_ON);
    SSD1306_Delay(100);
    SSD1306_FillBuffer(SSD1306_PIXEL_OFF);
    SSD1306_UpdateDisp();
}
