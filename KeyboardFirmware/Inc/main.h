/**
  ******************************************************************************
  * File Name          : main.h
  * Description        : This file contains the common defines of the application
  ******************************************************************************
  * This notice applies to any and all portions of this file
  * that are not between comment pairs USER CODE BEGIN and
  * USER CODE END. Other portions of this file, whether 
  * inserted by the user or by software development tools
  * are owned by their respective copyright owners.
  *
  * Copyright (c) 2017 STMicroelectronics International N.V. 
  * All rights reserved.
  *
  * Redistribution and use in source and binary forms, with or without 
  * modification, are permitted, provided that the following conditions are met:
  *
  * 1. Redistribution of source code must retain the above copyright notice, 
  *    this list of conditions and the following disclaimer.
  * 2. Redistributions in binary form must reproduce the above copyright notice,
  *    this list of conditions and the following disclaimer in the documentation
  *    and/or other materials provided with the distribution.
  * 3. Neither the name of STMicroelectronics nor the names of other 
  *    contributors to this software may be used to endorse or promote products 
  *    derived from this software without specific written permission.
  * 4. This software, including modifications and/or derivative works of this 
  *    software, must execute solely and exclusively on microcontroller or
  *    microprocessor devices manufactured by or for STMicroelectronics.
  * 5. Redistribution and use of this software other than as permitted under 
  *    this license is void and will automatically terminate your rights under 
  *    this license. 
  *
  * THIS SOFTWARE IS PROVIDED BY STMICROELECTRONICS AND CONTRIBUTORS "AS IS" 
  * AND ANY EXPRESS, IMPLIED OR STATUTORY WARRANTIES, INCLUDING, BUT NOT 
  * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY, FITNESS FOR A 
  * PARTICULAR PURPOSE AND NON-INFRINGEMENT OF THIRD PARTY INTELLECTUAL PROPERTY
  * RIGHTS ARE DISCLAIMED TO THE FULLEST EXTENT PERMITTED BY LAW. IN NO EVENT 
  * SHALL STMICROELECTRONICS OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT,
  * INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
  * LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, 
  * OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF 
  * LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING 
  * NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE,
  * EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
  *
  ******************************************************************************
  */
/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __MAIN_H
#define __MAIN_H
  /* Includes ------------------------------------------------------------------*/

/* USER CODE BEGIN Includes */

/* USER CODE END Includes */

/* Private define ------------------------------------------------------------*/

/* USER CODE BEGIN Private defines */

#define VBatReading_Pin GPIO_PIN_5
#define VBatReading_GPIO_Port GPIOA
#define LED_R_Pin GPIO_PIN_9
#define LED_R_GPIO_Port GPIOA
#define LED_G_Pin GPIO_PIN_6
#define LED_G_GPIO_Port GPIOA
#define LED_B_Pin GPIO_PIN_7
#define LED_B_GPIO_Port GPIOA
#define BMBCTS_Pin GPIO_PIN_10
#define BMBCTS_GPIO_Port GPIOB
#define BM_EN_Pin GPIO_PIN_14
#define BM_EN_GPIO_Port GPIOB
#define BMRES_Pin GPIO_PIN_15
#define BMRES_GPIO_Port GPIOB

#define Row0_Pin GPIO_PIN_0
#define Row0_GPIO_Port GPIOA
#define Row1_Pin GPIO_PIN_1
#define Row1_GPIO_Port GPIOA
#define Row2_Pin GPIO_PIN_2
#define Row2_GPIO_Port GPIOA
#define Row3_Pin GPIO_PIN_3
#define Row3_GPIO_Port GPIOA
#define Row4_Pin GPIO_PIN_4
#define Row4_GPIO_Port GPIOA
#define Col0_Pin GPIO_PIN_0
#define Col0_GPIO_Port GPIOB
#define Col1_Pin GPIO_PIN_1
#define Col1_GPIO_Port GPIOB
#define Col2_Pin GPIO_PIN_2
#define Col2_GPIO_Port GPIOB
#define Col3_Pin GPIO_PIN_3
#define Col3_GPIO_Port GPIOB
#define Col4_Pin GPIO_PIN_4
#define Col4_GPIO_Port GPIOB
#define Col5_Pin GPIO_PIN_5
#define Col5_GPIO_Port GPIOB

#define Col6_Pin GPIO_PIN_0
#define Col6_GPIO_Port GPIOB
#define Col7_Pin GPIO_PIN_1
#define Col7_GPIO_Port GPIOB
#define Col8_Pin GPIO_PIN_2
#define Col8_GPIO_Port GPIOB
#define Col9_Pin GPIO_PIN_3
#define Col9_GPIO_Port GPIOB
#define Col10_Pin GPIO_PIN_4
#define Col10_GPIO_Port GPIOB
#define Col11_Pin GPIO_PIN_5
#define Col11_GPIO_Port GPIOB

#define Col12_Pin GPIO_PIN_12
#define Col12_GPIO_Port GPIOB
#define Col13_Pin GPIO_PIN_13
#define Col13_GPIO_Port GPIOB

#define LorR_Pin GPIO_PIN_8
#define LorR_GPIO_Port GPIOA


/* USER CODE END Private defines */

void _Error_Handler(char *, int);

#define Error_Handler() _Error_Handler(__FILE__, __LINE__)

/**
  * @}
  */ 

/**
  * @}
*/ 

#endif /* __MAIN_H */
/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
