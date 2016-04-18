#ifndef MAX7219_H
#define MAX7219_H
//=============================================================================
#include <inttypes.h>
#include "pins.h"
//=============================================================================
// Registers
#define MAX7219_NO_OP			0x00
#define MAX7219_DIGIT_0			0x01
#define MAX7219_DIGIT_1			0x02
#define MAX7219_DIGIT_2			0x03
#define MAX7219_DIGIT_3			0x04
#define MAX7219_DIGIT_4			0x05
#define MAX7219_DIGIT_5			0x06
#define MAX7219_DIGIT_6			0x07
#define MAX7219_DIGIT_7			0x08
#define MAX7219_DEC_MODE		0x09
#define MAX7219_INTENSITY		0x0A
#define MAX7219_SCAN_LIMIT		0x0B
#define MAX7219_SHUTDOWN		0x0C
#define MAX7219_DISP_TEST		0x0F
//=============================================================================
void MAX7219_SendByte(uint8_t data);
void MAX7219_SendCmd(uint8_t reg, uint8_t data);
void MAX7219_Init(void);
void MAX7219_clearDisplay(void);
void MAX7219_updateDisplay(void);
void MAX7219_printNum(uint8_t pos, signed int x, unsigned char n, unsigned char fillch);
void MAX7219_printStr(uint8_t pos, char *data);
char MAX7219_lookupCode(char character);
void MAX7219_setCommaPos(uint8_t pos, uint8_t comma);
//=============================================================================
#endif /* MAX7219_H */
