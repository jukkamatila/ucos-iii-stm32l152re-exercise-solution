#include "main.h"

/*******************************************************************************
 * App Task Start: Defines and Variables
 ******************************************************************************/ 
#define APP_TASK_START_STK_SIZE 1024u
#define APP_TASK_START_PRIO 1u
static OS_TCB AppTaskStartTCB;
static CPU_STK AppTaskStartStk[APP_TASK_START_STK_SIZE];
static void AppTaskStart(void *p_arg);

/*******************************************************************************
 * Task 1: Defines and Variables
 * Task 1 reads & display when User button is pressed
 * (High priority)
 ******************************************************************************/ 
#define TASK_1_STK_SIZE 256u
#define TASK_1_PRIO 2u
static OS_TCB Task1TCB;
static CPU_STK Task1Stk[TASK_1_STK_SIZE];
static void Task1(void *p_arg);

/*******************************************************************************
 * Shared resources
 ******************************************************************************/ 
OS_SEM sem;

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
        (OS_SEM *)&sem,
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
    MX_USART2_UART_Init();
    BMP280_Setup();
    Button_Init();

    OSTaskCreate(
        (OS_TCB *)&Task1TCB,
        (CPU_CHAR *)"Task 1",
        (OS_TASK_PTR)Task1,
        (void *)0,
        (OS_PRIO)TASK_1_PRIO,
        (CPU_STK *)&Task1Stk[0],
        (CPU_STK_SIZE)TASK_1_STK_SIZE / 10,
        (CPU_STK_SIZE)TASK_1_STK_SIZE,
        (OS_MSG_QTY)5u,
        (OS_TICK)0u,
        (void *)0,
        (OS_OPT)(OS_OPT_TASK_STK_CHK | OS_OPT_TASK_STK_CLR),
        (OS_ERR *)&os_err);
}

static void Task1(void *p_arg)
{
    OS_ERR os_err;
    
    while (DEF_TRUE)
    {
        HAL_UART_Transmit(&huart2, (uint8_t *)"\rTask 1 - OSMutexPend\n\r", 22, 100);
        OSSemPend(
            (OS_SEM *)&sem,
            (OS_TICK)0,
            (OS_OPT)OS_OPT_PEND_BLOCKING,
            (CPU_TS *)NULL,
            (OS_ERR *)&os_err);

        uint8_t msg[] = "\rTask 1 run\n\r";
        HAL_UART_Transmit(&huart2, msg, sizeof(msg), 100);

        BMP280_Read();
        BMP280_Print(&huart2);

        OSTimeDlyHMSM(0, 0, 1, 0, OS_OPT_TIME_HMSM_STRICT, &os_err);
    }
}


void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin)
{
    OS_ERR os_err;
    if(GPIO_Pin == GPIO_PIN_13)
    {
        HAL_UART_Transmit(&huart2, (uint8_t *)"\rButton pressed\n\r", 20, 100);
        OSSemPost(
            (OS_SEM *)&sem,
            (OS_OPT)OS_OPT_POST_1,
            (OS_ERR *)&os_err);
    }
}