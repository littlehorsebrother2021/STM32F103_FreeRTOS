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
#include "string.h"
#include "semphr.h"
#include "usart.h"
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
/* Definitions for Task_High */
osThreadId_t Task_HighHandle;
const osThreadAttr_t Task_High_attributes = {
  .name = "Task_High",
  .stack_size = 128 * 4,
  .priority = (osPriority_t) osPriorityHigh,
};
/* Definitions for Task_Middle */
osThreadId_t Task_MiddleHandle;
const osThreadAttr_t Task_Middle_attributes = {
  .name = "Task_Middle",
  .stack_size = 128 * 4,
  .priority = (osPriority_t) osPriorityNormal,
};
/* Definitions for Task_Low */
osThreadId_t Task_LowHandle;
const osThreadAttr_t Task_Low_attributes = {
  .name = "Task_Low",
  .stack_size = 128 * 4,
  .priority = (osPriority_t) osPriorityLow,
};
/* Definitions for token */
osSemaphoreId_t tokenHandle;
const osSemaphoreAttr_t token_attributes = {
  .name = "token"
};

/* Private function prototypes -----------------------------------------------*/
/* USER CODE BEGIN FunctionPrototypes */

/* USER CODE END FunctionPrototypes */

void AppTask_High(void *argument);
void AppTask_Middle(void *argument);
void AppTask_Low(void *argument);

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
  /* creation of token */
  tokenHandle = osSemaphoreNew(1, 1, &token_attributes);

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
  /* creation of Task_High */
  Task_HighHandle = osThreadNew(AppTask_High, NULL, &Task_High_attributes);

  /* creation of Task_Middle */
  Task_MiddleHandle = osThreadNew(AppTask_Middle, NULL, &Task_Middle_attributes);

  /* creation of Task_Low */
  Task_LowHandle = osThreadNew(AppTask_Low, NULL, &Task_Low_attributes);

  /* USER CODE BEGIN RTOS_THREADS */
  /* add threads, ... */
  /* USER CODE END RTOS_THREADS */

  /* USER CODE BEGIN RTOS_EVENTS */
  /* add events, ... */
  /* USER CODE END RTOS_EVENTS */

}

/* USER CODE BEGIN Header_AppTask_High */
/**
  * @brief  Function implementing the Task_High thread.
  * @param  argument: Not used
  * @retval None
  */
/* USER CODE END Header_AppTask_High */
void AppTask_High(void *argument)
{
  /* USER CODE BEGIN AppTask_High */
  char strHigh[] = "Task_High get token \r\n";

  /* Infinite loop */
  for(;;)
  {
    if (xSemaphoreTake(tokenHandle, portMAX_DELAY) == pdTRUE)
    {
      HAL_UART_Transmit(&huart1, (uint8_t*)strHigh, strlen(strHigh), 300);
      HAL_Delay(10);
      xSemaphoreGive(tokenHandle);
    }
    vTaskDelay(pdMS_TO_TICKS(500));
  }
  /* USER CODE END AppTask_High */
}

/* USER CODE BEGIN Header_AppTask_Middle */
/**
* @brief Function implementing the Task_Middle thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_AppTask_Middle */
void AppTask_Middle(void *argument)
{
  /* USER CODE BEGIN AppTask_Middle */
  char strMid[] = "Task_Middle is running \r\n";
  /* Infinite loop */
  for(;;)
  {
    HAL_UART_Transmit(&huart1, (uint8_t*)strMid, strlen(strMid), 300);
    HAL_Delay(10);
    vTaskDelay(pdMS_TO_TICKS(500));
    //osDelay(1);
  }
  /* USER CODE END AppTask_Middle */
}

/* USER CODE BEGIN Header_AppTask_Low */
/**
* @brief Function implementing the Task_Low thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_AppTask_Low */
void AppTask_Low(void *argument)
{
  /* USER CODE BEGIN AppTask_Low */
  char str1[] = "Task_Low take it \r\n";
  char str2[] = "Task_Low give it \r\n";
  /* Infinite loop */
  for(;;)
  {
    if (xSemaphoreTake(tokenHandle, pdMS_TO_TICKS(200)) == pdTRUE)
    {
      HAL_UART_Transmit(&huart1, (uint8_t*)str1, strlen(str1), 300);
      HAL_Delay(1000);//假设程序运行1s时间
      HAL_UART_Transmit(&huart1, (uint8_t*)str2, strlen(str2), 300);
      HAL_Delay(10);
      xSemaphoreGive(tokenHandle);
    }
    vTaskDelay(pdMS_TO_TICKS(20));

    //osDelay(1);
  }
  /* USER CODE END AppTask_Low */
}

/* Private application code --------------------------------------------------*/
/* USER CODE BEGIN Application */

/* USER CODE END Application */

