#include <sys/errno.h>
#include <unistd.h>
#include <stdint.h>

#include "stm32l0xx_hal.h"

#include "uart.h"

UART_HandleTypeDef huart;

void uart_init(uint32_t baudrate)
{
	huart.Instance = USART2;
	huart.Init.BaudRate = baudrate;
	huart.Init.WordLength = UART_WORDLENGTH_8B;
	huart.Init.StopBits = UART_STOPBITS_1;
	huart.Init.Parity = UART_PARITY_NONE;
	huart.Init.Mode = UART_MODE_TX_RX;
	huart.Init.HwFlowCtl = UART_HWCONTROL_NONE;
	huart.Init.OverSampling = UART_OVERSAMPLING_16;
	huart.Init.OneBitSampling = UART_ONE_BIT_SAMPLE_DISABLE;
	huart.AdvancedInit.AdvFeatureInit = UART_ADVFEATURE_NO_INIT;
	HAL_UART_Init(&huart);
}

void HAL_UART_MspInit(UART_HandleTypeDef* huart)
{

	GPIO_InitTypeDef GPIO_InitStruct = {0};

	if(huart->Instance == USART2)
	{
		__HAL_RCC_USART2_CLK_ENABLE();

		__HAL_RCC_GPIOA_CLK_ENABLE();
		GPIO_InitStruct.Pin = GPIO_PIN_2 | GPIO_PIN_3;
		GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
		GPIO_InitStruct.Pull = GPIO_NOPULL;
		GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
		GPIO_InitStruct.Alternate = GPIO_AF4_USART2;
		HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);
	}

}

void HAL_UART_MspDeInit(UART_HandleTypeDef* huart)
{

	if(huart->Instance == USART2)
	{
		__HAL_RCC_USART2_CLK_DISABLE();
		HAL_GPIO_DeInit(GPIOA, GPIO_PIN_2 | GPIO_PIN_3);
	}

}

// Source: https://electronics.stackexchange.com/questions/206113/how-do-i-use-the-printf-function-on-stm32
int _write(int file, char *data, int len)
{
	 if ((file != STDOUT_FILENO) && (file != STDERR_FILENO))
	 {
			errno = EBADF;
			return -1;
	 }

	 // arbitrary timeout 1000
	 HAL_StatusTypeDef status = HAL_UART_Transmit(&huart, (uint8_t*)data, len, 1000);

	 // return # of bytes written - as best we can tell
	 return (status == HAL_OK ? len : 0);
}

