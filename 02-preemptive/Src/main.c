/*******************************************************************************
 * INCLUDES
*******************************************************************************/
#include <stdio.h>
#include "stm32l1xx_hal.h"
#include "os.h"
#include "bsp_clk.h"
#include "bsp_led.h"
#include "bsp_usart.h"

/*******************************************************************************
 * COUNT TASK's DEFINES, VARIABLES AND FUNCTION PROTOTYPE
*******************************************************************************/
#define COUNT_TASK_STK_SIZE 128u
#define COUNT_TASK_PRIO 1u
static OS_TCB CountTaskTCB;
static CPU_STK CountTaskStk[COUNT_TASK_STK_SIZE];
static void CountTask(void *p_arg);

/*******************************************************************************
 * BLINK TASK's DEFINES, VARIABLES AND FUNCTION PROTOTYPE
*******************************************************************************/
#define BLINK_TASK_STK_SIZE 128u
#define BLINK_TASK_PRIO 2u
static OS_TCB BlinkTaskTCB;
static CPU_STK BlinkTaskStk[BLINK_TASK_STK_SIZE];
static void BlinkTask(void *p_arg);

/*******************************************************************************
 * MAIN FUNCTION
*******************************************************************************/

int main(void)
{
    HAL_Init();
    SystemClock_Config();
    UART_Init();
    LED_Init();

    OS_ERR os_err;
    OSInit(&os_err);

    if (os_err != OS_ERR_NONE)
    {
        while (DEF_TRUE)
            ;
    }

    OSTaskCreate(
        (OS_TCB *)&CountTaskTCB,
        (CPU_CHAR *)"Count Task",
        (OS_TASK_PTR)CountTask,
        (void *)0,
        (OS_PRIO)COUNT_TASK_PRIO,
        (CPU_STK *)&CountTaskStk[0],
        (CPU_STK_SIZE)COUNT_TASK_STK_SIZE / 10,
        (CPU_STK_SIZE)COUNT_TASK_STK_SIZE,
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

    if (os_err != OS_ERR_NONE)
    {
        while (DEF_TRUE)
            ;
    }

    OSStart(&os_err);
}

/*******************************************************************************
 * TASKS
*******************************************************************************/

static void CountTask(void *p_arg)
{
    OS_ERR os_err;

    while (DEF_TRUE)
    {
        uint8_t msg[] = "\rCount Task\r\n";
        UART_Transmit(msg, sizeof(msg));
        for(int i = 0; i < 2; i++)
        {
            uint8_t count[5];
            sprintf((char*)count, "\r%d\n\r", i+1);
            UART_Transmit(count, sizeof(count));
            Delay_Blocking(1000);
        }		
        OSTimeDlyHMSM(0, 0, 2, 0, OS_OPT_TIME_HMSM_STRICT, &os_err);

    }
}

static void BlinkTask(void *p_arg)
{
    OS_ERR os_err;

    while (DEF_TRUE)
    {
        uint8_t msg[] = "\rBlink Task\r\n";
        UART_Transmit(msg, sizeof(msg));
        LED_Toggle();
        OSTimeDlyHMSM(0, 0, 1, 0, OS_OPT_TIME_HMSM_STRICT, &os_err);
    }
}