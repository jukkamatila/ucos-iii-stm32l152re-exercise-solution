#include "main.h"
#include <stdio.h>
#include "stm32l1xx_hal.h"
#include "os.h"
#include "bsp_clk.h"
#include "bsp_bmp280_cfg.h"
#include "bsp_usart.h"
#include "bsp_led.h"

/*******************************************************************************
 * App Task Start: Defines and Variables
 ******************************************************************************/ 
#define APP_TASK_START_STK_SIZE 1024u
#define APP_TASK_START_PRIO 1u
static OS_TCB AppTaskStartTCB;
static CPU_STK AppTaskStartStk[APP_TASK_START_STK_SIZE];
static void AppTaskStart(void *p_arg);

/*******************************************************************************
 * Read Task: Defines and Variables
 * reads data from BMP280
 ******************************************************************************/ 
#define READTASK_STK_SIZE 256u
#define READTASK_PRIO 4u
static OS_TCB ReadTaskTCB;
static CPU_STK ReadTaskStk[READTASK_STK_SIZE];
static void ReadTask(void *p_arg);

/*******************************************************************************
 * Task 2: Defines and Variables
 * Task 2 send data to UART (temperature and humidity)
 ******************************************************************************/ 
#define TASK_2_STK_SIZE 256u
#define TASK_2_PRIO 2u
static OS_TCB PrintTaskTCB;
static CPU_STK PrintTaskStk[TASK_2_STK_SIZE];
static void PrintTask(void *p_arg);

/*******************************************************************************
 * Blink Task: Defines and Variables
 * Blink Task blinks LED 3 times
 ******************************************************************************/ 
#define BLINK_TASK_STK_SIZE 1024u
#define BLINK_TASK_PRIO 3u
static OS_TCB BlinkTaskTCB;
static CPU_STK BlinkTaskStk[BLINK_TASK_STK_SIZE];
static void BlinkTask(void *p_arg);
void Blink();

/*******************************************************************************
 * Shared resources
 ******************************************************************************/ 
OS_SEM task_sync_sem;

/*******************************************************************************
 * Main function
 ******************************************************************************/ 
int main(void)
{
    OS_ERR os_err;
    OSInit(&os_err);

    if (os_err != OS_ERR_NONE)
    {
        while (DEF_TRUE)
            ;
    }
  OSSemCreate(
      (OS_SEM *)&task_sync_sem,
      (CPU_CHAR *)"Task Synchronize Semaphore",
      (OS_SEM_CTR)0,
      (OS_ERR *)&os_err);

  OSTaskCreate(
      (OS_TCB *)&AppTaskStartTCB,
      (CPU_CHAR *)"App Task Start",
      (OS_TASK_PTR)AppTaskStart,
      (void *)0,
      (OS_PRIO)APP_TASK_START_PRIO,
      (CPU_STK *)&AppTaskStartStk[0],
      (CPU_STK_SIZE)APP_TASK_START_STK_SIZE / 10,
      (CPU_STK_SIZE)APP_TASK_START_STK_SIZE,
      (OS_MSG_QTY)5u,
      (OS_TICK)0u,
      (void *)0,
      (OS_OPT)(OS_OPT_TASK_STK_CHK | OS_OPT_TASK_STK_CLR),
      (OS_ERR *)&os_err);

    if (os_err != OS_ERR_NONE)
    {
        while (DEF_TRUE)
            ;
    }
    OSStart(&os_err);
}

/*******************************************************************************
 * Tasks
 ******************************************************************************/ 
static void AppTaskStart(void *p_arg)
{
    OS_ERR os_err;

    HAL_Init();
    SystemClock_Config();
    LED_Init(GPIOA, GPIO_PIN_5, GPIO_PIN_RESET);
    UART_Init();
    BMP280_Setup();

    OSTaskCreate(
        (OS_TCB *)&ReadTaskTCB,
        (CPU_CHAR *)"Read Task",
        (OS_TASK_PTR)ReadTask,
        (void *)0,
        (OS_PRIO)READTASK_PRIO,
        (CPU_STK *)&ReadTaskStk[0],
        (CPU_STK_SIZE)READTASK_STK_SIZE / 10,
        (CPU_STK_SIZE)READTASK_STK_SIZE,
        (OS_MSG_QTY)5u,
        (OS_TICK)0u,
        (void *)0,
        (OS_OPT)(OS_OPT_TASK_STK_CHK | OS_OPT_TASK_STK_CLR),
        (OS_ERR *)&os_err);

    OSTaskCreate(
        (OS_TCB *)&PrintTaskTCB,
        (CPU_CHAR *)"Task 2",
        (OS_TASK_PTR)PrintTask,
        (void *)0,
        (OS_PRIO)TASK_2_PRIO,
        (CPU_STK *)&PrintTaskStk[0],
        (CPU_STK_SIZE)TASK_2_STK_SIZE / 10,
        (CPU_STK_SIZE)TASK_2_STK_SIZE,
        (OS_MSG_QTY)5u,
        (OS_TICK)0u,
        (void *)0,
        (OS_OPT)(OS_OPT_TASK_STK_CHK | OS_OPT_TASK_STK_CLR),
        (OS_ERR *)&os_err);

    OSTaskCreate(
        (OS_TCB *)&BlinkTaskTCB,
        (CPU_CHAR *)"Blink Task",
        (OS_TASK_PTR)BlinkTask,
        (void *)0,
        (OS_PRIO)BLINK_TASK_PRIO,
        (CPU_STK *)&BlinkTaskStk[0],
        (CPU_STK_SIZE)BLINK_TASK_STK_SIZE / 10,
        (CPU_STK_SIZE)BLINK_TASK_STK_SIZE,
        (OS_MSG_QTY)5u,
        (OS_TICK)0u,
        (void *)0,
        (OS_OPT)(OS_OPT_TASK_STK_CHK | OS_OPT_TASK_STK_CLR),
        (OS_ERR *)&os_err);
}

static void ReadTask(void *p_arg)
{
    OS_ERR os_err;
    
    while (DEF_TRUE)
    {
        uint8_t msg[] = "\rRead Task - run\n\r";
        HAL_UART_Transmit(&huart2, msg, sizeof(msg), 100);

        Delay_Blocking(5000);
        BMP280_Read();

        HAL_UART_Transmit(&huart2, (uint8_t *)"\rRead Task - OSSemPost\n\r", 25, 100);
        OSSemPost(
            (OS_SEM *)&task_sync_sem,
            (OS_OPT)OS_OPT_POST_1,
            (OS_ERR *)&os_err);
        HAL_UART_Transmit(&huart2, (uint8_t *)"\rRead Task - end\n\r", 19, 100);

        OSTimeDlyHMSM(0, 0, 6, 0, OS_OPT_TIME_HMSM_STRICT, &os_err);
    }
}

static void PrintTask(void *p_arg)
{
    OS_ERR os_err;

    while (DEF_TRUE)
    {
        uint8_t msg[] = "\rPrint Task - run\n\r";
        HAL_UART_Transmit(&huart2, msg, sizeof(msg), 100);
        HAL_UART_Transmit(&huart2, (uint8_t *)"\rPrint Task - OSSemPend\n\r", 26, 100);
        OSSemPend(
            (OS_SEM *)&task_sync_sem,
            (OS_TICK)0,
            (OS_OPT)OS_OPT_PEND_BLOCKING,
            (CPU_TS *)NULL,
            (OS_ERR *)&os_err);

        HAL_UART_Transmit(&huart2, (uint8_t *)"\rPrint Task - resume\n\r", 23, 100);
        BMP280_Print(&huart2);

        HAL_UART_Transmit(&huart2, (uint8_t *)"\rPrint Task - end\n\r", 20, 100);

        OSTimeDlyHMSM(0, 0, 6, 0, OS_OPT_TIME_HMSM_STRICT, &os_err);
    }
}

static void BlinkTask(void *p_arg)
{
    OS_ERR os_err;

    while (DEF_TRUE)
    {

        uint8_t msg[] = "\rBlink Task - run\n\r";
        HAL_UART_Transmit(&huart2, msg, sizeof(msg), 100);

        for(int i = 0; i < 3; i++)
        {
            uint8_t countmsg[12];
            sprintf((char*)countmsg, "\rBlink %d\n\r", i+1);
            HAL_UART_Transmit(&huart2,countmsg, sizeof(countmsg) , 100);
            Blink();
        }
        HAL_UART_Transmit(&huart2, (uint8_t *)"\rBlink Task - end\n\r", 20, 100);

        OSTimeDlyHMSM(0, 0, 0, 3, OS_OPT_TIME_HMSM_STRICT, &os_err);
    }
}

void Blink()
{
    LED_On(GPIOA, GPIO_PIN_5);
    Delay_Blocking(500);
    LED_Off(GPIOA, GPIO_PIN_5);
    Delay_Blocking(500);
}