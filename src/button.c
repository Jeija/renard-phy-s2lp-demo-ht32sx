#include <stdbool.h>
#include <stdint.h>

#include <stm32l0xx_hal.h>

#define USER_BUTTON_PIN                         GPIO_PIN_5
#define USER_BUTTON_GPIO_PORT                   GPIOB
#define USER_BUTTON_GPIO_CLK_ENABLE()           __HAL_RCC_GPIOB_CLK_ENABLE()
#define USER_BUTTON_GPIO_CLK_DISABLE()          __HAL_RCC_GPIOB_CLK_DISABLE()
#define USER_BUTTON_EXTI_IRQn                   EXTI4_15_IRQn
#define USER_BUTTON_EXTI_LINE                   GPIO_PIN_5

static bool pressed_flag = 0;

bool button_pressed(void)
{
	if (pressed_flag) {
		pressed_flag = false;
		return true;
	} else {
		return false;
	}
}

void button_init(void)
{
	GPIO_InitTypeDef gpioinitstruct;
	USER_BUTTON_GPIO_CLK_ENABLE();

	gpioinitstruct.Pin = USER_BUTTON_PIN;
	gpioinitstruct.Pull = GPIO_NOPULL;
	gpioinitstruct.Speed = GPIO_SPEED_FREQ_MEDIUM;
	gpioinitstruct.Mode = GPIO_MODE_IT_FALLING;

	HAL_GPIO_Init(USER_BUTTON_GPIO_PORT, &gpioinitstruct);

	/* Enable and set Button EXTI Interrupt to the lowest priority */
	HAL_NVIC_SetPriority((IRQn_Type)(USER_BUTTON_EXTI_IRQn), 0x0F, 0);
	HAL_NVIC_EnableIRQ((IRQn_Type)(USER_BUTTON_EXTI_IRQn));
}

/*
 * Catch GPIO_EXTI_Callback interrupt, intercept button press and otherwise forward to renard-phy-s2lp-hal-stm32.
 * Weak symbol renard_phy_s2lp_hal_stm32_GPIO_EXTI_Callback will be overridden when linking to
 * renard-phy-s2lp-hal-stm32.
 */
__weak void renard_phy_s2lp_hal_stm32_GPIO_EXTI_Callback(uint16_t GPIO_Pin)
{
}

void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin)
{
	if(GPIO_Pin == USER_BUTTON_PIN) {
		pressed_flag = 1;
	} else {
		renard_phy_s2lp_hal_stm32_GPIO_EXTI_Callback(GPIO_Pin);
	}
}
