#include "systick_delay.h"

static volatile uint32_t systickCounter = 0;

void SysTick_Handler(void)
{
	systickCounter++;
}

void systick_delay_init(void)
{
	systickCounter = 0;

	/* Systick Setup: Once per millisecond */
	HAL_SYSTICK_Config(HAL_RCC_GetHCLKFreq() / 1000);
	HAL_NVIC_SetPriority(SysTick_IRQn, 0, 0);
}

void systick_delay_ms(uint32_t duration)
{
	uint32_t targetSystick = ~systickCounter;

	if (targetSystick < duration) {
		while (systickCounter != 0xFFFFFFFF);
		targetSystick = duration - targetSystick;
	} else {
		targetSystick = duration + ~targetSystick;
	}

	while (systickCounter != targetSystick);
}
