#include "main.h"

/*******************************************************************************
 * App Task Start: Defines and Variables
 ******************************************************************************/ 
#define APP_TASK_START_STK_SIZE 128u
#define APP_TASK_START_PRIO 1u
static OS_TCB AppTaskStartTCB;
static CPU_STK AppTaskStartStk[APP_TASK_START_STK_SIZE];
static void AppTaskStart(void *p_arg);

/*******************************************************************************
 * Task 1: Defines and Variables
 * Task 1 reads data from BMP280
 ******************************************************************************/ 
#define TASK_1_STK_SIZE 128u
#define TASK_1_PRIO 2u
static OS_TCB Task1TCB;
static CPU_STK Task1Stk[TASK_1_STK_SIZE];
static void Task1(void *p_arg);

/*******************************************************************************
 * Task 2: Defines and Variables
 * Task 2 display the data (from Task 1) using USART2
 ******************************************************************************/ 
#define TASK_2_STK_SIZE 256u
#define TASK_2_PRIO 2u
static OS_TCB Task2TCB;
static CPU_STK Task2Stk[TASK_2_STK_SIZE];
static void Task2(void *p_arg);

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
    MX_USART2_UART_Init();
    BMP280_Setup();

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

    OSTaskCreate(
        (OS_TCB *)&Task2TCB,
        (CPU_CHAR *)"Task 2",
        (OS_TASK_PTR)Task2,
        (void *)0,
        (OS_PRIO)TASK_2_PRIO,
        (CPU_STK *)&Task2Stk[0],
        (CPU_STK_SIZE)TASK_2_STK_SIZE / 10,
        (CPU_STK_SIZE)TASK_2_STK_SIZE,
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
        uint8_t msg[] = "\rTask 1\n\r";
        HAL_UART_Transmit(&huart2, msg, sizeof(msg), 100);
        BMP280_Read();

        OSSemPost(
            (OS_SEM *)&task_sync_sem,
            (OS_OPT)OS_OPT_POST_1,
            (OS_ERR *)&os_err);

        OSTimeDlyHMSM(0, 0, 3, 0, OS_OPT_TIME_HMSM_STRICT, &os_err);
    }
}

static void Task2(void *p_arg)
{
    OS_ERR os_err;

    while (DEF_TRUE)
    {
        OSSemPend(
            (OS_SEM *)&task_sync_sem,
            (OS_TICK)0,
            (OS_OPT)OS_OPT_PEND_BLOCKING,
            (CPU_TS *)NULL,
            (OS_ERR *)&os_err);

        uint8_t msg[] = "\rTask 2\n\r";
        HAL_UART_Transmit(&huart2, msg, sizeof(msg), 100);
        BMP280_Print(&huart2);

        OSTimeDlyHMSM(0, 0, 0, 500, OS_OPT_TIME_HMSM_STRICT, &os_err);
    }
}