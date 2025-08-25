/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * File Name          : freertos.c
  * Description        : Code for freertos applications
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2025 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */
/* USER CODE END Header */

/* Includes ------------------------------------------------------------------*/
#include "FreeRTOS.h"
#include "task.h"
#include "main.h"
#include "cmsis_os.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "stdio.h"
#include "key.h"
#include "usart.h"
#include "tim.h"
#include "semphr.h"
#include "string.h"
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
/* USER CODE BEGIN Variables */

/* USER CODE END Variables */
/* Definitions for Task_Checkin */
osThreadId_t Task_CheckinHandle;
const osThreadAttr_t Task_Checkin_attributes = {
  .name = "Task_Checkin",
  .stack_size = 128 * 4,
  .priority = (osPriority_t) osPriorityNormal,
};
/* Definitions for Sem_Tables */
osSemaphoreId_t Sem_TablesHandle;
const osSemaphoreAttr_t Sem_Tables_attributes = {
  .name = "Sem_Tables"
};

/* Private function prototypes -----------------------------------------------*/
/* USER CODE BEGIN FunctionPrototypes */

/* USER CODE END FunctionPrototypes */

void AppTask_Checkin(void *argument);

void MX_FREERTOS_Init(void); /* (MISRA C 2004 rule 8.1) */

/**
  * @brief  FreeRTOS initialization
  * @param  None
  * @retval None
  */
void MX_FREERTOS_Init(void) {
  /* USER CODE BEGIN Init */

  /* USER CODE END Init */

  /* USER CODE BEGIN RTOS_MUTEX */
  /* add mutexes, ... */
  /* USER CODE END RTOS_MUTEX */

  /* Create the semaphores(s) */
  /* creation of Sem_Tables */
  Sem_TablesHandle = osSemaphoreNew(5, 0, &Sem_Tables_attributes);

  /* USER CODE BEGIN RTOS_SEMAPHORES */
  /* add semaphores, ... */
  /* USER CODE END RTOS_SEMAPHORES */

  /* USER CODE BEGIN RTOS_TIMERS */
  /* start timers, add new ones, ... */
  /* USER CODE END RTOS_TIMERS */

  /* USER CODE BEGIN RTOS_QUEUES */
  /* add queues, ... */
  /* USER CODE END RTOS_QUEUES */

  /* Create the thread(s) */
  /* creation of Task_Checkin */
  Task_CheckinHandle = osThreadNew(AppTask_Checkin, NULL, &Task_Checkin_attributes);

  /* USER CODE BEGIN RTOS_THREADS */
  /* add threads, ... */
  /* USER CODE END RTOS_THREADS */

  /* USER CODE BEGIN RTOS_EVENTS */
  /* add events, ... */
  /* USER CODE END RTOS_EVENTS */

}

/* USER CODE BEGIN Header_AppTask_Checkin */
/**
  * @brief  Function implementing the Task_Checkin thread.
  * @param  argument: Not used
  * @retval None
  */
/* USER CODE END Header_AppTask_Checkin */
void AppTask_Checkin(void *argument)
{
  /* USER CODE BEGIN AppTask_Checkin */
  char temp_str[50];
  //sprintf(temp_str, "Hello World!\r\n");
  /* Infinite loop */
  for(;;)
  {
    if (HAL_GPIO_ReadPin(KEYUP_GPIO_PORT, KEYUP_GPIO_PIN) == GPIO_PIN_SET)
    {
      if (xSemaphoreTake(Sem_TablesHandle, pdMS_TO_TICKS(100)) == pdTRUE)
      {
        sprintf(temp_str, "Success to Check in!\r\n");
        HAL_UART_Transmit(&huart1, (uint8_t *)temp_str, strlen(temp_str), 1000);
      }else
      {
        sprintf(temp_str, "Failed to Check in!\r\n");
        HAL_UART_Transmit(&huart1, (uint8_t *)temp_str, strlen(temp_str), 1000);
      }
      vTaskDelay(pdMS_TO_TICKS(300));
    }
    sprintf(temp_str, "Available tables = %d \r\n",uxSemaphoreGetCount(Sem_TablesHandle));
    HAL_UART_Transmit(&huart1, (uint8_t *)temp_str, strlen(temp_str), 1000);
    vTaskDelay(pdMS_TO_TICKS(10));
    //osDelay(1);
  }
  /* USER CODE END AppTask_Checkin */
}

/* Private application code --------------------------------------------------*/
/* USER CODE BEGIN Application */
//编写定时器中断回调函数程序
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
  /* USER CODE BEGIN Callback 0 */

  /* USER CODE END Callback 0 */
  if (htim->Instance == TIM6)
  {
    HAL_IncTick();
  }
  /* USER CODE BEGIN Callback 1 */
  if (htim == &htim3)
  {
    BaseType_t hightaskWoken = pdFALSE;
    if (Sem_TablesHandle != NULL)
    {
      xSemaphoreGiveFromISR(Sem_TablesHandle, &hightaskWoken);
      //重新调度任务优先级
      portYIELD_FROM_ISR(hightaskWoken);
    }
  }
  /* USER CODE END Callback 1 */
}

/* USER CODE END Application */

