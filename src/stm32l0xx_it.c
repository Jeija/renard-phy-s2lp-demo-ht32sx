/**
********************************************************************************
* @file    stm32l0xx_it.c
* @author  MCD Application Team
* @version V1.0.0
* @date    10-July-2018
* @brief   Main Interrupt Service Routines.
*          This file provides template for all exceptions handler and
*          peripherals interrupt service routine.
********************************************************************************
* @attention
*
* <h2><center>&copy; COPYRIGHT(c) 2018 STMicroelectronics</center></h2>
*
* Redistribution and use in source and binary forms, with or without modification,
* are permitted provided that the following conditions are met:
*   1. Redistributions of source code must retain the above copyright notice,
*      this list of conditions and the following disclaimer.
*   2. Redistributions in binary form must reproduce the above copyright notice,
*      this list of conditions and the following disclaimer in the documentation
*      and/or other materials provided with the distribution.
*   3. Neither the name of STMicroelectronics nor the names of its contributors
*      may be used to endorse or promote products derived from this software
*      without specific prior written permission.
*
* THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
* AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
* IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
* DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
* FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
* DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
* SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
* CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
* OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
* OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*
******************************************************************************
*/
/* Includes ------------------------------------------------------------------*/
#include "stm32l0xx_hal.h"
#include "stm32l0xx_it.h"

/** @addtogroup Sigfox_CLI_Demo
  * @{
  */

/* Exported variables --------------------------------------------------------*/
extern uint16_t counter;
extern uint8_t firstinterrupt;


/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/


/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/

/******************************************************************************/
/*            Cortex-M0+ Processor Exceptions Handlers                         */
/******************************************************************************/

/**
  * @brief  This function handles NMI exception.
  * @param  None
  * @retval : None
  */
void NMI_Handler(void)
{
}

/**
  * @brief  This function handles Hard Fault exception.
  * @param  None
  * @retval : None
  */
void HardFault_Handler(void)
{
  /* Go to infinite loop when Hard Fault exception occurs */
  while (1)
  {
  }
}

/**
  * @brief  This function handles Memory Manage exception.
  * @param  None
  * @retval : None
  */
void MemManage_Handler(void)
{
  /* Go to infinite loop when Memory Manage exception occurs */
  while (1)
  {
  }
}

/**
  * @brief  This function handles Bus Fault exception.
  * @param  None
  * @retval : None
  */
void BusFault_Handler(void)
{
  /* Go to infinite loop when Bus Fault exception occurs */
  while (1)
  {
  }
}

/**
  * @brief  This function handles Usage Fault exception.
  * @param  None
  * @retval : None
  */
void UsageFault_Handler(void)
{
  /* Go to infinite loop when Usage Fault exception occurs */
  while (1)
  {
  }
}

/**
  * @brief  This function handles SVCall exception.
  * @param  None
  * @retval : None
  */
void SVC_Handler(void)
{
}

/**
  * @brief  This function handles Debug Monitor exception.
  * @param  None
  * @retval : None
  */
void DebugMon_Handler(void)
{
}

/**
  * @brief  This function handles PendSVC exception.
  * @param  None
  * @retval : None
  */
void PendSV_Handler(void)
{
}

/**
* @brief  This function handles External External line 0 to 1 interrupt request.
* @param  None
* @retval None
*/
void EXTI0_1_IRQHandler(void)
{
  if(__HAL_GPIO_EXTI_GET_IT(GPIO_PIN_0) != RESET)
  {
    HAL_GPIO_EXTI_IRQHandler(GPIO_PIN_0);
  }

  if(__HAL_GPIO_EXTI_GET_IT(GPIO_PIN_1) != RESET)
  {
    HAL_GPIO_EXTI_IRQHandler(GPIO_PIN_1);
  }
}

/**
* @brief  This function handles External External line 2 to 3 interrupt request.
* @param  None
* @retval None
*/
void EXTI2_3_IRQHandler(void)
{
  if(__HAL_GPIO_EXTI_GET_IT(GPIO_PIN_2) != RESET)
  {
    HAL_GPIO_EXTI_IRQHandler(GPIO_PIN_2);
  }

  if(__HAL_GPIO_EXTI_GET_IT(GPIO_PIN_3) != RESET)
  {
    HAL_GPIO_EXTI_IRQHandler(GPIO_PIN_3);
  }
}

/**
* @brief  This function handles External External line 9 to 5 interrupt request.
* @param  None
* @retval None
*/
void EXTI4_15_IRQHandler(void)
{
  if(__HAL_GPIO_EXTI_GET_IT(GPIO_PIN_4) != RESET)
  {
    HAL_GPIO_EXTI_IRQHandler(GPIO_PIN_4);
  }
  else if(__HAL_GPIO_EXTI_GET_IT(GPIO_PIN_5) != RESET)
  {
    HAL_GPIO_EXTI_IRQHandler(GPIO_PIN_5);
  }
  else if(__HAL_GPIO_EXTI_GET_IT(GPIO_PIN_6) != RESET)
  {
    HAL_GPIO_EXTI_IRQHandler(GPIO_PIN_6);
  }
  else if(__HAL_GPIO_EXTI_GET_IT(GPIO_PIN_7) != RESET)
  {
    HAL_GPIO_EXTI_IRQHandler(GPIO_PIN_7);
  }
  else if(__HAL_GPIO_EXTI_GET_IT(GPIO_PIN_8) != RESET)
  {
    HAL_GPIO_EXTI_IRQHandler(GPIO_PIN_8);
  }
  else if(__HAL_GPIO_EXTI_GET_IT(GPIO_PIN_9) != RESET)
  {
    HAL_GPIO_EXTI_IRQHandler(GPIO_PIN_9);
  }
  else if(__HAL_GPIO_EXTI_GET_IT(GPIO_PIN_10) != RESET)
  {
    HAL_GPIO_EXTI_IRQHandler(GPIO_PIN_10);
  }
  else if(__HAL_GPIO_EXTI_GET_IT(GPIO_PIN_11) != RESET)
  {
    HAL_GPIO_EXTI_IRQHandler(GPIO_PIN_11);
  }
  else if(__HAL_GPIO_EXTI_GET_IT(GPIO_PIN_12) != RESET)
  {
    HAL_GPIO_EXTI_IRQHandler(GPIO_PIN_12);
  }
  else if(__HAL_GPIO_EXTI_GET_IT(GPIO_PIN_13) != RESET)
  {
    HAL_GPIO_EXTI_IRQHandler(GPIO_PIN_13);
  }
  else if(__HAL_GPIO_EXTI_GET_IT(GPIO_PIN_14) != RESET)
  {
    HAL_GPIO_EXTI_IRQHandler(GPIO_PIN_14);
  }
  else if(__HAL_GPIO_EXTI_GET_IT(GPIO_PIN_15) != RESET)
  {
    HAL_GPIO_EXTI_IRQHandler(GPIO_PIN_15);
  }

}

/**
  * @}
  */


/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
