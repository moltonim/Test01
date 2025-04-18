/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file    stm32u0xx_it.c
  * @brief   Interrupt Service Routines.
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
#include "stm32u0xx_it.h"
/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN TD */

/* USER CODE END TD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
/* USER CODE BEGIN PV */

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */

/* USER CODE END 0 */

/* External variables --------------------------------------------------------*/

/* USER CODE BEGIN EV */

/* USER CODE END EV */

/******************************************************************************/
/*           Cortex Processor Interruption and Exception Handlers          */
/******************************************************************************/
/**
  * @brief This function handles Non maskable interrupt.
  */
void NMI_Handler(void)
{
  /* USER CODE BEGIN NonMaskableInt_IRQn 0 */

  /* USER CODE END NonMaskableInt_IRQn 0 */
  /* USER CODE BEGIN NonMaskableInt_IRQn 1 */
   while (1)
  {
  }
  /* USER CODE END NonMaskableInt_IRQn 1 */
}

/**
  * @brief This function handles Hard fault interrupt.
  */
void HardFault_Handler(void)
{
  /* USER CODE BEGIN HardFault_IRQn 0 */

  /* USER CODE END HardFault_IRQn 0 */
  while (1)
  {
    /* USER CODE BEGIN W1_HardFault_IRQn 0 */
    /* USER CODE END W1_HardFault_IRQn 0 */
  }
}

/**
  * @brief This function handles System service call via SVC instruction.
  */
void SVC_Handler(void)
{
  /* USER CODE BEGIN SVCall_IRQn 0 */

  /* USER CODE END SVCall_IRQn 0 */
  /* USER CODE BEGIN SVCall_IRQn 1 */

  /* USER CODE END SVCall_IRQn 1 */
}

/**
  * @brief This function handles Pendable request for system service.
  */
void PendSV_Handler(void)
{
  /* USER CODE BEGIN PendSV_IRQn 0 */

  /* USER CODE END PendSV_IRQn 0 */
  /* USER CODE BEGIN PendSV_IRQn 1 */

  /* USER CODE END PendSV_IRQn 1 */
}

/**
  * @brief This function handles System tick timer.
  */
void SysTick_Handler(void)
{
  /* USER CODE BEGIN SysTick_IRQn 0 */

  /* USER CODE END SysTick_IRQn 0 */

  /* USER CODE BEGIN SysTick_IRQn 1 */

  /* USER CODE END SysTick_IRQn 1 */
}

/******************************************************************************/
/* STM32U0xx Peripheral Interrupt Handlers                                    */
/* Add here the Interrupt Handlers for the used peripherals.                  */
/* For the available peripheral interrupt handler names,                      */
/* please refer to the startup file (startup_stm32u0xx.s).                    */
/******************************************************************************/

/**
  * @brief This function handles EXTI line 4 to 15 interrupts.
  */
void EXTI4_15_IRQHandler(void)
{
  /* USER CODE BEGIN EXTI4_15_IRQn 0 */

  /* USER CODE END EXTI4_15_IRQn 0 */
  if (LL_EXTI_IsActiveFallingFlag_0_31(LL_EXTI_LINE_13) != RESET)
  {
    LL_EXTI_ClearFallingFlag_0_31(LL_EXTI_LINE_13);
    /* USER CODE BEGIN LL_EXTI_LINE_13_FALLING */
    PressBttn = 1;
    /* USER CODE END LL_EXTI_LINE_13_FALLING */
  }
  /* USER CODE BEGIN EXTI4_15_IRQn 1 */

  /* USER CODE END EXTI4_15_IRQn 1 */
}

/**
  * @brief This function handles USART3 (combined with EXTI 24) + LPUART1 global interrupt (combined with EXTI lines 28).
  */
void USART3_LPUART1_IRQHandler(void)
{
  /* USER CODE BEGIN USART3_LPUART1_IRQn 0 */
	if (LL_LPUART_IsActiveFlag_WKUP(LPUART1) && LL_LPUART_IsEnabledIT_WKUP(LPUART1))
	{
		/* Configure LPUART1 transfer interrupts : */
		/* Disable the UART Wake UP from stop mode Interrupt */
		LL_LPUART_DisableIT_WKUP(LPUART1);

		/* WUF flag clearing */
		LL_LPUART_ClearFlag_WKUP(LPUART1);

		/* Call function in charge of handling Character reception */
		LPUART_CharReception_Callback();
	}
	else if (LL_LPUART_IsEnabledIT_RXNE_RXFNE(LPUART1))
	{
		LPUART_CharReception_Callback();
	}
	else
	{
		/* Call Error function */
		Error_Handler();
	}

  /* USER CODE END USART3_LPUART1_IRQn 0 */
  /* USER CODE BEGIN USART3_LPUART1_IRQn 1 */
#ifdef	BOOTH5
	if(LL_USART_IsEnabledIT_TXE(USART1) && LL_USART_IsActiveFlag_TXE(USART1))
		{
			if (tx_pointer == (tx_lenbuf-1) )
			{
				// Disable TX interrupt
				LL_USART_DisableIT_TXE(USART1);
				// Enable TC interrupt
				LL_USART_EnableIT_TC(USART1);
			}
			LL_USART_TransmitData8(USART1, CommBuf[tx_pointer++]);
		}

		if (LL_USART_IsActiveFlag_TC(USART1) && LL_USART_IsEnabledIT_TC(USART1))
		{

			LL_USART_DisableIT_TC(USART1); // Suppress interrupt when empty
			// >>> set 485 direction to receive
			rx_pointer = 0;
			Clear_Periph_Event(EVENT_COMM_485Rec);
			COMM485_RX;
		}
#endif
  /* USER CODE END USART3_LPUART1_IRQn 1 */
}

/* USER CODE BEGIN 1 */

/* USER CODE END 1 */
