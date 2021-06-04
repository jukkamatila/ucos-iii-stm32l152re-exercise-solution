/**
 * @file bsp_ssd1306_cfg.h
 */

#ifndef __BSP_SSD1306_CFG_H__
#define __BSP_SSD1306_CFG_H__

#ifdef __cplusplus
extern "C"
{
#endif
#include "bsp_ssd1306.h"
#include "bsp_clk.h"

SPI_HandleTypeDef hspi2;

#define SSD1306_DC_PIN GPIO_PIN_8
#define SSD1306_DC_PORT GPIOA
/** @brief RESET Pin is active LOW 
 *  Keep this pin HIGH during normal operation
 */
#define SSD1306_RESET_PIN GPIO_PIN_9
#define SSD1306_RESET_PORT GPIOA
#define SSD1306_CS_PIN GPIO_PIN_10
#define SSD1306_CS_PORT GPIOA


#define SSD1306_CS_PORT GPIOA
#define SSD1306_CS_PIN GPIO_PIN_10

#define SSD1306_SPI_HANDLER &hspi2

void MX_SPI2_Init(void);
void SSD1390_CtrlPinInit(void);
void SSD1306_Init(void);

/**
 * @brief To write command to SSD1306
 *
 * @param[in] cmd: specify command
 *
 * @note This function is an internal API and not intented to use directly
 *
 * @return execution status
 * @retval SSD1306_OK for Success, SSD1306_WRITECMD_FAILED for failed.
 */
int8_t SSD1306_WriteCommand(uint8_t cmd);

/**
 * @brief To write data to SSD1306
 *
 * @param[in] data: pointer to data
 * @param[in] length: length of data
 *
 * @note This function is an internal API and not intented to use directly
 *
 * @return execution status
 * @retval SSD1306_OK for Success, SSD1306_WRITEDATA_FAILED for failed.
 */
int8_t SSD1306_WriteData(uint8_t *data, uint8_t length);

/**
 * @brief creates delay
 * @param[in] ms : time in milliseconds
 */
void SSD1306_Delay(uint32_t ms);

/**
 * @brief set CS pin LOW
 */
void SSD1306_CSPinOff(void);

/**
 * @brief set CS pin HIGH
 */
void SSD1306_CSPinOn(void);

/**
 * @brief set DC LOW
 */
void SSD1306_DCPinOff(void);

/**
 * @brief set DC HIGH
 */
void SSD1306_DCPinOn(void);

/**
 * @brief set RESET LOW
 */
void SSD1306_ResetPinOff(void);

/**
 * @brief set RESET HIGH
 */
void SSD1306_ResetPinOn(void);

#ifdef __cplusplus
}
#endif /* End of CPP guard */

#endif /* _BSP_BMP280_CFG_H_ */