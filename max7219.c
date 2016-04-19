//=============================================================================
#include <avr/pgmspace.h>
//=============================================================================
#include "max7219.h"
//=============================================================================
uint8_t digit_pin[] = {MAX_DIG_1, MAX_DIG_2, MAX_DIG_3, MAX_DIG_4, MAX_DIG_5, MAX_DIG_6, MAX_DIG_7, MAX_DIG_8};
uint8_t buffer_display[8] = {' ',' ',' ',' ',' ',' ',' ',' '};
//=============================================================================
char MAX7219_lookupCode(char character)
{
  switch(character) {
    case '0':
      return ((1<<SEG_A)|(1<<SEG_B)|(1<<SEG_C)|(1<<SEG_D)|(1<<SEG_E)|(1<<SEG_F)|(0<<SEG_G)|(0<<SEG_H));
    break;
    case '1':
      return ((0<<SEG_A)|(1<<SEG_B)|(1<<SEG_C)|(0<<SEG_D)|(0<<SEG_E)|(0<<SEG_F)|(0<<SEG_G)|(0<<SEG_H));
    break;
    case '2':
      return ((1<<SEG_A)|(1<<SEG_B)|(0<<SEG_C)|(1<<SEG_D)|(1<<SEG_E)|(0<<SEG_F)|(1<<SEG_G)|(0<<SEG_H));
    break;
    case '3':
      return ((1<<SEG_A)|(1<<SEG_B)|(1<<SEG_C)|(1<<SEG_D)|(0<<SEG_E)|(0<<SEG_F)|(1<<SEG_G)|(0<<SEG_H));
    break;
    case '4':
      return ((0<<SEG_A)|(1<<SEG_B)|(1<<SEG_C)|(0<<SEG_D)|(0<<SEG_E)|(1<<SEG_F)|(1<<SEG_G)|(0<<SEG_H));
    break;
    case '5':
      return ((1<<SEG_A)|(0<<SEG_B)|(1<<SEG_C)|(1<<SEG_D)|(0<<SEG_E)|(1<<SEG_F)|(1<<SEG_G)|(0<<SEG_H));
    break;
    case '6':
      return ((1<<SEG_A)|(0<<SEG_B)|(1<<SEG_C)|(1<<SEG_D)|(1<<SEG_E)|(1<<SEG_F)|(1<<SEG_G)|(0<<SEG_H));
    break;
    case '7':
      return ((1<<SEG_A)|(1<<SEG_B)|(1<<SEG_C)|(0<<SEG_D)|(0<<SEG_E)|(0<<SEG_F)|(0<<SEG_G)|(0<<SEG_H));
    break;
    case '8':
      return ((1<<SEG_A)|(1<<SEG_B)|(1<<SEG_C)|(1<<SEG_D)|(1<<SEG_E)|(1<<SEG_F)|(1<<SEG_G)|(0<<SEG_H));
    break;
    case '9':
      return ((1<<SEG_A)|(1<<SEG_B)|(1<<SEG_C)|(1<<SEG_D)|(0<<SEG_E)|(1<<SEG_F)|(1<<SEG_G)|(0<<SEG_H));
    break;

    case 'a':
    case 'A':
      return ((1<<SEG_A)|(1<<SEG_B)|(1<<SEG_C)|(0<<SEG_D)|(1<<SEG_E)|(1<<SEG_F)|(1<<SEG_G)|(0<<SEG_H));
    break;
    case 'b':
    case 'B':
      return ((0<<SEG_A)|(0<<SEG_B)|(1<<SEG_C)|(1<<SEG_D)|(1<<SEG_E)|(1<<SEG_F)|(1<<SEG_G)|(0<<SEG_H));
    break;
    case 'c':
      return ((0<<SEG_A)|(0<<SEG_B)|(0<<SEG_C)|(1<<SEG_D)|(1<<SEG_E)|(0<<SEG_F)|(1<<SEG_G)|(0<<SEG_H));
    break;
    case 'C':
      return ((1<<SEG_A)|(0<<SEG_B)|(0<<SEG_C)|(1<<SEG_D)|(1<<SEG_E)|(1<<SEG_F)|(0<<SEG_G)|(0<<SEG_H));
    break;
    case 'd':
    case 'D':
      return ((0<<SEG_A)|(1<<SEG_B)|(1<<SEG_C)|(1<<SEG_D)|(1<<SEG_E)|(0<<SEG_F)|(1<<SEG_G)|(0<<SEG_H));
    break;
    case 't':
    case 'T':
      return ((0<<SEG_A)|(0<<SEG_B)|(0<<SEG_C)|(1<<SEG_D)|(1<<SEG_E)|(1<<SEG_F)|(1<<SEG_G)|(0<<SEG_H));
    break;
    case 'o':
    case 'O':
      return ((0<<SEG_A)|(0<<SEG_B)|(1<<SEG_C)|(1<<SEG_D)|(1<<SEG_E)|(0<<SEG_F)|(1<<SEG_G)|(0<<SEG_H));
    break;
    case 'i':
    case 'I':
      return ((0<<SEG_A)|(0<<SEG_B)|(1<<SEG_C)|(0<<SEG_D)|(0<<SEG_E)|(0<<SEG_F)|(0<<SEG_G)|(0<<SEG_H));
    break;
    case ' ':
      return ((0<<SEG_A)|(0<<SEG_B)|(0<<SEG_C)|(0<<SEG_D)|(0<<SEG_E)|(0<<SEG_F)|(0<<SEG_G)|(0<<SEG_H));
    break;
    case 'r':
    case 'R':
      return ((0<<SEG_A)|(0<<SEG_B)|(0<<SEG_C)|(0<<SEG_D)|(1<<SEG_E)|(0<<SEG_F)|(1<<SEG_G)|(0<<SEG_H));
    break;
    case 'p':
    case 'P':
      return ((1<<SEG_A)|(1<<SEG_B)|(0<<SEG_C)|(0<<SEG_D)|(1<<SEG_E)|(1<<SEG_F)|(1<<SEG_G)|(0<<SEG_H));
    break;
    case '-':
      return ((0<<SEG_A)|(0<<SEG_B)|(0<<SEG_C)|(0<<SEG_D)|(0<<SEG_E)|(0<<SEG_F)|(1<<SEG_G)|(0<<SEG_H));
    break;
    case '^':
      return ((1<<SEG_A)|(0<<SEG_B)|(0<<SEG_C)|(0<<SEG_D)|(0<<SEG_E)|(0<<SEG_F)|(0<<SEG_G)|(0<<SEG_H));
    break;
    case '_':
      return ((0<<SEG_A)|(0<<SEG_B)|(0<<SEG_C)|(1<<SEG_D)|(0<<SEG_E)|(0<<SEG_F)|(0<<SEG_G)|(0<<SEG_H));
    break;
    case 'u':
    case 'U':
      return ((0<<SEG_A)|(1<<SEG_B)|(1<<SEG_C)|(1<<SEG_D)|(1<<SEG_E)|(1<<SEG_F)|(0<<SEG_G)|(0<<SEG_H));
    break;
    case 'n':
    case 'N':
      return ((0<<SEG_A)|(0<<SEG_B)|(1<<SEG_C)|(0<<SEG_D)|(1<<SEG_E)|(0<<SEG_F)|(1<<SEG_G)|(0<<SEG_H));
    break;
    case 'h':
      return ((0<<SEG_A)|(0<<SEG_B)|(1<<SEG_C)|(0<<SEG_D)|(1<<SEG_E)|(1<<SEG_F)|(1<<SEG_G)|(0<<SEG_H));
    break;
    case 'H':
      return ((0<<SEG_A)|(1<<SEG_B)|(1<<SEG_C)|(0<<SEG_D)|(1<<SEG_E)|(1<<SEG_F)|(1<<SEG_G)|(0<<SEG_H));
    break;
    case 'F':
      return ((1<<SEG_A)|(0<<SEG_B)|(0<<SEG_C)|(0<<SEG_D)|(1<<SEG_E)|(1<<SEG_F)|(1<<SEG_G)|(0<<SEG_H));
    break;
	default:
      return IND_BLANK;
    break;
  }    
}

//=============================================================================
void MAX7219_SendByte(uint8_t data)
{
  uint8_t i;
  for(i = 0; i < 8; i++) {
	if (data & 0x80)
	  PORT(MAX7219_DIN) |= MAX7219_DIN_LINE;
	else
	  PORT(MAX7219_DIN) &= ~MAX7219_DIN_LINE;
	data <<= 1;
	PORT(MAX7219_CLK) &= ~MAX7219_CLK_LINE;
	asm("nop");
	asm("nop");
	PORT(MAX7219_CLK) |= MAX7219_CLK_LINE;
  }
  return;
}
//=============================================================================
void MAX7219_SendCmd(uint8_t reg, uint8_t data)
{
  uint8_t j;
  PORT(MAX7219_LOAD) &= ~MAX7219_LOAD_LINE;
  for (j = 0; j < 8; j++) {
	MAX7219_SendByte(reg);
	MAX7219_SendByte(data);
  }
  PORT(MAX7219_LOAD) |= MAX7219_LOAD_LINE;
  return;
}
//=============================================================================
void MAX7219_Init(void)
{
  DDR(MAX7219_LOAD) |= MAX7219_LOAD_LINE;
  DDR(MAX7219_DIN) |= MAX7219_DIN_LINE;
  DDR(MAX7219_CLK) |= MAX7219_CLK_LINE;
  PORT(MAX7219_LOAD) |= MAX7219_LOAD_LINE;
  PORT(MAX7219_DIN) &= ~MAX7219_DIN_LINE;
  PORT(MAX7219_CLK) &= ~MAX7219_CLK_LINE;
  MAX7219_SendCmd(MAX7219_SHUTDOWN, 1);
  MAX7219_SendCmd(MAX7219_DISP_TEST, 0);
  MAX7219_SendCmd(MAX7219_DEC_MODE, 0);
  MAX7219_SendCmd(MAX7219_INTENSITY, 4);
  MAX7219_SendCmd(MAX7219_SCAN_LIMIT, 7);
  MAX7219_clearDisplay();
  return;
}
//=============================================================================
void MAX7219_updateDisplay(void)
{
  uint8_t j;
  for (j = 0; j < 8; j++) {
    MAX7219_SendCmd(MAX7219_DIGIT_0 + digit_pin[j], buffer_display[j]);
  }
}
//=============================================================================
void MAX7219_clearDisplay(void) 
{
  uint8_t j;
  for (j = 0; j < 8; j++) {
    buffer_display[j] = IND_BLANK;
  }
  MAX7219_updateDisplay();
}
//=============================================================================
void MAX7219_printNum(uint8_t pos, signed int x, unsigned char n, unsigned char fillch)
{
  char negative = 0;
  unsigned char i;
  unsigned char s[4];
  if (x < 0) {
    negative = 1;
    x *= -1;
  }
  for (i = 0; i < n; i++) {
    s[n - i - 1] = '0' + (x % 10);
    x /= 10;
  }
  for (i = 0; i < (n - 1); i++) {
    if (s[i] == '0') s[i] = fillch; else break;
  }
  for (i = 0; i < n; i++) {
    if ((pos + i) < 8) {
      buffer_display[pos + i] = MAX7219_lookupCode(s[i]);
    }
  }
  if (negative) {
    buffer_display[pos - 1] = MAX7219_lookupCode('-');
  }
  MAX7219_updateDisplay();
}
//=============================================================================
void MAX7219_printChar(uint8_t pos, char data)
{
  if (pos < 9) {
    buffer_display[pos - 1] = MAX7219_lookupCode(data);
    MAX7219_updateDisplay();
  }
}
//=============================================================================
void MAX7219_printStr(uint8_t pos, char *data)
{
  unsigned char i = pos;
  while(*data) {
    if (i < 9) {
	  buffer_display[i - 1] = MAX7219_lookupCode(*(data++));
      i++;
	} else break;
  }
  MAX7219_updateDisplay();
}
//=============================================================================
void MAX7219_setCommaPos(uint8_t pos, uint8_t comma)
{
  if (comma == 1) {
    buffer_display[pos - 1] |= (1 << SEG_H);
  } else {
    buffer_display[pos - 1] &= (~(1 << SEG_H));
  }
  MAX7219_updateDisplay();
}
//=============================================================================
