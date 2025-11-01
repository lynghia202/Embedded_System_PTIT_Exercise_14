#include "stm32f10x.h"
#include "FreeRTOS.h"
#include "task.h"
#include "event_groups.h"
#include <stdio.h>

EventGroupHandle_t xEventGroup;

#define EVENT_BIT_TASK_A   (1 << 0)
#define EVENT_BIT_TASK_B   (1 << 1)
#define EVENT_BIT_TASK_C   (1 << 2)


void USART1_Init(void);
void USART1_SendChar(char c);
int fputc(int ch, FILE *f);
void TaskController(void *pvParameters);
void TaskA(void *pvParameters);
void TaskB(void *pvParameters);
void TaskC(void *pvParameters);


int main(void)
{
    SystemInit();
    USART1_Init();

    printf("=== FreeRTOS EventGroup Demo ===\r\n");

    xEventGroup = xEventGroupCreate();

    xTaskCreate(TaskController, "Controller", 256, NULL, 2, NULL);
    xTaskCreate(TaskA, "TaskA", 256, NULL, 1, NULL);
    xTaskCreate(TaskB, "TaskB", 256, NULL, 1, NULL);
    xTaskCreate(TaskC, "TaskC", 256, NULL, 1, NULL);

    vTaskStartScheduler();

    while(1);
}


void USART1_Init(void)
{
    GPIO_InitTypeDef GPIO_InitStruct;
    USART_InitTypeDef USART_InitStruct;

    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA | RCC_APB2Periph_USART1, ENABLE);

    GPIO_InitStruct.GPIO_Pin = GPIO_Pin_9;
    GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AF_PP;
    GPIO_Init(GPIOA, &GPIO_InitStruct);


    GPIO_InitStruct.GPIO_Pin = GPIO_Pin_10;
    GPIO_InitStruct.GPIO_Mode = GPIO_Mode_IN_FLOATING;
    GPIO_Init(GPIOA, &GPIO_InitStruct);

    USART_InitStruct.USART_BaudRate = 115200;
    USART_InitStruct.USART_WordLength = USART_WordLength_8b;
    USART_InitStruct.USART_StopBits = USART_StopBits_1;
    USART_InitStruct.USART_Parity = USART_Parity_No;
    USART_InitStruct.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
    USART_InitStruct.USART_Mode = USART_Mode_Tx | USART_Mode_Rx;

    USART_Init(USART1, &USART_InitStruct);
    USART_Cmd(USART1, ENABLE);
}


void USART1_SendChar(char c)
{
    while(USART_GetFlagStatus(USART1, USART_FLAG_TXE) == RESET);
    USART_SendData(USART1, c);
}

int fputc(int ch, FILE *f)
{
    USART1_SendChar(ch);
    return ch;
}

//Task dieu khien
void TaskController(void *pvParameters)
{
    while(1)
    {
        printf("\r\n[Controller] Kich hoat Task A...\r\n");
        xEventGroupSetBits(xEventGroup, EVENT_BIT_TASK_A);
        vTaskDelay(pdMS_TO_TICKS(2000));

        printf("[Controller] Kich hoat Task B...\r\n");
        xEventGroupSetBits(xEventGroup, EVENT_BIT_TASK_B);
        vTaskDelay(pdMS_TO_TICKS(2000));

        printf("[Controller] Kich hoat Task C...\r\n");
        xEventGroupSetBits(xEventGroup, EVENT_BIT_TASK_C);
        vTaskDelay(pdMS_TO_TICKS(2000));

        printf("[Controller] Kich hoat dong thoi ca 3 task A+B+C...\r\n");
        xEventGroupSetBits(xEventGroup,EVENT_BIT_TASK_A | EVENT_BIT_TASK_B | EVENT_BIT_TASK_C);
        vTaskDelay(pdMS_TO_TICKS(3000));

        xEventGroupClearBits(xEventGroup, EVENT_BIT_TASK_A | EVENT_BIT_TASK_B | EVENT_BIT_TASK_C);
    }
}


void TaskA(void *pvParameters)
{
    EventBits_t uxBits;
    while(1)
    {
       uxBits = xEventGroupWaitBits(xEventGroup, EVENT_BIT_TASK_A, pdTRUE,pdFALSE, portMAX_DELAY);
        if (uxBits & EVENT_BIT_TASK_A)
            printf("Task A: Đang chay...\r\n");
    }
}

void TaskB(void *pvParameters)
{
    EventBits_t uxBits;
    while(1)
    {
        uxBits = xEventGroupWaitBits(xEventGroup, EVENT_BIT_TASK_B, pdTRUE,pdFALSE, portMAX_DELAY);
        if (uxBits & EVENT_BIT_TASK_B)
            printf("Task B: Đang chay...\r\n");
    }
}

void TaskC(void *pvParameters)
{
    EventBits_t uxBits;
    while(1)
    {
        uxBits = xEventGroupWaitBits(xEventGroup, EVENT_BIT_TASK_C, pdTRUE,pdFALSE, portMAX_DELAY);
        if (uxBits & EVENT_BIT_TASK_C)
            printf("Task C: Đang chay...\r\n");
    }
}
