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
 * Task 1 reads data from BMP280
 * (High priority)
 ******************************************************************************/ 
#define TASK_1_STK_SIZE 256u
#define TASK_1_PRIO 2u
static OS_TCB Task1TCB;
static CPU_STK Task1Stk[TASK_1_STK_SIZE];
static void Task1(void *p_arg);

/*******************************************************************************
 * Task 2: Defines and Variables
 * Task 2 send data to UART (temperature and humidity)
 * (Medium priority)
 ******************************************************************************/ 
#define TASK_2_STK_SIZE 256u
#define TASK_2_PRIO 3u
static OS_TCB Task2TCB;
static CPU_STK Task2Stk[TASK_2_STK_SIZE];
static void Task2(void *p_arg);

/*******************************************************************************
 * Task 3: Defines and Variables
 * Task 3 blinks LED 3 times
 * (Low priority)
 ******************************************************************************/ 
#define TASK_3_STK_SIZE 1024u
#define TASK_3_PRIO 4u
static OS_TCB Task3TCB;
static CPU_STK Task3Stk[TASK_3_STK_SIZE];
static void Task3(void *p_arg);

/*******************************************************************************
 * Shared resources
 ******************************************************************************/ 
OS_SEM task_sync_sem;
uint8_t is_bmp280_read = 0;

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
      (OS_SEM_CTR)1,
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

    OSTaskCreate(
        (OS_TCB *)&Task3TCB,
        (CPU_CHAR *)"Task 3",
        (OS_TASK_PTR)Task3,
        (void *)0,
        (OS_PRIO)TASK_3_PRIO,
        (CPU_STK *)&Task3Stk[0],
        (CPU_STK_SIZE)TASK_3_STK_SIZE / 10,
        (CPU_STK_SIZE)TASK_3_STK_SIZE,
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
        HAL_UART_Transmit(&huart2, (uint8_t *)"\rTask 1 - OSSemPend\n\r", 22, 100);
        OSSemPend(
            (OS_SEM *)&task_sync_sem,
            (OS_TICK)0,
            (OS_OPT)OS_OPT_PEND_BLOCKING,
            (CPU_TS *)NULL,
            (OS_ERR *)&os_err);

        uint8_t msg[] = "\rTask 1 run\n\r";
        HAL_UART_Transmit(&huart2, msg, sizeof(msg), 100);
        if(is_bmp280_read == 0)
        {
            BMP280_Read();
            is_bmp280_read = 1;
        }
        HAL_UART_Transmit(&huart2, (uint8_t *)"\rTask 1 - OSSemPost\n\r", 22, 100);
        OSSemPost(
            (OS_SEM *)&task_sync_sem,
            (OS_OPT)OS_OPT_POST_1,
            (OS_ERR *)&os_err);

        OSTimeDlyHMSM(0, 0, 1, 0, OS_OPT_TIME_HMSM_STRICT, &os_err);
    }
}

static void Task2(void *p_arg)
{
    OS_ERR os_err;

    while (DEF_TRUE)
    {
        uint8_t msg[] = "\rTask 2 run\n\r";
        HAL_UART_Transmit(&huart2, msg, sizeof(msg), 100);
        BMP280_Print(&huart2);
        OSTimeDlyHMSM(0, 0, 2, 0, OS_OPT_TIME_HMSM_STRICT, &os_err);
    }
}

static void Task3(void *p_arg)
{
    OS_ERR os_err;

    while (DEF_TRUE)
    {
        HAL_UART_Transmit(&huart2, (uint8_t *)"\rTask 3 - OSSemPend\n\r", 22, 100);
        OSSemPend(
            (OS_SEM *)&task_sync_sem,
            (OS_TICK)0,
            (OS_OPT)OS_OPT_PEND_BLOCKING,
            (CPU_TS *)NULL,
            (OS_ERR *)&os_err);

        uint8_t msg[] = "\rTask 3 run\n\r";
        HAL_UART_Transmit(&huart2, msg, sizeof(msg), 100);

        if(is_bmp280_read == 1)
        {
            for(int i = 0; i < 3; i++)
            {
                HAL_UART_Transmit(&huart2, (uint8_t*)"\rBlink\n\r", 9, 100);
                LED_On(GPIOA, GPIO_PIN_5);
                Delay_Blocking(500);
                LED_Off(GPIOA, GPIO_PIN_5);
                Delay_Blocking(500);
            }
            is_bmp280_read = 0;
        }

        HAL_UART_Transmit(&huart2, (uint8_t *)"\rTask 3 - OSSemPost\n\r", 22, 100);
        OSSemPost(
            (OS_SEM *)&task_sync_sem,
            (OS_OPT)OS_OPT_POST_1,
            (OS_ERR *)&os_err);

        OSTimeDlyHMSM(0, 0, 0, 500, OS_OPT_TIME_HMSM_STRICT, &os_err);
    }
}