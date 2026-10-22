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


/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "../../Drivers/SSD1306/ssd1306.h"
#include "../../Drivers/SSD1306/ssd1306_fonts.h"
#include <stdio.h>
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
I2C_HandleTypeDef hi2c1;

/* USER CODE BEGIN PV */

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_I2C1_Init(void);
/* USER CODE BEGIN PFP */
typedef enum {
    GO_SOUTH, WAIT_SOUTH, CLEAR_SOUTH,
    GO_WEST,  WAIT_WEST,  CLEAR_WEST,
    GO_WALK,  WAIT_WALK,  CLEAR_WALK
} FSM_State;

typedef enum {GREEN, YELLOW, RED} Led;

void Display_Traffic_UI(Led south, Led west, Led walk, uint32_t time);

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */
void Process_Traffic_FSM(uint32_t led_input,
			Led *south_led, Led *west_led, Led *walk_led,
			uint32_t *wait_time) {

    static FSM_State current_state = GO_WALK;

    switch(current_state) {

        case GO_SOUTH:
            *south_led = GREEN; *west_led = RED; *walk_led = RED;
            *wait_time = 12000; // 12s Green
            if ((led_input & 2) || (led_input & 1)) {
                current_state = WAIT_SOUTH;
            }
            break;

        case WAIT_SOUTH:
            *south_led = YELLOW; *west_led = RED; *walk_led = RED;
            *wait_time = 3000; // 3s Yellow
            current_state = CLEAR_SOUTH;
            break;

        case CLEAR_SOUTH:
            *south_led = RED; *west_led = RED; *walk_led = RED;
            *wait_time = 3000; // 3s Clearance

            if (led_input & 2)      current_state = GO_WEST;
            else if (led_input & 1) current_state = GO_WALK;
            else if (led_input & 4) current_state = GO_SOUTH;
            else                    current_state = GO_WALK;
            break;

        case GO_WEST:
            *south_led = RED; *west_led = GREEN; *walk_led = RED;
            *wait_time = 12000; // 12s Green
            if ((led_input & 4) || (led_input & 1)) {
                current_state = WAIT_WEST;
            }
            break;

        case WAIT_WEST:
            *south_led = RED; *west_led = YELLOW; *walk_led = RED;
            *wait_time = 3000; // 3s Yellow
            current_state = CLEAR_WEST;
            break;

        case CLEAR_WEST:
            *south_led = RED; *west_led = RED; *walk_led = RED;
            *wait_time = 3000; // 3s Clearance

            if (led_input & 1)      current_state = GO_WALK;
            else if (led_input & 4) current_state = GO_SOUTH;
            else if (led_input & 2) current_state = GO_WEST;
            else                    current_state = GO_WALK;
            break;

        case GO_WALK:
            *south_led = RED; *west_led = RED; *walk_led = GREEN;
            *wait_time = 12000; // 12s Cross
            if ((led_input & 4) || (led_input & 2)) {
                current_state = WAIT_WALK;
            }
            break;

        case WAIT_WALK:
            *south_led = RED; *west_led = RED; *walk_led = YELLOW;
            *wait_time = 3000; // 3s Warning
            current_state = CLEAR_WALK;
            break;

        case CLEAR_WALK:
            *south_led = RED; *west_led = RED; *walk_led = RED;
            *wait_time = 3000; // 3s Red

            if (led_input & 4)      current_state = GO_SOUTH;
            else if (led_input & 2) current_state = GO_WEST;
            else if (led_input & 1) current_state = GO_WALK;
            else                    current_state = GO_WALK;
            break;
    }
}

void Display_Traffic_UI(Led south, Led west, Led walk, uint32_t time) {
	//convert enum Led -> string to print
		const char *led_names[] = {"GRN", "YLW", "RED"};

	    ssd1306_Fill(Black);

	    //crossroad drawing
	    ssd1306_Line(45, 0, 45, 20, White);
	    ssd1306_Line(83, 0, 83, 20, White);
	    ssd1306_Line(45, 44, 45, 64, White);
	    ssd1306_Line(83, 44, 83, 64, White);

	    ssd1306_Line(0, 20, 45, 20, White);
	    ssd1306_Line(0, 44, 45, 44, White);
	    ssd1306_Line(83, 20, 128, 20, White);
	    ssd1306_Line(83, 44, 128, 44, White);

	    ssd1306_Line(45, 22, 83, 22, White);
	    ssd1306_Line(45, 42, 83, 42, White);

	    char buf[20];
	    //dong ho dem thoi gian sang cua cac led
	    snprintf(buf, sizeof(buf), "S:%s %2lus", led_names[south], time);
	    ssd1306_SetCursor(48, 5);
	    ssd1306_WriteString(buf, Font_6x8, White);

	    snprintf(buf, sizeof(buf), "W:%s %2lus", led_names[west], time);
	    ssd1306_SetCursor(2, 27);
	    ssd1306_WriteString(buf, Font_6x8, White);

	    snprintf(buf, sizeof(buf), "WLK:%s %2lus", led_names[walk], time);
	    ssd1306_SetCursor(48, 50);
	    ssd1306_WriteString(buf, Font_6x8, White);

	    ssd1306_UpdateScreen();
}

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
  /* USER CODE BEGIN 2 */
  ssd1306_Init();
  uint32_t last_tick = HAL_GetTick();
  uint32_t wait_time = 0;
  uint32_t elapsed_time = 0;
  Led south_led;
  Led west_led;
  Led walk_led;
  Display_Traffic_UI(south_led, west_led, walk_led, elapsed_time);

  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */
	  uint32_t current_tick = HAL_GetTick();
	  uint32_t south_input = HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_9);
	  uint32_t west_input = HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_10);
	  uint32_t walk_input = HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_11);
	  uint32_t led_input = (south_input << 2) | (west_input << 1) | walk_input;
	  elapsed_time = current_tick - last_tick;

	  if (elapsed_time >= wait_time) {
	  		  Process_Traffic_FSM(led_input, &south_led, &west_led, &walk_led, &wait_time);
	  		  last_tick = HAL_GetTick();
	  		  elapsed_time = 0; // reset elapsed time for the new state
	  	  }
	  Display_Traffic_UI(south_led, west_led, walk_led, elapsed_time);
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
  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE2);

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
  hi2c1.Init.ClockSpeed = 400000;
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
  __HAL_RCC_GPIOA_CLK_ENABLE();
  __HAL_RCC_GPIOB_CLK_ENABLE();

  /*Configure GPIO pins : PA9 PA10 PA11 */
  GPIO_InitStruct.Pin = GPIO_PIN_9|GPIO_PIN_10|GPIO_PIN_11;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

  /* USER CODE BEGIN MX_GPIO_Init_2 */

  /* USER CODE END MX_GPIO_Init_2 */
}

/* USER CODE BEGIN 4 */

/* USER CODE END 4 */

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
