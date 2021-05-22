/**
 * @file bsp_clk.c
 */

#include "bsp_clk.h"

void Delay_Blocking(uint16_t period_ms)
{
    OS_ERR os_err;
    OS_TICK ticks = ((OSCfg_TickRate_Hz * 
                    ((OS_TICK)period_ms + ((OS_TICK)500u / OSCfg_TickRate_Hz)))
                    / (OS_TICK)1000u);
    OS_TICK start_ticks = OSTimeGet(&os_err);
    OS_TICK current_ticks = start_ticks;

    while (current_ticks - start_ticks <= ticks)
    {
        current_ticks = OSTimeGet(&os_err);
        if (current_ticks < start_ticks)
        {
            ticks = (start_ticks + ticks) - UINT32_MAX;
            start_ticks = 0;
        }
    }
}

void Delay_NonBlocking(uint16_t period_ms)
{
    OS_ERR os_err;
    OSTimeDlyHMSM(period_ms / 3600000, 
					(period_ms / 60000) % 60, 
					(period_ms / 1000) % 60, 
					period_ms % 1000, 
					OS_OPT_TIME_HMSM_STRICT, 
					&os_err);
}

void Error_Handler(void)
{
    __disable_irq();
    while (1)
    {
    }
}

void SystemClock_Config(void)
{
    RCC_OscInitTypeDef RCC_OscInitStruct = {0};
    RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

    /* Configure the main internal regulator output voltage */
    __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE1);

    /* Initializes the RCC Oscillators according to the specified parameters
	 * in the RCC_OscInitTypeDef structure.*/
    RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI;
    RCC_OscInitStruct.HSIState = RCC_HSI_ON;
    RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
    RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
    RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSI;
    RCC_OscInitStruct.PLL.PLLMUL = RCC_PLL_MUL6;
    RCC_OscInitStruct.PLL.PLLDIV = RCC_PLL_DIV3;
    if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
    {
        Error_Handler();
    }
    /* Initializes the CPU, AHB and APB buses clocks */
    RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK 
                                | RCC_CLOCKTYPE_SYSCLK 
                                | RCC_CLOCKTYPE_PCLK1 
                                | RCC_CLOCKTYPE_PCLK2;
    RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
    RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
    RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;
    RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

    if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_1) != HAL_OK)
    {
        Error_Handler();
    }
}
