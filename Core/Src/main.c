/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
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
#include "main.h"
#include "usart.h"
#include "tim.h"
#include "gpio.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include <stdio.h>
#include <string.h>
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
volatile uint8_t PressBttn;
__IO uint8_t      ubFinalCharReceived = 0;
__IO uint32_t     ubReceivedChar;

uint32_t TimOutClock = 1;
uint32_t timxPrescaler = 0;
uint32_t timxPeriod = 0;

char R,G,B;


uint8_t CommBuf[100];

int tx_pointer;
int tx_lenbuf;

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/

/* USER CODE BEGIN PV */

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
/* USER CODE BEGIN PFP */
__STATIC_INLINE void Configure_DutyCycle2(uint8_t chan, uint32_t D);
__STATIC_INLINE void Configure_DutyCycle(uint32_t D);
/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */
static uint32_t n;
/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{

  /* USER CODE BEGIN 1 */
	R = 0; G = 0; B = 0;
	memset(CommBuf, 0, sizeof(CommBuf));
  /* USER CODE END 1 */

  /* MCU Configuration--------------------------------------------------------*/

  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
  LL_APB1_GRP1_EnableClock(LL_APB1_GRP1_PERIPH_PWR);
  LL_APB1_GRP2_EnableClock(LL_APB1_GRP2_PERIPH_SYSCFG);

  /* System interrupt init*/
  NVIC_SetPriorityGrouping(NVIC_PRIORITYGROUP_2);

  /* SysTick_IRQn interrupt configuration */
  NVIC_SetPriority(SysTick_IRQn, NVIC_EncodePriority(NVIC_GetPriorityGrouping(),3, 0));

  /* USER CODE BEGIN Init */

  /* USER CODE END Init */

  /* Configure the system clock */
  SystemClock_Config();

  /* USER CODE BEGIN SysInit */
  PressBttn = 0;
  n = 20 * 1000;		//freq finale
//  n = 100;
  int p = 500 * 1000;

  TimOutClock = SystemCoreClock/1;
  timxPrescaler = __LL_TIM_CALC_PSC(SystemCoreClock, p);
  timxPeriod = __LL_TIM_CALC_ARR(TimOutClock, timxPrescaler, n);
  /* USER CODE END SysInit */

  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  MX_LPUART1_UART_Init();
  MX_TIM1_Init();
  /* USER CODE BEGIN 2 */

  /* Enable the capture/compare interrupt for channel 1 */
  LL_TIM_EnableIT_CC1(TIM1);

  /* Enable output channel 1 */
  LL_TIM_CC_EnableChannel(TIM1, LL_TIM_CHANNEL_CH1);
  LL_TIM_CC_EnableChannel(TIM1, LL_TIM_CHANNEL_CH2);
  LL_TIM_CC_EnableChannel(TIM1, LL_TIM_CHANNEL_CH3);

  /* Enable counter */
  LL_TIM_EnableCounter(TIM1);

  LL_TIM_EnableAllOutputs(TIM1);

  //Configure_DutyCycle(50);
  Configure_DutyCycle2(1, 50);
  Configure_DutyCycle2(2, 25);
  Configure_DutyCycle2(3, 75);

  /* Force update generation */
  LL_TIM_GenerateEvent_UPDATE(TIM1);

  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */
	  //for(n = 0; n < 100000UL; n++)
//	  HAL_Delay(200);
	  LL_mDelay(200);
//	  HAL_GPIO_TogglePin(GREENLED_GPIO_Port, GREENLED_Pin);
	  LL_GPIO_TogglePin(GREENLED_GPIO_Port, GREENLED_Pin);
	  if (PressBttn)
	  {
		  PressBttn = 0;
	  }
  }
  /* USER CODE END 3 */
}

/**
  * @brief System Clock Configuration
  * @retval None
  */
void SystemClock_Config(void)
{
  LL_FLASH_SetLatency(LL_FLASH_LATENCY_0);
  while(LL_FLASH_GetLatency() != LL_FLASH_LATENCY_0)
  {
  }

  LL_PWR_SetRegulVoltageScaling(LL_PWR_REGU_VOLTAGE_SCALE2);
  while (LL_PWR_IsActiveFlag_VOS() != 0)
  {
  }
  LL_RCC_MSI_Enable();

   /* Wait till MSI is ready */
  while(LL_RCC_MSI_IsReady() != 1)
  {
  }

  LL_RCC_MSI_EnableRangeSelection();
  LL_RCC_MSI_SetRange(LL_RCC_MSIRANGE_7);
  LL_RCC_SetSysClkSource(LL_RCC_SYS_CLKSOURCE_MSI);

   /* Wait till System clock is ready */
  while(LL_RCC_GetSysClkSource() != LL_RCC_SYS_CLKSOURCE_STATUS_MSI)
  {
  }

  LL_RCC_SetAHBPrescaler(LL_RCC_SYSCLK_DIV_1);
  LL_RCC_SetAPB1Prescaler(LL_RCC_APB1_DIV_1);

  LL_Init1msTick(8000000);

  LL_SetSystemCoreClock(8000000);
}

/* USER CODE BEGIN 4 */

__STATIC_INLINE void Configure_DutyCycle(uint32_t D)
{
  uint32_t P;    /* Pulse duration */
  uint32_t T;    /* PWM signal period */

  /* PWM signal period is determined by the value of the auto-reload register */
  T = LL_TIM_GetAutoReload(TIM1) + 1;

  /* Pulse duration is determined by the value of the compare register.       */
  /* Its value is calculated in order to match the requested duty cycle.      */
  P = (D*T)/100;
  LL_TIM_OC_SetCompareCH1(TIM1, P);
}


__STATIC_INLINE void Configure_DutyCycle2(uint8_t chan, uint32_t D)
{
  uint32_t P = 0;    /* Pulse duration */
  uint32_t T = 0;    /* PWM signal period */

  /* PWM signal period is determined by the value of the auto-reload register */
  T = LL_TIM_GetAutoReload(TIM1) + 1;

  /* Pulse duration is determined by the value of the compare register.       */
  /* Its value is calculated in order to match the requested duty cycle.      */
  P = (D*T)/100;
  switch (chan)
  {
	  default:
	  case 1:	LL_TIM_OC_SetCompareCH1(TIM1, P);	break;
	  case 2:	LL_TIM_OC_SetCompareCH2(TIM1, P);	break;
	  case 3:	LL_TIM_OC_SetCompareCH3(TIM1, P);	break;
  }
}


void LPUART_CharReception_Callback(void)
{
  /* Read Received character. RXNE flag is cleared by reading of RDR register */
  ubReceivedChar = LL_LPUART_ReceiveData8(LPUART1);
//  LL_LPUART_IsEnabledIT_RXNE_RXFNE

  /* Check if received value is corresponding to specific one : S or s */
  if ((ubReceivedChar == 'S') || (ubReceivedChar == 's'))
  {
    /* Turn LED4 On : Expected character has been received */
    //LED_On();

    /* End of program : set boolean for main loop exit */
    ubFinalCharReceived = 1;

    //scrivo i tre valori!
    sprintf((char*)CommBuf, "Rx= %d; G = %d, B = %d\n", R, G, B);
    tx_pointer = 0;
    tx_lenbuf = strlen((char*)&CommBuf);
    LL_LPUART_DisableIT_TC(LPUART1);
    LL_LPUART_DisableIT_RXNE_RXFNE(LPUART1);
    LL_LPUART_TransmitData8(LPUART1, CommBuf[tx_pointer++]);
    LL_LPUART_EnableIT_TXE(LPUART1);

    return;
  }

  switch (ubReceivedChar)
  {
	  case 'R':		R += 2; break;
	  case 'r':		R -= 2; break;
	  case 'G':		G += 2; break;
	  case 'g':		G -= 2; break;
	  case 'B':		B += 2;	break;
	  case 'b': 	B -= 2; break;

	  default : break;

  }

  if (R > 100)
	  R = 100;
  if (R < 0)
	  R = 0;

  if (G > 100)
	  G = 100;
  if (G < 0)
	  G = 0;

  if (B > 100)
  	  B = 100;
    if (B < 0)
  	  B = 0;


  /* Echo received character on TX */
  LL_LPUART_TransmitData8(LPUART1, ubReceivedChar);




}
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

#ifdef  USE_FULL_ASSERT
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
