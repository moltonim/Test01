/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.h
  * @brief          : Header for main.c file.
  *                   This file contains the common defines of the application.
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

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __MAIN_H
#define __MAIN_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "stm32u0xx_hal.h"

#include "stm32u0xx_ll_lpuart.h"
#include "stm32u0xx_ll_rcc.h"
#include "stm32u0xx_ll_pwr.h"
#include "stm32u0xx_ll_crs.h"
#include "stm32u0xx_ll_bus.h"
#include "stm32u0xx_ll_system.h"
#include "stm32u0xx_ll_exti.h"
#include "stm32u0xx_ll_cortex.h"
#include "stm32u0xx_ll_utils.h"
#include "stm32u0xx_ll_dma.h"
#include "stm32u0xx_ll_tim.h"
#include "stm32u0xx_ll_gpio.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */

/* USER CODE END Includes */

/* Exported types ------------------------------------------------------------*/
/* USER CODE BEGIN ET */
extern volatile uint8_t PressBttn;
extern void LPUART_CharReception_Callback(void);

extern uint8_t CommBuf[100];

extern int tx_pointer;
extern int tx_lenbuf;
extern __IO uint32_t     sTime;

/* USER CODE END ET */

/* Exported constants --------------------------------------------------------*/
/* USER CODE BEGIN EC */

/* USER CODE END EC */

/* Exported macro ------------------------------------------------------------*/
/* USER CODE BEGIN EM */

/* USER CODE END EM */

/* Exported functions prototypes ---------------------------------------------*/
void Error_Handler(void);

/* USER CODE BEGIN EFP */

/* USER CODE END EFP */

/* Private defines -----------------------------------------------------------*/
#define BLUBTTN_Pin LL_GPIO_PIN_13
#define BLUBTTN_GPIO_Port GPIOC
#define BLUBTTN_EXTI_IRQn EXTI4_15_IRQn
#define GREENLED_Pin LL_GPIO_PIN_5
#define GREENLED_GPIO_Port GPIOA

/* USER CODE BEGIN Private defines */

/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */
