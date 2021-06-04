#include "main.h"

/*******************************************************************************
 * DEFINES
*******************************************************************************/

#define APP_TASK_START_STK_SIZE 128u
#define APP_TASK_START_PRIO 1u
#define BLINK_TASK_STK_SIZE 128u
#define BLINK_TASK_PRIO 2u
#define REUSED_STACK_TASK_PRIO 3u


/*******************************************************************************
 * VARIABLES
*******************************************************************************/

static OS_TCB AppTaskStartTCB;
static CPU_STK AppTaskStartStk[APP_TASK_START_STK_SIZE];

static OS_TCB BlinkTaskTCB;
static CPU_STK BlinkTaskStk[BLINK_TASK_STK_SIZE];

/*******************************************************************************
 * FUNCTION PROTOTYPES
*******************************************************************************/

static void AppTaskStart(void *p_arg);
static void BlinkTask(void *p_arg);
static void ReusedStackTask(void *p_arg);

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
    MX_USART2_UART_Init();
    LED_Init(GPIOA, GPIO_PIN_5, GPIO_PIN_RESET);

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
    while (DEF_TRUE)
    {
        uint8_t msg[] = "\rApp Task Start\r\n";
        HAL_UART_Transmit(&huart2, msg, sizeof(msg), 100);
        OSTimeDlyHMSM(0, 0, 0, 500, OS_OPT_TIME_HMSM_STRICT, &os_err);

        /* delete the currently running task */
        OSTaskDel((OS_TCB *)0, &os_err);
    }
}

static void BlinkTask(void *p_arg)
{
    OS_ERR os_err;

    /* Reuse OS_TCB and stack of AppTaskStart for ReusedStackTask */
    OSTaskCreate(
        (OS_TCB *)&AppTaskStartTCB,
        (CPU_CHAR *)"Reused Stack Task",
        (OS_TASK_PTR)ReusedStackTask,
        (void *)0,
        (OS_PRIO)REUSED_STACK_TASK_PRIO,
        (CPU_STK *)&AppTaskStartStk[0],
        (CPU_STK_SIZE)APP_TASK_START_STK_SIZE / 10,
        (CPU_STK_SIZE)APP_TASK_START_STK_SIZE,
        (OS_MSG_QTY)5u,
        (OS_TICK)0u,
        (void *)0,
        (OS_OPT)(OS_OPT_TASK_STK_CHK | OS_OPT_TASK_STK_CLR),
        (OS_ERR *)&os_err);

    while (DEF_TRUE)
    {
        uint8_t msg[] = "\rBlink Task\r\n";
        HAL_UART_Transmit(&huart2, msg, sizeof(msg), 100);
        LED_Toggle(GPIOA, GPIO_PIN_5);
        OSTimeDlyHMSM(0, 0, 0, 500, OS_OPT_TIME_HMSM_STRICT, &os_err);
    }
}

static void ReusedStackTask(void *p_arg)
{
    OS_ERR os_err;
    while (DEF_TRUE)
    {
        uint8_t msg[] = "\rReused Stack Task\r\n";
        HAL_UART_Transmit(&huart2, msg, sizeof(msg), 100);
        OSTimeDlyHMSM(0, 0, 0, 500, OS_OPT_TIME_HMSM_STRICT, &os_err);
    }
}
