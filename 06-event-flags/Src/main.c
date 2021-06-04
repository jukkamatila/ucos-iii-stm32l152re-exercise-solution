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
 ******************************************************************************/ 
#define TASK_1_STK_SIZE 256u
#define TASK_1_PRIO 2u
static OS_TCB Task1TCB;
static CPU_STK Task1Stk[TASK_1_STK_SIZE];
static void Task1(void *p_arg);

/*******************************************************************************
 * Task 2: Defines and Variables
 ******************************************************************************/ 
#define TASK_2_STK_SIZE 256u
#define TASK_2_PRIO 2u
static OS_TCB Task2TCB;
static CPU_STK Task2Stk[TASK_2_STK_SIZE];
static void Task2(void *p_arg);

/*******************************************************************************
 * Task 3: Defines and Variables
 ******************************************************************************/ 
#define TASK_3_STK_SIZE 256u
#define TASK_3_PRIO 2u
static OS_TCB Task3TCB;
static CPU_STK Task3Stk[TASK_3_STK_SIZE];
static void Task3(void *p_arg);

/*******************************************************************************
 * Shared resources
 ******************************************************************************/ 
OS_FLAG_GRP task_sync_flag;
#define BMP280_READ     0x01
#define MAX4466_READ    0x02

/*******************************************************************************
 * Main function
 ******************************************************************************/ 
int main(void)
{
    OS_ERR os_err;
    OSInit(&os_err);

    OSFlagCreate((OS_FLAG_GRP *)&task_sync_flag,
                    (CPU_CHAR *)"Task Synchronize Flag Group",
                    (OS_FLAGS) 0,
                    (OS_ERR *)&os_err);

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
 * Tasks
 ******************************************************************************/ 
static void AppTaskStart(void *p_arg)
{
    OS_ERR os_err;

    HAL_Init();
    SystemClock_Config();
    LED_Init(GPIOA, GPIO_PIN_5, GPIO_PIN_RESET);
    MX_USART2_UART_Init();
    MAX4466_Init();
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
        uint8_t msg[] = "\rTask 1\n\r";
        HAL_UART_Transmit(&huart2, msg, sizeof(msg), 100);
        BMP280_Read();
        OSFlagPost( (OS_FLAG_GRP *)&task_sync_flag,
                    (OS_FLAGS)BMP280_READ,
                    (OS_OPT)OS_OPT_POST_FLAG_SET,
                    (OS_ERR *)os_err);
        OSTimeDlyHMSM(0, 0, 5, 0, OS_OPT_TIME_HMSM_STRICT, &os_err);
    }
}

static void Task2(void *p_arg)
{
    OS_ERR os_err;
    
    while (DEF_TRUE)
    {
        uint8_t msg[] = "\rTask 2\n\r";
        HAL_UART_Transmit(&huart2, msg, sizeof(msg), 100);
        MAX4466_Read();
        OSFlagPost( (OS_FLAG_GRP *)&task_sync_flag,
                    (OS_FLAGS)MAX4466_READ,
                    (OS_OPT)OS_OPT_POST_FLAG_SET,
                    (OS_ERR *)os_err);
        OSTimeDlyHMSM(0, 0, 1, 0, OS_OPT_TIME_HMSM_STRICT, &os_err);
    }
}

static void Task3(void *p_arg)
{
    OS_ERR os_err;
    
    while (DEF_TRUE)
    {
        OSFlagPend( (OS_FLAG_GRP *)&task_sync_flag,
                    (OS_FLAGS)BMP280_READ + MAX4466_READ,
                    (OS_TICK) 0,
                    (OS_OPT)OS_OPT_PEND_FLAG_SET_ALL + OS_OPT_PEND_FLAG_CONSUME,
                    (CPU_TS *)NULL,
                    (OS_ERR*)&os_err);
        uint8_t msg[] = "\rTask 3\n\r";
        HAL_UART_Transmit(&huart2, msg, sizeof(msg), 100);
        BMP280_Print(&huart2);
        MAX4466_Print(&huart2);
        OSTimeDlyHMSM(0, 0, 0, 500, OS_OPT_TIME_HMSM_STRICT, &os_err);
    }
}