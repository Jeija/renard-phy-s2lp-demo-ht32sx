#include <stdio.h>
#include <string.h>

#include "stm32l0xx.h"

/* Renard PHY for S2-LP */
#include "renard_phy_s2lp_protocol.h"
#include "renard_phy_s2lp.h"

/* Application */
#include "systick_delay.h"
#include "button.h"
#include "uart.h"

#define SEQNUM_ADDR (DATA_EEPROM_BASE + 0)

/* ***PUT YOUR SIGFOX CREDENTIALS HERE*** */
uint8_t sigfox_key[] = {0x47, 0x9e, 0x44, 0x80, 0xfd, 0x75, 0x96, 0xd4, 0x5b, 0x01, 0x22, 0xfd, 0x28, 0x2d, 0xb3, 0xcf};
uint32_t sigfox_devid = 0x004d33db;
uint8_t sigfox_payload[] = {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x12, 0x34, 0x56};

/* *** Test credentials for testing with Sigfox RSA (Radio Signal Analyzer) *** */
/*uint8_t sigfox_key[] = {0x01, 0x23, 0x45, 0x67, 0x89, 0xAB, 0xCD, 0xEF, 0x01, 0x23, 0x45, 0x67, 0x89, 0xAB, 0xCD, 0xEF};
uint32_t sigfox_devid = 0xfedcba98;
uint8_t sigfox_payload[] = {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x07, 0x00, 0x00, 0x00};*/

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

void renard_phy_s2lp_hal_stm32_sysclk_init(void)
{
    sysclk_init();
}

uint16_t next_seqnum(void)
{
	uint16_t seqnum = *((uint8_t *)SEQNUM_ADDR);
	seqnum = (seqnum + 1) % 0xfff;

	HAL_FLASHEx_DATAEEPROM_Unlock();
	HAL_FLASHEx_DATAEEPROM_Program(TYPEPROGRAMDATA_WORD, SEQNUM_ADDR, seqnum);
	HAL_FLASHEx_DATAEEPROM_Lock();

	return seqnum;
}

int main(void)
{
	/* App Initialization */
	HAL_Init();
	sysclk_init();
	systick_delay_init();
	button_init();
	uart_init(115200);
	systick_delay_ms(10);

	/* S2-LP SPI communication initialization */
	printf("[renard-phy-s2lp-demo-stm32] Initializing S2-LP...\r\n");

	while (!renard_phy_s2lp_init()) {
		printf("[renard-phy-s2lp-demo-stm32] Initialization FAIL - S2-LP not responding!\r\n");
		printf("[renard-phy-s2lp-demo-stm32] Retrying initialization in 1s...\r\n");
		systick_delay_ms(1000);
	}
	printf("[renard-phy-s2lp-demo-stm32] Initialization OK, S2-LP detected!\r\n");

	/* Use current sequence number as (really bad) entropy source for uplink frequency randomization */
	uint16_t current_seqnum = next_seqnum();
	renard_phy_s2lp_protocol_init(current_seqnum, UL_DATARATE_100BPS);

	printf("[renard-phy-s2lp-demo-stm32] Initialization complete!\r\n");

	while(1)
	{
		if(button_pressed()) {
			printf("[renard-phy-s2lp-demo-stm32] Starting message transfer!\r\n");

			/* Prepare uplink */
			sfx_commoninfo common;
			common.seqnum = current_seqnum;
			common.devid = sigfox_devid;
			memcpy(common.key, sigfox_key, 16);
			current_seqnum = next_seqnum();

			sfx_ul_plain uplink;
			memcpy(uplink.payload, sigfox_payload, sizeof(sigfox_payload));
			uplink.payloadlen = sizeof(sigfox_payload);
			uplink.request_downlink = true;
			uplink.singlebit = false;
			uplink.replicas = true;

			sfx_dl_plain downlink;
			int16_t downlink_rssi;
			int err = renard_phy_s2lp_protocol_transfer(&common, &uplink, &downlink, &downlink_rssi);

			if (err == PROTOCOL_ERROR_NONE) {
				if (uplink.request_downlink) {
					printf("[renard-phy-s2lp-demo-stm32] Downlink received!\r\n");
					printf("[renard-phy-s2lp-demo-stm32] Downlink RSSI    : %d\r\n", downlink_rssi);
					printf("[renard-phy-s2lp-demo-stm32] Downlink payload : ");
					for (int i = 0; i < SFX_DL_PAYLOADLEN; i++)
						printf("%02x", downlink.payload[i]);
					printf("\r\n");
					printf("[renard-phy-s2lp-demo-stm32] Downlink CRC OK  : %d\r\n", downlink.crc_ok);
					printf("[renard-phy-s2lp-demo-stm32] Downlink MAC OK  : %d\r\n", downlink.mac_ok);
					printf("[renard-phy-s2lp-demo-stm32] Downlink FEC Use : %d\r\n", downlink.fec_corrected);
				} else {
					printf("[renard-phy-s2lp-demo-stm32] Uplink transmitted, no downlink requested.\r\n");
				}
			} else if (err == PROTOCOL_ERROR_TIMEOUT) {
				printf("[renard-phy-s2lp-demo-stm32] Timeout while waiting for downlink\r\n");
			} else {
				printf("[renard-phy-s2lp-demo-stm32] Unknown protocol error occurred\r\n");
			}
		}
	}
}

