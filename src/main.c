#include <stdio.h>
#include <string.h>

#include "stm32l0xx.h"
#include <stm32l0xx_hal.h>

/* Application */
#include "systick_delay.h"
#include "button.h"
#include "uart.h"

void sysclk_init(void)
{
	__HAL_RCC_PWR_CLK_ENABLE();
	__HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE1);
	while (__HAL_PWR_GET_FLAG(PWR_FLAG_VOS) != RESET) {};

	/* Enable HSI Oscillator and activate PLL with HSI as source */
	RCC_OscInitTypeDef RCC_OscInitStruct;
	RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI;
	RCC_OscInitStruct.HSEState = RCC_HSE_OFF;
	RCC_OscInitStruct.HSIState = RCC_HSI_ON;
	RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
	RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSI;
	RCC_OscInitStruct.PLL.PLLMUL = RCC_PLL_MUL4;
	RCC_OscInitStruct.PLL.PLLDIV = RCC_PLL_DIV2;
	RCC_OscInitStruct.HSICalibrationValue = 0x10;
	HAL_RCC_OscConfig(&RCC_OscInitStruct);

	/* Select PLL as system clock source and configure the HCLK, PCLK1 and PCLK2 clocks dividers */
	RCC_ClkInitTypeDef RCC_ClkInitStruct;
	RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_SYSCLK | RCC_CLOCKTYPE_HCLK | RCC_CLOCKTYPE_PCLK1 | RCC_CLOCKTYPE_PCLK2;
	RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
	RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV2;
	RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;
	RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;
	HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_1);
}

static void led_init(void)
{
	__HAL_RCC_GPIOB_CLK_ENABLE();

	GPIO_InitTypeDef gpioinitstruct;
	gpioinitstruct.Pin = GPIO_PIN_0;
	gpioinitstruct.Mode = GPIO_MODE_OUTPUT_PP;
	gpioinitstruct.Speed = GPIO_SPEED_LOW;
	gpioinitstruct.Pull = GPIO_NOPULL;
	HAL_GPIO_Init(GPIOB, &gpioinitstruct);
}

int main(void)
{
	/* App Initialization */
	HAL_Init();
	sysclk_init();
	systick_delay_init();
	button_init();
	uart_init(115200);
	led_init();
	systick_delay_ms(10);

	while(1)
	{
		if(button_pressed()) {
			printf("Hello World from HT32SX\r\n");
			HAL_GPIO_TogglePin(GPIOB, GPIO_PIN_0);
		}
	}
}
