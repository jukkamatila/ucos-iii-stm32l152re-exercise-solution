#include "main.h"

/*******************************************************************************
 * DEFINES
*******************************************************************************/

#define APP_TASK_START_STK_SIZE 128u
#define APP_TASK_START_PRIO 1u

/*******************************************************************************
 * VARIABLES
*******************************************************************************/

static OS_TCB AppTaskStartTCB;
static CPU_STK AppTaskStartStk[APP_TASK_START_STK_SIZE];

/*******************************************************************************
 * FUNCTION PROTOTYPES
*******************************************************************************/

static void AppTaskStart(void *p_arg);

/*******************************************************************************
 * MAIN FUNCTION
*******************************************************************************/

int main(void)
{
    OS_ERR os_err;
    OSInit(&os_err);

    if (os_err != OS_ERR_NONE)
    {
        while (DEF_TRUE)
            ;
    }

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
 * TASKS
*******************************************************************************/

static void AppTaskStart(void *p_arg)
{
    OS_ERR os_err;
	
    HAL_Init();
    SystemClock_Config();

    LED_Init(GPIOA, GPIO_PIN_5, GPIO_PIN_RESET);
    
    while (DEF_TRUE)
    {
        LED_Toggle(GPIOA, GPIO_PIN_5);
        OSTimeDlyHMSM(0, 0, 0, 500, OS_OPT_TIME_HMSM_STRICT, &os_err);
    }
}
