/* USER CODE BEGIN Header */
/**
 ******************************************************************************
 * @file           : main.c
 * @brief          : Main program body
 ******************************************************************************
 * @attention
 *
 * Copyright (c) 2026 STMicroelectronics.
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
#include "main.h"
#include "cmsis_os.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "adc_angle.h"
#include "oled.h"
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */

typedef enum
{
    STATE_MAIN_MENU,
} AppState_t;

AppState_t current_state = STATE_MAIN_MENU;

typedef enum
{
    MAIN_PLACE = 0,
    MAIN_GAME = 1,
} MainOption_t;

MainOption_t main_option = MAIN_PLACE;

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
ADC_HandleTypeDef hadc1;

I2C_HandleTypeDef hi2c1;

UART_HandleTypeDef huart1;

/* Definitions for Task_StateMachi */
osThreadId_t Task_StateMachiHandle;
const osThreadAttr_t Task_StateMachi_attributes = {
  .name = "Task_StateMachi",
  .stack_size = 256 * 4,
  .priority = (osPriority_t) osPriorityNormal4,
};
/* Definitions for Task_SensorScan */
osThreadId_t Task_SensorScanHandle;
const osThreadAttr_t Task_SensorScan_attributes = {
  .name = "Task_SensorScan",
  .stack_size = 256 * 4,
  .priority = (osPriority_t) osPriorityNormal3,
};
/* Definitions for Task_AILogic */
osThreadId_t Task_AILogicHandle;
const osThreadAttr_t Task_AILogic_attributes = {
  .name = "Task_AILogic",
  .stack_size = 512 * 4,
  .priority = (osPriority_t) osPriorityNormal2,
};
/* Definitions for Task_ArmControl */
osThreadId_t Task_ArmControlHandle;
const osThreadAttr_t Task_ArmControl_attributes = {
  .name = "Task_ArmControl",
  .stack_size = 256 * 4,
  .priority = (osPriority_t) osPriorityNormal2,
};
/* Definitions for Task_UI */
osThreadId_t Task_UIHandle;
const osThreadAttr_t Task_UI_attributes = {
  .name = "Task_UI",
  .stack_size = 256 * 4,
  .priority = (osPriority_t) osPriorityBelowNormal1,
};
/* Definitions for Task_ReadKEY */
osThreadId_t Task_ReadKEYHandle;
const osThreadAttr_t Task_ReadKEY_attributes = {
  .name = "Task_ReadKEY",
  .stack_size = 256 * 4,
  .priority = (osPriority_t) osPriorityBelowNormal1,
};
/* Definitions for LEDBinarySem */
osSemaphoreId_t LEDBinarySemHandle;
const osSemaphoreAttr_t LEDBinarySem_attributes = {
  .name = "LEDBinarySem"
};
/* USER CODE BEGIN PV */

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_I2C1_Init(void);
static void MX_USART1_UART_Init(void);
static void MX_ADC1_Init(void);
void StateMachine(void *argument);
void StartSensorScan(void *argument);
void StartAILogic(void *argument);
void StartArmControl(void *argument);
void StartUI(void *argument);
void StartReadKEY(void *argument);

/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */

/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{

  /* USER CODE BEGIN 1 */

  /* USER CODE END 1 */

  /* MCU Configuration--------------------------------------------------------*/

  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
  HAL_Init();

  /* USER CODE BEGIN Init */

  /* USER CODE END Init */

  /* Configure the system clock */
  SystemClock_Config();

  /* USER CODE BEGIN SysInit */

  /* USER CODE END SysInit */

  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  MX_I2C1_Init();
  MX_USART1_UART_Init();
  MX_ADC1_Init();
  /* USER CODE BEGIN 2 */
    OLED_Init();
    // OLED_Update();

    OLED_ShowString(0, 0, "Hello World!", OLED_8X16); // 左上角开始显示
    OLED_Update();

  /* USER CODE END 2 */

  /* Init scheduler */
  osKernelInitialize();

  /* USER CODE BEGIN RTOS_MUTEX */
    /* add mutexes, ... */
  /* USER CODE END RTOS_MUTEX */

  /* Create the semaphores(s) */
  /* creation of LEDBinarySem */
  LEDBinarySemHandle = osSemaphoreNew(1, 1, &LEDBinarySem_attributes);

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
  /* creation of Task_StateMachi */
  Task_StateMachiHandle = osThreadNew(StateMachine, NULL, &Task_StateMachi_attributes);

  /* creation of Task_SensorScan */
  Task_SensorScanHandle = osThreadNew(StartSensorScan, NULL, &Task_SensorScan_attributes);

  /* creation of Task_AILogic */
  Task_AILogicHandle = osThreadNew(StartAILogic, NULL, &Task_AILogic_attributes);

  /* creation of Task_ArmControl */
  Task_ArmControlHandle = osThreadNew(StartArmControl, NULL, &Task_ArmControl_attributes);

  /* creation of Task_UI */
  Task_UIHandle = osThreadNew(StartUI, NULL, &Task_UI_attributes);

  /* creation of Task_ReadKEY */
  Task_ReadKEYHandle = osThreadNew(StartReadKEY, NULL, &Task_ReadKEY_attributes);

  /* USER CODE BEGIN RTOS_THREADS */
    /* add threads, ... */
  /* USER CODE END RTOS_THREADS */

  /* USER CODE BEGIN RTOS_EVENTS */
    /* add events, ... */
  /* USER CODE END RTOS_EVENTS */

  /* Start scheduler */
  osKernelStart();

  /* We should never get here as control is now taken by the scheduler */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
    while (1)
    {
    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */
    }
  /* USER CODE END 3 */
}

/**
  * @brief System Clock Configuration
  * @retval None
  */
void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

  /** Configure the main internal regulator output voltage
  */
  __HAL_RCC_PWR_CLK_ENABLE();
  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE1);

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_NONE;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }

  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_HSI;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_0) != HAL_OK)
  {
    Error_Handler();
  }
}

/**
  * @brief ADC1 Initialization Function
  * @param None
  * @retval None
  */
static void MX_ADC1_Init(void)
{

  /* USER CODE BEGIN ADC1_Init 0 */

  /* USER CODE END ADC1_Init 0 */

  ADC_ChannelConfTypeDef sConfig = {0};

  /* USER CODE BEGIN ADC1_Init 1 */

  /* USER CODE END ADC1_Init 1 */

  /** Configure the global features of the ADC (Clock, Resolution, Data Alignment and number of conversion)
  */
  hadc1.Instance = ADC1;
  hadc1.Init.ClockPrescaler = ADC_CLOCK_SYNC_PCLK_DIV2;
  hadc1.Init.Resolution = ADC_RESOLUTION_12B;
  hadc1.Init.ScanConvMode = DISABLE;
  hadc1.Init.ContinuousConvMode = DISABLE;
  hadc1.Init.DiscontinuousConvMode = DISABLE;
  hadc1.Init.ExternalTrigConvEdge = ADC_EXTERNALTRIGCONVEDGE_NONE;
  hadc1.Init.ExternalTrigConv = ADC_SOFTWARE_START;
  hadc1.Init.DataAlign = ADC_DATAALIGN_RIGHT;
  hadc1.Init.NbrOfConversion = 1;
  hadc1.Init.DMAContinuousRequests = DISABLE;
  hadc1.Init.EOCSelection = ADC_EOC_SINGLE_CONV;
  if (HAL_ADC_Init(&hadc1) != HAL_OK)
  {
    Error_Handler();
  }

  /** Configure for the selected ADC regular channel its corresponding rank in the sequencer and its sample time.
  */
  sConfig.Channel = ADC_CHANNEL_0;
  sConfig.Rank = 1;
  sConfig.SamplingTime = ADC_SAMPLETIME_3CYCLES;
  if (HAL_ADC_ConfigChannel(&hadc1, &sConfig) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN ADC1_Init 2 */

  /* USER CODE END ADC1_Init 2 */

}

/**
  * @brief I2C1 Initialization Function
  * @param None
  * @retval None
  */
static void MX_I2C1_Init(void)
{

  /* USER CODE BEGIN I2C1_Init 0 */

  /* USER CODE END I2C1_Init 0 */

  /* USER CODE BEGIN I2C1_Init 1 */

  /* USER CODE END I2C1_Init 1 */
  hi2c1.Instance = I2C1;
  hi2c1.Init.ClockSpeed = 100000;
  hi2c1.Init.DutyCycle = I2C_DUTYCYCLE_2;
  hi2c1.Init.OwnAddress1 = 0;
  hi2c1.Init.AddressingMode = I2C_ADDRESSINGMODE_7BIT;
  hi2c1.Init.DualAddressMode = I2C_DUALADDRESS_DISABLE;
  hi2c1.Init.OwnAddress2 = 0;
  hi2c1.Init.GeneralCallMode = I2C_GENERALCALL_DISABLE;
  hi2c1.Init.NoStretchMode = I2C_NOSTRETCH_DISABLE;
  if (HAL_I2C_Init(&hi2c1) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN I2C1_Init 2 */

  /* USER CODE END I2C1_Init 2 */

}

/**
  * @brief USART1 Initialization Function
  * @param None
  * @retval None
  */
static void MX_USART1_UART_Init(void)
{

  /* USER CODE BEGIN USART1_Init 0 */

  /* USER CODE END USART1_Init 0 */

  /* USER CODE BEGIN USART1_Init 1 */

  /* USER CODE END USART1_Init 1 */
  huart1.Instance = USART1;
  huart1.Init.BaudRate = 115200;
  huart1.Init.WordLength = UART_WORDLENGTH_8B;
  huart1.Init.StopBits = UART_STOPBITS_1;
  huart1.Init.Parity = UART_PARITY_NONE;
  huart1.Init.Mode = UART_MODE_TX_RX;
  huart1.Init.HwFlowCtl = UART_HWCONTROL_NONE;
  huart1.Init.OverSampling = UART_OVERSAMPLING_16;
  if (HAL_UART_Init(&huart1) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN USART1_Init 2 */

  /* USER CODE END USART1_Init 2 */

}

/**
  * @brief GPIO Initialization Function
  * @param None
  * @retval None
  */
static void MX_GPIO_Init(void)
{
  GPIO_InitTypeDef GPIO_InitStruct = {0};
  /* USER CODE BEGIN MX_GPIO_Init_1 */

  /* USER CODE END MX_GPIO_Init_1 */

  /* GPIO Ports Clock Enable */
  __HAL_RCC_GPIOC_CLK_ENABLE();
  __HAL_RCC_GPIOA_CLK_ENABLE();
  __HAL_RCC_GPIOB_CLK_ENABLE();

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(LED_GPIO_Port, LED_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pin : LED_Pin */
  GPIO_InitStruct.Pin = LED_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(LED_GPIO_Port, &GPIO_InitStruct);

  /*Configure GPIO pins : sensor_1_Pin sensor_2_Pin sensor_3_Pin sensor_4_Pin
                           sensor_5_Pin sensor_6_Pin sensor_7_Pin */
  GPIO_InitStruct.Pin = sensor_1_Pin|sensor_2_Pin|sensor_3_Pin|sensor_4_Pin
                          |sensor_5_Pin|sensor_6_Pin|sensor_7_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

  /*Configure GPIO pin : sensor_8_Pin */
  GPIO_InitStruct.Pin = sensor_8_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(sensor_8_GPIO_Port, &GPIO_InitStruct);

  /*Configure GPIO pins : KEY_5_Pin KEY_4_Pin */
  GPIO_InitStruct.Pin = KEY_5_Pin|KEY_4_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_PULLUP;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

  /*Configure GPIO pins : KEY_3_Pin KEY_2_Pin KEY_1_Pin */
  GPIO_InitStruct.Pin = KEY_3_Pin|KEY_2_Pin|KEY_1_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_PULLUP;
  HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

  /* USER CODE BEGIN MX_GPIO_Init_2 */

  /* USER CODE END MX_GPIO_Init_2 */
}

/* USER CODE BEGIN 4 */

/* USER CODE END 4 */

/* USER CODE BEGIN Header_StateMachine */
/**
 * @brief  Function implementing the Task_StateMachi thread.
 * @param  argument: Not used
 * @retval None
 */
/* USER CODE END Header_StateMachine */
void StateMachine(void *argument)
{
  /* USER CODE BEGIN 5 */
    // char msg[32];
    /* Infinite loop */
    for (;;)
    {
        // osSemaphoreRelease(LEDBinarySemHandle);
        // HAL_GPIO_TogglePin(LED_GPIO_Port, LED_Pin);
        // sprintf(msg, "Angle: %d deg\r\n", (int)current_angle_deg);
        osDelay(1000);
    }
  /* USER CODE END 5 */
}

/* USER CODE BEGIN Header_StartSensorScan */
/**
 * @brief Function implementing the Task_SensorScan thread.
 * @param argument: Not used
 * @retval None
 */
/* USER CODE END Header_StartSensorScan */
void StartSensorScan(void *argument)
{
  /* USER CODE BEGIN StartSensorScan */
    /* Infinite loop */
    for (;;)
    {
        GetCurrentAngle();
        osDelay(50);
    }
  /* USER CODE END StartSensorScan */
}

/* USER CODE BEGIN Header_StartAILogic */
/**
 * @brief Function implementing the Task_AILogic thread.
 * @param argument: Not used
 * @retval None
 */
/* USER CODE END Header_StartAILogic */
void StartAILogic(void *argument)
{
  /* USER CODE BEGIN StartAILogic */
    /* Infinite loop */
    for (;;)
    {
        osDelay(1);
    }
  /* USER CODE END StartAILogic */
}

/* USER CODE BEGIN Header_StartArmControl */
/**
 * @brief Function implementing the Task_ArmControl thread.
 * @param argument: Not used
 * @retval None
 */
/* USER CODE END Header_StartArmControl */
void StartArmControl(void *argument)
{
  /* USER CODE BEGIN StartArmControl */
    /* Infinite loop */
    for (;;)
    {
        osDelay(1);
    }
  /* USER CODE END StartArmControl */
}

/* USER CODE BEGIN Header_StartUI */
/**
 * @brief Function implementing the Task_UI thread.
 * @param argument: Not used
 * @retval None
 */
/* USER CODE END Header_StartUI */
void StartUI(void *argument)
{
  /* USER CODE BEGIN StartUI */
    OLED_Clear();
    /* Infinite loop */
    for (;;)
    {
        // HAL_GPIO_TogglePin(LED_GPIO_Port, LED_Pin);

        // 显示主菜单
        OLED_Clear();
        OLED_ShowString(0, 0, "=== Chess Robot ===", OLED_8X16);
        OLED_ShowString(0, 20, main_option == MAIN_PLACE ? "-> Place Chess" : "   Place Chess", OLED_8X16);
        OLED_ShowString(0, 35, main_option == MAIN_GAME ? "-> Play Game" : "   Play Game", OLED_8X16);
        OLED_Update();

        osDelay(200);
    }
  /* USER CODE END StartUI */
}

/* USER CODE BEGIN Header_StartReadKEY */
/**
 * @brief Function implementing the Task_ReadKEY thread.
 * @param argument: Not used
 * @retval None
 */
/* USER CODE END Header_StartReadKEY */
void StartReadKEY(void *argument)
{
  /* USER CODE BEGIN StartReadKEY */
  uint8_t last_state = 1;
  uint8_t cur_state;
  /* Infinite loop */
  for (;;)
  {
    cur_state = (HAL_GPIO_ReadPin(KEY_5_GPIO_Port, KEY_5_Pin) == GPIO_PIN_RESET) ? 0 : 1;
    if (last_state == 1 && cur_state == 0)
    {
      osDelay(20);
      if (HAL_GPIO_ReadPin(KEY_5_GPIO_Port, KEY_5_Pin) == GPIO_PIN_RESET)
      {
        HAL_GPIO_TogglePin(LED_GPIO_Port, LED_Pin);
        while (HAL_GPIO_ReadPin(KEY_5_GPIO_Port, KEY_5_Pin) == GPIO_PIN_RESET)
          osDelay(10);
      }
    }
    last_state = cur_state;
    osDelay(20);
  }
  /* USER CODE END StartReadKEY */
}

/**
  * @brief  Period elapsed callback in non blocking mode
  * @note   This function is called  when TIM11 interrupt took place, inside
  * HAL_TIM_IRQHandler(). It makes a direct call to HAL_IncTick() to increment
  * a global variable "uwTick" used as application time base.
  * @param  htim : TIM handle
  * @retval None
  */
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
  /* USER CODE BEGIN Callback 0 */

  /* USER CODE END Callback 0 */
  if (htim->Instance == TIM11)
  {
    HAL_IncTick();
  }
  /* USER CODE BEGIN Callback 1 */

  /* USER CODE END Callback 1 */
}

/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
void Error_Handler(void)
{
  /* USER CODE BEGIN Error_Handler_Debug */
    /* User can add his own implementation to report the HAL error return state */
    __disable_irq();
    while (1)
    {
    }
  /* USER CODE END Error_Handler_Debug */
}
#ifdef USE_FULL_ASSERT
/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t *file, uint32_t line)
{
  /* USER CODE BEGIN 6 */
    /* User can add his own implementation to report the file name and line number,
       ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */
