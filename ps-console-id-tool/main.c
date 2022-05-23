#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <fcntl.h>

#include "types.h"


// Bitfield structure adapted for little-endian machines (unsure if working on big-endian machines)
typedef struct SceConsoleId {
	uint16_t unknown;      // {0, 0}, maybe magic
	uint16_t company_code; // {0, 1}
	uint16_t product_code;
	uint16_t product_sub_code;
	uint8_t serial_no_major: 2;
	uint8_t factory_code: 6;
	uint8_t serial_no_middle;
	uint16_t serial_no_minor;
	uint32_t random_stamp;
} SceConsoleId;

int cid_text_to_cid_hex(const char *cid_text, uint8_t* buffer) {
	uint8_t i = 0;
	char temp_byte_string[2];
	for (; i < 32; i += 2) {
		snprintf(temp_byte_string, 2, "%c", cid_text[i]);
		snprintf(temp_byte_string+1, 2, "%c", cid_text[i+1]);
		buffer[i/2] = strtol(temp_byte_string, NULL, 16);
	}
	return i;
}

int read_byte(FILE *fp, int offset, char *buf) {
	int prev = ftell(fp);
    fseek(fp, offset, SEEK_SET);
	int r = fread(buf, sizeof(char), 1, fp);
	fseek(fp, prev, SEEK_SET); // go back to where we were
	return r;
}

int main (int argc, char *argv[]) {
	char cid_text[32];
	memset(cid_text, 'Z', 32);
	memcpy(&cid_text, argv[1], 32);
	uint8_t cid_hex[16];
	memset(cid_hex, 'Z', 16);
	cid_text_to_cid_hex(cid_text, cid_hex);
	/////////////////////////////////////////////////////////////////////
	printf("Checking input ConsoleId...\n");
	if (cid_hex[15] == 0) {// We check if 32 bytes have well been read.
		printf("Usage ps-console-id-tool.exe 00000000000000000000000000000000\nPlease verify that the CID in input is 32 character long.\n");
	} else {
		printf("ConsoleId:\n");
		for (int j = 0; j < 16; ++j)
			printf("%02X", cid_hex[j]);
		printf("\n");
		/////////////////////////////////////////////////////////////////////
		printf("Structure (displayed in big endian):\n");
		SceConsoleId *cid = (SceConsoleId *)&cid_hex;
		printf("Unknown: 0x%04X\n", be16((u8 *)&(cid->unknown)));
		printf("Company Code: %d\n", be16((u8 *)&(cid->company_code)));
		printf("Product Code: 0x%04X\n", be16((u8 *)&(cid->product_code)));
		printf("Product Sub Code: 0x%04X\n", be16((u8 *)&(cid->product_sub_code)));
		printf("Factory Code: %d\n", cid->factory_code);
		uint32_t serial_no = (cid->serial_no_major << 24) | (cid->serial_no_middle << 16) | be16((u8 *)&(cid->serial_no_minor));
		printf("Serial Number: %d\n", serial_no);
		printf("Random Stamp: 0x%08X\n", be32((u8 *)&(cid->random_stamp)));
	}
	printf("Finished\n");
	return 0;
}