/*!
 * @file bsp_bmp280_cfg.c
 */

#include <bsp_bmp280_cfg.h>

/*! @brief is used in BMP280_Setup() function when BMP280_SPI is defined */
SPI_HandleTypeDef hspi1;

/*! @brief is used in BMP280_Setup() function when BMP280_I2C is defined */
I2C_HandleTypeDef hi2c1;

/*!  @brief store all info of BMP280 (device addr, chip id, calib parameters, function pointers, etc.) */
struct bmp280_dev bmp;

/*!  @brief store bmp280 configuration */
struct bmp280_config conf;

/*!  @brief store raw (uncomputed) data */
struct bmp280_uncomp_data ucomp_data;

/*!  @brief store 32bit temperature */
int32_t temp32;

/*!  @brief store floating point temperature */
double temp;

/*!  @brief store 32bit pressure */
uint32_t pres32;

/*!  @brief store floating point pressure */
double pres;

/*!
 *  @brief Function that reads and computes data (temp and pressure)
 *
 *  @return void.
 *
 */
void BMP280_Read(void)
{
    bmp280_get_uncomp_data(&ucomp_data, &bmp);

    bmp280_get_comp_temp_32bit(&temp32, ucomp_data.uncomp_temp, &bmp);
    bmp280_get_comp_pres_32bit(&pres32, ucomp_data.uncomp_press, &bmp);

    bmp280_get_comp_temp_double(&temp, ucomp_data.uncomp_temp, &bmp);
    bmp280_get_comp_pres_double(&pres, ucomp_data.uncomp_press, &bmp);
}

/*!
 *  @brief Function that initializes and configurates BMP280.
 *			Call BMP280_Setup() before BMP280_Read()
 *
 *  @return void.
 *
 */
void BMP280_Setup(void)
{
    /** Assign address. 
     * Use BMP280_I2C_ADDR_PRIM when SD0 connected to GND
     * Otherwise, use BMP280_I2C_ADDR_SEC
    */
    bmp.dev_id = BMP280_I2C_ADDR_PRIM;

    /** Assign delay function
     * Use Delay_NonBlocking() or Delay_Blocking from bsp_clk lib
    */
    bmp.delay_ms = Delay_NonBlocking;

#ifdef BMP280_SPI
    MX_SPI1_Init();
    bmp.read = SPI_Reg_Read;
    bmp.write = SPI_Reg_Write;
    bmp.intf = BMP280_SPI_INTF;
#endif
#ifdef BMP280_I2C
    MX_I2C1_Init();
    bmp.read = I2C_Reg_Read;
    bmp.write = I2C_Reg_Write;
    bmp.intf = BMP280_I2C_INTF;
#endif

    conf.filter = BMP280_FILTER_COEFF_2;
    conf.os_temp = BMP280_OS_4X;
    conf.os_pres = BMP280_OS_NONE;
    conf.odr = BMP280_ODR_1000_MS;
    bmp280_init(&bmp);
    bmp280_set_config(&conf, &bmp);
    bmp280_set_power_mode(BMP280_NORMAL_MODE, &bmp);
}

/*!
 * @brief Function that print temperature and pressure to the given UART. 
 * Requires initialized UART (bsp_usart.h)
 *
 * @param[in] uart: UART Handler address (&huart1, &huart2, etc.)
 * @return void
 */
void BMP280_Print(UART_HandleTypeDef *uart)
{
    unsigned char msg[55];
    sprintf((char *)msg,
            "\rTemperature = %.2f Celsius | Pressure = %.2f Pa\n\r", temp,
            pres);
    HAL_UART_Transmit(uart, msg, sizeof(msg), 100);
}

/*!
 *  @brief Function for writing the sensor's registers through I2C bus.
 *
 *  @param[in] i2c_addr : sensor I2C address.
 *  @param[in] reg_addr : Register address.
 *  @param[in] reg_data : Pointer to the data buffer whose value is to be
 * written.
 *  @param[in] length   : No of bytes to write.
 *
 *  @return Status of execution
 *  @retval 0 -> Success
 *  @retval -1 -> Failed
 *
 */
int8_t I2C_Reg_Write(uint8_t i2c_addr, uint8_t reg_addr, uint8_t *reg_data,
                     uint16_t length)
{

    uint16_t bmp280_addr = (bmp.dev_id << 1);
    if (HAL_I2C_Mem_Write(&hi2c1, bmp280_addr, reg_addr, 1,
                          reg_data, length, 500) == HAL_OK)
        return 0;
    else
        return -1;
}

/*!
 *  @brief Function for reading the sensor's registers through I2C bus.
 *
 *  @param[in] i2c_addr : Sensor I2C address.
 *  @param[in] reg_addr : Register address.
 *  @param[out] reg_data    : Pointer to the data buffer to store the read data.
 *  @param[in] length   : No of bytes to read.
 *
 *  @return Status of execution
 *  @retval 0 -> Success
 *  @retval -1 -> Failed
 *
 */
int8_t I2C_Reg_Read(uint8_t i2c_addr, uint8_t reg_addr, uint8_t *reg_data,
                    uint16_t length)
{

    uint16_t bmp280_addr;
    bmp280_addr = (bmp.dev_id << 1);

    if (HAL_I2C_Mem_Read(&hi2c1, bmp280_addr, reg_addr, 1, reg_data,
                         length, 500) == HAL_OK)
    {
        return 0;
    }
    else
        return -1;
}

/*!
 *  @brief Function for writing the sensor's registers through SPI bus.
 *
 *  @param[in] cs           : NOT USE
 *  @param[in] reg_addr     : NOT USE
 *  @param[in] reg_data 	: Pointer to the data buffer whose data has to be
 * written.
 *  @param[in] length       : Number of bytes to write.
 *
 *  @return Status of execution
 *  @retval 0 -> Success
 *  @retval -1 -> Failed
 *
 */
int8_t SPI_Reg_Write(uint8_t cs, uint8_t reg_addr, uint8_t *reg_data,
                     uint16_t length)
{
    HAL_GPIO_WritePin(BMP280_SPI_CS_PORT, BMP280_SPI_CS_PIN, GPIO_PIN_RESET);
    if (HAL_SPI_Transmit(&hspi1, reg_data, length, 100) == HAL_OK)
    {
        HAL_GPIO_WritePin(BMP280_SPI_CS_PORT, BMP280_SPI_CS_PIN, GPIO_PIN_SET);
        return 0;
    }
    else
        return -1;
}

/*!
 *  @brief Function for reading the sensor's registers through SPI bus.
 *
 *  @param[in] cs       	: NOT USE
 *  @param[in] reg_addr 	: Register command
 *  @param[out] reg_data    : Pointer to the data buffer to store the read data.
 *  @param[in] length   	: Number of bytes to read.
 *
 *  @return Status of execution
 *  @retval 0 -> Success
 *  @retval 1 -> Failed
 *
 */
int8_t SPI_Reg_Read(uint8_t cs, uint8_t reg_addr, uint8_t *reg_data,
                    uint16_t length)
{

    HAL_GPIO_WritePin(BMP280_SPI_CS_PORT, BMP280_SPI_CS_PIN, GPIO_PIN_RESET);
    if (HAL_SPI_Transmit(&hspi1, &reg_addr, 1, 100) == HAL_OK &&
        HAL_SPI_Receive(&hspi1, reg_data, length, 100) == HAL_OK)
    {
        HAL_GPIO_WritePin(BMP280_SPI_CS_PORT, BMP280_SPI_CS_PIN, GPIO_PIN_SET);
        return 0;
    }
    else
        return -1;
}


void MX_I2C1_Init(void)
{
  hi2c1.Instance = I2C1;
  hi2c1.Init.ClockSpeed = 100000;
  hi2c1.Init.DutyCycle = I2C_DUTYCYCLE_2;
  hi2c1.Init.OwnAddress1 = 0;
  hi2c1.Init.AddressingMode = I2C_ADDRESSINGMODE_7BIT;
  hi2c1.Init.DualAddressMode = I2C_DUALADDRESS_DISABLE;
  hi2c1.Init.OwnAddress2 = 0;
  hi2c1.Init.GeneralCallMode = I2C_GENERALCALL_DISABLE;
  hi2c1.Init.NoStretchMode = I2C_NOSTRETCH_DISABLE;
  if (HAL_I2C_Init(&hi2c1) != HAL_OK)
  {
    Error_Handler();
  }
}

void HAL_I2C_MspInit(I2C_HandleTypeDef* i2cHandle)
{

  GPIO_InitTypeDef GPIO_InitStruct = {0};
  if(i2cHandle->Instance==I2C1)
  {
    __HAL_RCC_GPIOB_CLK_ENABLE();
    /**I2C1 GPIO Configuration
    PB6     ------> I2C1_SCL
    PB7     ------> I2C1_SDA
    */
    GPIO_InitStruct.Pin = GPIO_PIN_6|GPIO_PIN_7;
    GPIO_InitStruct.Mode = GPIO_MODE_AF_OD;
    GPIO_InitStruct.Pull = GPIO_PULLUP;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
    GPIO_InitStruct.Alternate = GPIO_AF4_I2C1;
    HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

    /* I2C1 clock enable */
    __HAL_RCC_I2C1_CLK_ENABLE();
  }
}

void HAL_I2C_MspDeInit(I2C_HandleTypeDef* i2cHandle)
{

  if(i2cHandle->Instance==I2C1)
  {
    /* Peripheral clock disable */
    __HAL_RCC_I2C1_CLK_DISABLE();

    /**I2C1 GPIO Configuration
    PB6     ------> I2C1_SCL
    PB7     ------> I2C1_SDA
    */
    HAL_GPIO_DeInit(GPIOB, GPIO_PIN_6);

    HAL_GPIO_DeInit(GPIOB, GPIO_PIN_7);
  }
}

void MX_SPI1_Init(void)
{
  hspi1.Instance = SPI1;
  hspi1.Init.Mode = SPI_MODE_MASTER;
  hspi1.Init.Direction = SPI_DIRECTION_2LINES;
  hspi1.Init.DataSize = SPI_DATASIZE_8BIT;
  hspi1.Init.CLKPolarity = SPI_POLARITY_HIGH;
  hspi1.Init.CLKPhase = SPI_PHASE_2EDGE;
  hspi1.Init.NSS = SPI_NSS_SOFT;
  hspi1.Init.BaudRatePrescaler = SPI_BAUDRATEPRESCALER_4;
  hspi1.Init.FirstBit = SPI_FIRSTBIT_MSB;
  hspi1.Init.TIMode = SPI_TIMODE_DISABLE;
  hspi1.Init.CRCCalculation = SPI_CRCCALCULATION_DISABLE;
  hspi1.Init.CRCPolynomial = 10;
  if (HAL_SPI_Init(&hspi1) != HAL_OK)
  {
    Error_Handler();
  }
}

void HAL_SPI_MspInit(SPI_HandleTypeDef* spiHandle)
{

  GPIO_InitTypeDef GPIO_InitStruct = {0};
  if(spiHandle->Instance==SPI1)
  {
    /* SPI1 clock enable */
    __HAL_RCC_SPI1_CLK_ENABLE();

    __HAL_RCC_GPIOA_CLK_ENABLE();
    /**SPI1 GPIO Configuration
    PA5     ------> SPI1_SCK
    PA6     ------> SPI1_MISO
    PA7     ------> SPI1_MOSI
    */
    GPIO_InitStruct.Pin = GPIO_PIN_5|GPIO_PIN_6|GPIO_PIN_7;
    GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
    GPIO_InitStruct.Alternate = GPIO_AF5_SPI1;
    HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);
  }
}

void HAL_SPI_MspDeInit(SPI_HandleTypeDef* spiHandle)
{

  if(spiHandle->Instance==SPI1)
  {
    /* Peripheral clock disable */
    __HAL_RCC_SPI1_CLK_DISABLE();

    /**SPI1 GPIO Configuration
    PA5     ------> SPI1_SCK
    PA6     ------> SPI1_MISO
    PA7     ------> SPI1_MOSI
    */
    HAL_GPIO_DeInit(GPIOA, GPIO_PIN_5|GPIO_PIN_6|GPIO_PIN_7);
  }
}