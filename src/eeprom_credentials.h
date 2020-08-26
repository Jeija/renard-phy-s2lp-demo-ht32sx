#include <stdbool.h>
#include <stdint.h>

#include "stm32l0xx.h"

#ifndef _EEPROM_CREDENTIALS
#define _EEPROM_CREDENTIALS

#define EEPROM_CREDENTIALS_ADDR DATA_EEPROM_BASE
#define EEPROM_CREDENTIALS_PAC_SIZE 8
#define EEPROM_CREDENTIALS_NAK_SIZE 16

bool eeprom_credentials_read(uint32_t *devid, uint8_t pac[static EEPROM_CREDENTIALS_PAC_SIZE], uint8_t nak[static EEPROM_CREDENTIALS_NAK_SIZE]);

#endif
