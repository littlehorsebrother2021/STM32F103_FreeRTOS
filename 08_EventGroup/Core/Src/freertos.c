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
#include "event_groups.h"
#include "string.h"
#include "stdio.h"
#include "usart.h"
#include "key.h"

/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
#define BITMASK_KEY_LEFT  (0x01 << 2)  // 0x01 µÈÐ§ÓÚ 0b00000001
#define BITMASK_KEY_RIGHT (0x01 << 0)
/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
/* USER CODE BEGIN Variables */

/* USER CODE END Variables */
/* Definitions for Task_LED */
osThreadId_t Task_LEDHandle;
const osThreadAttr_t Task_LED_attributes = {
  .name = "Task_LED",
  .stack_size = 128 * 4,
  .priority = (osPriority_t) osPriorityBelowNormal,
};
/* Definitions for Task_Buzzer */
osThreadId_t Task_BuzzerHandle;
const osThreadAttr_t Task_Buzzer_attributes = {
  .name = "Task_Buzzer",
  .stack_size = 128 * 4,
  .priority = (osPriority_t) osPriorityLow,
};
/* Definitions for Task_ScanKeys */
osThreadId_t Task_ScanKeysHandle;
const osThreadAttr_t Task_ScanKeys_attributes = {
  .name = "Task_ScanKeys",
  .stack_size = 128 * 4,
  .priority = (osPriority_t) osPriorityAboveNormal,
};
/* Definitions for EventGroup */
osEventFlagsId_t EventGroupHandle;
const osEventFlagsAttr_t EventGroup_attributes = {
  .name = "EventGroup"
};

/* Private function prototypes -----------------------------------------------*/
/* USER CODE BEGIN FunctionPrototypes */

/* USER CODE END FunctionPrototypes */

void AppTask_LED(void *argument);
void AppTask_Buzzer(void *argument);
void AppTask_ScanKeys(void *argument);

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

  /* USER CODE BEGIN RTOS_QUEUES */
  /* add queues, ... */
  /* USER CODE END RTOS_QUEUES */

  /* Create the thread(s) */
  /* creation of Task_LED */
  Task_LEDHandle = osThreadNew(AppTask_LED, NULL, &Task_LED_attributes);

  /* creation of Task_Buzzer */
  Task_BuzzerHandle = osThreadNew(AppTask_Buzzer, NULL, &Task_Buzzer_attributes);

  /* creation of Task_ScanKeys */
  Task_ScanKeysHandle = osThreadNew(AppTask_ScanKeys, NULL, &Task_ScanKeys_attributes);

  /* USER CODE BEGIN RTOS_THREADS */
  /* add threads, ... */
  /* USER CODE END RTOS_THREADS */

  /* Create the event(s) */
  /* creation of EventGroup */
  EventGroupHandle = osEventFlagsNew(&EventGroup_attributes);

  /* USER CODE BEGIN RTOS_EVENTS */
  /* add events, ... */
  /* USER CODE END RTOS_EVENTS */

}

/* USER CODE BEGIN Header_AppTask_LED */
/**
  * @brief  Function implementing the Task_LED thread.
  * @param  argument: Not used
  * @retval None
  */
/* USER CODE END Header_AppTask_LED */
void AppTask_LED(void *argument)
{
  /* USER CODE BEGIN AppTask_LED */
  /* Infinite loop */
  for(;;)
  {
    xEventGroupWaitBits(EventGroupHandle, BITMASK_KEY_LEFT | BITMASK_KEY_RIGHT, pdTRUE, pdTRUE, portMAX_DELAY);
    for ( int i = 0; i < 5; i++ )
    {
      HAL_GPIO_TogglePin(LED0_GPIO_Port, LED0_Pin);
      osDelay(500);
    }
  }
  /* USER CODE END AppTask_LED */
}

/* USER CODE BEGIN Header_AppTask_Buzzer */
/**
* @brief Function implementing the Task_Buzzer thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_AppTask_Buzzer */
void AppTask_Buzzer(void *argument)
{
  /* USER CODE BEGIN AppTask_Buzzer */
  /* Infinite loop */
  for(;;)
  {
    xEventGroupWaitBits(EventGroupHandle, BITMASK_KEY_LEFT | BITMASK_KEY_RIGHT, pdTRUE, pdTRUE, portMAX_DELAY);
    for ( int i = 0; i < 5; i++ )
    {
      HAL_GPIO_TogglePin(LED1_GPIO_Port, LED1_Pin);
      osDelay(500);
    }
    osDelay(1);
  }
  /* USER CODE END AppTask_Buzzer */
}

/* USER CODE BEGIN Header_AppTask_ScanKeys */
/**
* @brief Function implementing the Task_ScanKeys thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_AppTask_ScanKeys */
void AppTask_ScanKeys(void *argument)
{
  /* USER CODE BEGIN AppTask_ScanKeys */
  char temp_str[20];
  KEY_TYPE KEY = KEY_NONE;
  /* Infinite loop */
  for(;;)
  {
    sprintf(temp_str, "Current event bits = %02X\r\n", xEventGroupGetBits(EventGroupHandle));
    HAL_UART_Transmit(&huart1, (uint8_t*)temp_str, strlen(temp_str), 100);

    if ( HAL_GPIO_ReadPin(KEYLEFT_GPIO_PORT, KEYLEFT_GPIO_PIN) == GPIO_PIN_RESET )
    {
      KEY = KEY_LEFT;
    }else if ( HAL_GPIO_ReadPin(KEYRIGHT_GPIO_PORT, KEYRIGHT_GPIO_PIN) == GPIO_PIN_RESET )
    {
      KEY = KEY_RIGHT;
    }else if ( HAL_GPIO_ReadPin(KEYDOWN_GPIO_PORT, KEYDOWN_GPIO_PIN) == GPIO_PIN_RESET )
    {
      KEY = KEY_DOWN;
    }
    if ( KEY != KEY_NONE )
    {
      switch (KEY)
      {
        case KEY_LEFT:
          xEventGroupSetBits(EventGroupHandle, BITMASK_KEY_LEFT);
        break;
        case KEY_RIGHT:
          xEventGroupSetBits(EventGroupHandle, BITMASK_KEY_RIGHT);
        break;  
        case KEY_DOWN:
          xEventGroupClearBits(EventGroupHandle, BITMASK_KEY_LEFT | BITMASK_KEY_RIGHT);
        break;
        default:
        break;
      }
      vTaskDelay(pdMS_TO_TICKS(200));
    }else
    {
      vTaskDelay(pdMS_TO_TICKS(10));
    }
    //osDelay(1);
  }
  /* USER CODE END AppTask_ScanKeys */
}

/* Private application code --------------------------------------------------*/
/* USER CODE BEGIN Application */

/* USER CODE END Application */

