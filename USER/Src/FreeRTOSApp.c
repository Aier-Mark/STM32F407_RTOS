
/* FreeRTOS -----------------------------------------------------------------------*/
#include "FreeRTOS.h"
#include "task.h"
/* Includes --------------------------------------------------------------------*/
#include "FreeRTOSApp.h"
#include "./BSP/LED/led.h"
#include "./BSP/LCD/lcd.h"

#include "lvgl.h"
#include "lv_port_disp.h"
#include "lv_port_indev.h"
#include "lv_demo_stress.h"
/* FreeRTOS配置 -----------------------------------------------------------------*/

/* START_TASK 任务 配置
 * 包括: 任务句柄 任务优先级 堆栈大小 创建任务
 */
#define START_TASK_PRIO 1            /* 任务优先级 */
#define START_STK_SIZE  128          /* 任务堆栈大小 */
TaskHandle_t StartTask_Handler;      /* 任务句柄 */
void start_task(void *pvParameters); /* 任务函数 */

/* TASK1 任务 配置
 * 包括: 任务句柄 任务优先级 堆栈大小 创建任务
 */
#define TASK1_PRIO     3        /* 任务优先级 */
#define TASK1_STK_SIZE 1024      /* 任务堆栈大小 */
TaskHandle_t Task1Task_Handler; /* 任务句柄 */
void task1(void *pvParameters); /* 任务函数 */

/* TASK2 任务 配置
 * 包括: 任务句柄 任务优先级 堆栈大小 创建任务
 */
#define TASK2_PRIO     4        /* 任务优先级 */
#define TASK2_STK_SIZE 128      /* 任务堆栈大小 */
TaskHandle_t Task2Task_Handler; /* 任务句柄 */
void task2(void *pvParameters); /* 任务函数 */

/* FreeRTOS配置 End -----------------------------------------------------------------*/

/**
 * @brief FreeRTOS 入口函数
 * @param None
 * @retval None
 */
void freeRTOS_Init(void)
{
    lv_init();            /* lvgl系统初始化 */
    lv_port_disp_init();  /* lvgl显示接口初始化,放在lv_init()的后面 */
    lv_port_indev_init(); /* lvgl输入接口初始化,放在lv_init()的后面 */

    xTaskCreate((TaskFunction_t)start_task,          /* 任务函数 */
                (const char *)"start_task",          /* 任务名称 */
                (uint16_t)START_STK_SIZE,            /* 任务堆栈大小 */
                (void *)NULL,                        /* 传入给任务函数的参数 */
                (UBaseType_t)START_TASK_PRIO,        /* 任务优先级 */
                (TaskHandle_t *)&StartTask_Handler); /* 任务句柄 */

    vTaskStartScheduler(); /* 启动任务调度器 */
}

/**
 * @brief       start_task
 * @param       pvParameters : 传入参数(未用到)
 * @retval      None
 */
void start_task(void *pvParameters)
{
    taskENTER_CRITICAL(); /* 进入临界区 */

    /* 创建任务1 */
    xTaskCreate((TaskFunction_t)task1,               /* 任务函数 */
                (const char *)"task1",               /* 任务名称 */
                (uint16_t)TASK1_STK_SIZE,            /* 任务堆栈大小 */
                (void *)NULL,                        /* 传入给任务函数的参数 */
                (UBaseType_t)TASK1_PRIO,             /* 任务优先级 */
                (TaskHandle_t *)&Task1Task_Handler); /* 任务句柄 */

    /* 创建任务2 */
    xTaskCreate((TaskFunction_t)task2,               /* 任务函数 */
                (const char *)"task2",               /* 任务名称 */
                (uint16_t)TASK2_STK_SIZE,            /* 任务堆栈大小 */
                (void *)NULL,                        /* 传入给任务函数的参数 */
                (UBaseType_t)TASK2_PRIO,             /* 任务优先级 */
                (TaskHandle_t *)&Task2Task_Handler); /* 任务句柄 */

    vTaskDelete(StartTask_Handler); /* 删除开始任务 */
    taskEXIT_CRITICAL();            /* 退出临界区 */
}

/**
 * @brief       task1
 * @param       pvParameters : 传入参数(未用到)
 * @retval      None
 */
void task1(void *pvParameters)
{
    // lcd_show_string(30, 50, 200, 16, 16, "STM32", RED);
    // lcd_show_string(30, 70, 200, 16, 16, "USMART TEST", RED);
    // lcd_show_string(30, 90, 200, 16, 16, "ATOM@ALIENTEK", RED);
    // while (1) {
    //     printf("freertos\r\n");
    //     vTaskDelay(1000); /* 延时1000ticks */
    // }
    
    lv_demo_stress();       /* 测试的demo */
    
    while(1)
    {
        lv_timer_handler(); /* LVGL计时器 */
        vTaskDelay(5);
    }
}

/**
 * @brief       task2
 * @param       pvParameters : 传入参数(未用到)
 * @retval      None
 */
void task2(void *pvParameters)
{
    while (1) {
        LED0_TOGGLE();    /* LED0 红灯闪烁 */
        vTaskDelay(1000); /* 延时1000ticks */
    }
}