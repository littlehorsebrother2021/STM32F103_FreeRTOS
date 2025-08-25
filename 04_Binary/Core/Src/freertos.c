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
#include "semphr.h"
#include "adc.h"
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
uint16_t adc_value = 0;
/* USER CODE END Variables */
/* Definitions for Task_Idle */
osThreadId_t Task_IdleHandle;
const osThreadAttr_t Task_Idle_attributes = {
  .name = "Task_Idle",
  .stack_size = 128 * 4,
  .priority = (osPriority_t) osPriorityBelowNormal,
};
/* Definitions for Task_Scankeys */
osThreadId_t Task_ScankeysHandle;
const osThreadAttr_t Task_Scankeys_attributes = {
  .name = "Task_Scankeys",
  .stack_size = 128 * 4,
  .priority = (osPriority_t) osPriorityNormal,
};
/* Definitions for Queue_Keys */
osMessageQueueId_t Queue_KeysHandle;
const osMessageQueueAttr_t Queue_Keys_attributes = {
  .name = "Queue_Keys"
};
/* Definitions for BinSem_DataReady */
osSemaphoreId_t BinSem_DataReadyHandle;
const osSemaphoreAttr_t BinSem_DataReady_attributes = {
  .name = "BinSem_DataReady"
};

/* Private function prototypes -----------------------------------------------*/
/* USER CODE BEGIN FunctionPrototypes */

/* USER CODE END FunctionPrototypes */

void AppTask_Idle(void *argument);
void AppTask_Scankeys(void *argument);

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
  /* creation of BinSem_DataReady */
  BinSem_DataReadyHandle = osSemaphoreNew(1, 1, &BinSem_DataReady_attributes);

  /* USER CODE BEGIN RTOS_SEMAPHORES */
  /* add semaphores, ... */
  /* USER CODE END RTOS_SEMAPHORES */

  /* USER CODE BEGIN RTOS_TIMERS */
  /* start timers, add new ones, ... */
  /* USER CODE END RTOS_TIMERS */

  /* Create the queue(s) */
  /* creation of Queue_Keys */
  Queue_KeysHandle = osMessageQueueNew (10, sizeof(uint8_t), &Queue_Keys_attributes);

  /* USER CODE BEGIN RTOS_QUEUES */
  /* add queues, ... */
  /* USER CODE END RTOS_QUEUES */

  /* Create the thread(s) */
  /* creation of Task_Idle */
  Task_IdleHandle = osThreadNew(AppTask_Idle, NULL, &Task_Idle_attributes);

  /* creation of Task_Scankeys */
  Task_ScankeysHandle = osThreadNew(AppTask_Scankeys, NULL, &Task_Scankeys_attributes);

  /* USER CODE BEGIN RTOS_THREADS */
  /* add threads, ... */
  /* USER CODE END RTOS_THREADS */

  /* USER CODE BEGIN RTOS_EVENTS */
  /* add events, ... */
  /* USER CODE END RTOS_EVENTS */

}

/* USER CODE BEGIN Header_AppTask_Idle */
/**
  * @brief  Function implementing the Task_Idle thread.
  * @param  argument: Not used
  * @retval None
  */
/* USER CODE END Header_AppTask_Idle */
void AppTask_Idle(void *argument)
{
  /* USER CODE BEGIN AppTask_Idle */
  uint8_t temp_str[50];
  /* Infinite loop */
  for(;;)
  {
    if  (xSemaphoreTake(BinSem_DataReadyHandle, portMAX_DELAY) == pdTRUE)
    {
      sprintf(temp_str, "ADC value: %d\r\n", adc_value);
      HAL_UART_Transmit(&huart1, (uint8_t*)temp_str, strlen(temp_str), 1000);

      sprintf(temp_str, "Voltage: %f\r\n", adc_value/4095.0*3.3);
      HAL_UART_Transmit(&huart1, (uint8_t*)temp_str, strlen(temp_str), 1000);

      osDelay(100);
    }else
    {
      osDelay(10);
    }
    //osDelay(1);
  }
  /* USER CODE END AppTask_Idle */
}

/* USER CODE BEGIN Header_AppTask_Scankeys */
/**
* @brief Function implementing the Task_Scankeys thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_AppTask_Scankeys */
void AppTask_Scankeys(void *argument)
{
  /* USER CODE BEGIN AppTask_Scankeys */
  /* Infinite loop */
  for(;;)
  {
    osDelay(1);
  }
  /* USER CODE END AppTask_Scankeys */
}

/* Private application code --------------------------------------------------*/
/* USER CODE BEGIN Application */
void HAL_ADC_ConvCpltCallback(ADC_HandleTypeDef* hadc)
{
  if (hadc == &hadc1)
  {
    adc_value = HAL_ADC_GetValue(hadc);
    BaseType_t highTaskWoken = pdFALSE;
    if (BinSem_DataReadyHandle != NULL)
    {
      xSemaphoreGiveFromISR(BinSem_DataReadyHandle, &highTaskWoken);
      portYIELD_FROM_ISR(highTaskWoken);
    }
    //osSemaphoreRelease(BinSem_DataReadyHandle);
  }

}
/* USER CODE END Application */

