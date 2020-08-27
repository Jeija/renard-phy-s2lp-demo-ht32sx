#include <stdbool.h>
#include <stdint.h>
#include <string.h>

#include "eeprom_credentials.h"

#define rev_endian32(num) ((num >> 24) & 0xff) | ((num << 8) & 0xff0000) | ((num >> 8) & 0xff00) | ((num << 24) & 0xff000000);

// TODO: Currently simply reuses librenard's AES function, this is a dirty hack
void renard_aes_enc_dec(unsigned char *state, const unsigned char *Localkey, unsigned char dir);

/* 
 * Return value:
 *  -> true : Sigfox credentials successfully read
 *  -> false: Either checksum computation failed (corrupted EEPROM) or unknown NAK encryption mode
 */
bool eeprom_credentials_read(uint32_t *devid, uint8_t pac[static EEPROM_CREDENTIALS_PAC_SIZE], uint8_t nak[static EEPROM_CREDENTIALS_NAK_SIZE])
{
	// Copy credential data from eeprom
	uint8_t eeprom[64];
	memcpy(eeprom, ((uint8_t *)DATA_EEPROM_BASE), sizeof(eeprom));

	// Reverse EEPROM endianness
	for (uint8_t i = 0; i < sizeof(eeprom); i += 4)
		*((uint32_t *)(eeprom + i)) = rev_endian32(*((uint32_t *)(eeprom + i)));

	// Compute checksum
	uint32_t checksum = 0;
	uint32_t checksum_true = *((uint32_t *)&eeprom[60]);
	for (uint8_t i = 0; i < 48; ++i)
		checksum += eeprom[i];

	if (checksum != checksum_true)
		return false;

	// Copy device ID and PAC
	*devid = *((uint32_t*)eeprom);
	memcpy(pac, &eeprom[8], EEPROM_CREDENTIALS_PAC_SIZE);

	// Decrypt NAK
	uint8_t encryption_mode = eeprom[7];
	if (encryption_mode != 1) {
		return false;
	} else {
		uint8_t nak_dec_key[16];
		nak_dec_key[ 0] = eeprom[ 6];
		nak_dec_key[ 1] = eeprom[32];
		nak_dec_key[ 2] = eeprom[34];
		nak_dec_key[ 3] = eeprom[36];
		nak_dec_key[ 4] = eeprom[38];
		nak_dec_key[ 5] = eeprom[40];
		nak_dec_key[ 6] = eeprom[42];
		nak_dec_key[ 7] = eeprom[44];
		nak_dec_key[ 8] = eeprom[45];
		nak_dec_key[ 9] = eeprom[33];
		nak_dec_key[10] = eeprom[43];
		nak_dec_key[11] = eeprom[35];
		nak_dec_key[12] = eeprom[41];
		nak_dec_key[13] = eeprom[37];
		nak_dec_key[14] = eeprom[39];
		nak_dec_key[15] = eeprom[ 5];

		memcpy(nak, &eeprom[16], EEPROM_CREDENTIALS_NAK_SIZE);
		renard_aes_enc_dec(nak, nak_dec_key, 1);
	}

	return true;
}
