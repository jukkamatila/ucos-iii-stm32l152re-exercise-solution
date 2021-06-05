#include "main.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "stm32l1xx_hal.h"
#include "os.h"
#include "bsp_clk.h"
#include "bsp_usart.h"
#include "bsp_ssd1306_cfg.h"
#include "bsp_max4466.h"

struct OLED_Data_t{
    uint8_t data[60];
    uint8_t pos;
};

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
#define TASK_2_STK_SIZE 2048u
#define TASK_2_PRIO 2u
static OS_TCB Task2TCB;
static CPU_STK Task2Stk[TASK_2_STK_SIZE];
static void Task2(void *p_arg);

/*******************************************************************************
 * Shared resources
 ******************************************************************************/ 

OS_Q queue;
OS_Q queue_pos;
struct OLED_Data_t oled_data;

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
    UART_Init();
    SSD1306_Init();
    SSD1306_Board_Initialize();
    SSD1306_Board_PrintTitle(0, (uint8_t*)"Random Characters",18);
    MAX4466_Init();
    uint8_t seed = MAX4466_Read();
    srand(seed);

    OSQCreate(  (OS_Q*)&queue,
                (CPU_CHAR*)"Message Queue for Data",
                (OS_MSG_QTY)1,
                (OS_ERR*)&os_err);

    OSQCreate(  (OS_Q*)&queue_pos,
                (CPU_CHAR*)"Message Queue for Position",
                (OS_MSG_QTY)1,
                (OS_ERR*)&os_err);

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

        oled_data.data[oled_data.pos] = (uint8_t)rand() % 95;
        OSQPost(    (OS_Q*)&queue,
                    (void *)&oled_data,
                    (OS_MSG_SIZE)sizeof(oled_data),
                    (OS_OPT)OS_OPT_POST_FIFO,
                    (OS_ERR*)&os_err);
        OSTaskDel(NULL, &os_err);
}

static void Task1(void *p_arg)
{
    OS_ERR os_err;
    OS_MSG_SIZE msg_size;
    struct OLED_Data_t *recv;

    while (DEF_TRUE)
    {
        uint8_t msg[] = "\rTask 1\n\r";
        UART_Transmit(msg, sizeof(msg));
        recv = OSQPend( (OS_Q*)&queue_pos,
                    (OS_TICK)0,
                    (OS_OPT)OS_OPT_PEND_BLOCKING,
                    (OS_MSG_SIZE*)&msg_size,
                    (CPU_TS*)NULL,
                    (OS_ERR*)&os_err);
        if(recv->pos < SSD1306_BOARD_MAX_CHAR)
        {
            recv->data[recv->pos] = (uint8_t)rand() % 95;
        }
        else
        {
            UART_Transmit((uint8_t *)"\rDeleted tasks.\n\r",18);
            OSTaskDel(&Task2TCB, &os_err);
            OSTaskDel(NULL, &os_err);
        }

        OSQPost(    (OS_Q*)&queue,
                    (void *)&oled_data,
                    (OS_MSG_SIZE)sizeof(oled_data),
                    (OS_OPT)OS_OPT_POST_FIFO,
                    (OS_ERR*)&os_err);

        OSTimeDlyHMSM(0, 0, 0, 50, OS_OPT_TIME_HMSM_STRICT, &os_err);
    }

}

static void Task2(void *p_arg)
{
    OS_ERR os_err;
    OS_MSG_SIZE msg_size;
    struct OLED_Data_t *recv;

    while (DEF_TRUE)
    {
        uint8_t task2[] = "\rTask 2\n\r";
        UART_Transmit(task2, sizeof(task2));
        recv = OSQPend( (OS_Q*)&queue,
                    (OS_TICK)0,
                    (OS_OPT)OS_OPT_PEND_BLOCKING,
                    (OS_MSG_SIZE*)&msg_size,
                    (CPU_TS*)NULL,
                    (OS_ERR*)&os_err);
        if(oled_data.pos < SSD1306_BOARD_MAX_CHAR)
        {
            SSD1306_Board_PrintChar(recv->pos%15, recv->pos/15, recv->data[recv->pos]);
            oled_data.pos++;
        }
        OSQPost(    (OS_Q*)&queue_pos,
                    (void *)&oled_data,
                    (OS_MSG_SIZE)sizeof(oled_data),
                    (OS_OPT)OS_OPT_POST_FIFO,
                    (OS_ERR*)&os_err);

        OSTimeDlyHMSM(0, 0, 0, 100, OS_OPT_TIME_HMSM_STRICT, &os_err);
    }
}
