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
#include "queue.h"
#include "key.h"
#include "usart.h"
#include "string.h"
#include "stdint.h"
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
  uint8_t tempStr[30];
  KEY_TYPE keyvalue = KEY_NONE;
  /* Infinite loop */
  for(;;)
  {
    
    if (xQueueReceive(Queue_KeysHandle, &keyvalue, pdMS_TO_TICKS(50)) != pdTRUE)
    {
      continue;
    }


//    sprintf(tempStr, "uxQueueMessagesWaiting = %d\r\n", uxQueueMessagesWaiting(Queue_KeysHandle));
//    HAL_UART_Transmit(&huart1, (uint8_t*)tempStr, strlen(tempStr), 1000);


//    sprintf(tempStr, "uxQueueSpacesAvailable = %d\r\n", uxQueueSpacesAvailable(Queue_KeysHandle));
//    HAL_UART_Transmit(&huart1, (uint8_t*)tempStr, strlen(tempStr), 1000);

    switch (keyvalue)
    {
      case KEY_UP:
        sprintf(tempStr, "keyvalue = KEY_UP \r\n");
        HAL_UART_Transmit(&huart1, (uint8_t*)tempStr, strlen(tempStr), 1000);
        break;
      case KEY_DOWN:
        sprintf(tempStr, "keyvalue = KEY_DOWN \r\n");
        HAL_UART_Transmit(&huart1, (uint8_t*)tempStr, strlen(tempStr), 1000);
        break;
      case KEY_LEFT:
        sprintf(tempStr, "keyvalue = KEY_LEFT \r\n");
        HAL_UART_Transmit(&huart1, (uint8_t*)tempStr, strlen(tempStr), 1000);
        break;
      case KEY_RIGHT:
        sprintf(tempStr, "keyvalue = KEY_RIGHT \r\n");
        HAL_UART_Transmit(&huart1, (uint8_t*)tempStr, strlen(tempStr), 1000);
        break;
    }
    osDelay(1);
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
  GPIO_PinState keyState = GPIO_PIN_RESET;
  KEY_TYPE key = KEY_NONE;
  /* Infinite loop */
  for(;;)
  {
    key = KEY_NONE;
    keyState = HAL_GPIO_ReadPin(KEYUP_GPIO_PORT, KEYUP_GPIO_PIN);
    if (keyState == GPIO_PIN_SET)
    {
      key = KEY_UP;
    }

    keyState = HAL_GPIO_ReadPin(KEYDOWN_GPIO_PORT, KEYDOWN_GPIO_PIN);
    if (keyState == GPIO_PIN_RESET)
    {
      key = KEY_DOWN;
    }

    keyState = HAL_GPIO_ReadPin(KEYLEFT_GPIO_PORT, KEYLEFT_GPIO_PIN);
    if (keyState == GPIO_PIN_RESET)
    {
      key = KEY_LEFT;
    }

    keyState = HAL_GPIO_ReadPin(KEYRIGHT_GPIO_PORT, KEYRIGHT_GPIO_PIN);
    if (keyState == GPIO_PIN_RESET)
    {
      key = KEY_RIGHT;
    }



    if(key != KEY_NONE)
    {
      BaseType_t err =  xQueueSendToBack(Queue_KeysHandle, &key, pdMS_TO_TICKS(50));
      if (err == errQUEUE_FULL)
      {
        xQueueReset(Queue_KeysHandle);
      }
      vTaskDelay(pdMS_TO_TICKS(300));  
    }else
    {
      vTaskDelay(pdMS_TO_TICKS(10));
    }
    //osDelay(1);
  }
  /* USER CODE END AppTask_Scankeys */
}

/* Private application code --------------------------------------------------*/
/* USER CODE BEGIN Application */

/* USER CODE END Application */

