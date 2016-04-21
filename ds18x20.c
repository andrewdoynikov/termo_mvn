#include <avr/interrupt.h>
#include "ds18x20.h"
#include <util/delay.h>
#include <util/crc16.h>
//=============================================================================
static ds18x20Dev devs[DS18X20_MAX_DEV];
static uint8_t devCount[DS18X20_MAX_DEV] = {0, 0, 0, 0};
//=============================================================================
uint8_t ds18x20IsOnBus(uint8_t chanel)
{
  uint8_t ret = 0;
  cli();
  switch(chanel) {
    case 1:
      DDR(ONE_WIRE_1) |= ONE_WIRE_1_LINE;					/* Pin as output (0) */
      PORT(ONE_WIRE_1) &= ~ONE_WIRE_1_LINE;					/* Set active 0 */
      _delay_us(480);										/* Reset */
      DDR(ONE_WIRE_1) &= ~ONE_WIRE_1_LINE;					/* Pin as input (1) */
      PORT(ONE_WIRE_1) |= ONE_WIRE_1_LINE;					/* Enable pull-up resitor */
      _delay_us(70);										/* Wait for response */
      ret = !(PIN(ONE_WIRE_1) & ONE_WIRE_1_LINE);
    break;
    case 2:
      DDR(ONE_WIRE_2) |= ONE_WIRE_2_LINE;					/* Pin as output (0) */
      PORT(ONE_WIRE_2) &= ~ONE_WIRE_2_LINE;					/* Set active 0 */
      _delay_us(480);										/* Reset */
      DDR(ONE_WIRE_2) &= ~ONE_WIRE_2_LINE;					/* Pin as input (1) */
      PORT(ONE_WIRE_2) |= ONE_WIRE_2_LINE;					/* Enable pull-up resitor */
      _delay_us(70);										/* Wait for response */
      ret = !(PIN(ONE_WIRE_2) & ONE_WIRE_2_LINE);
    break;
    case 3:
      DDR(ONE_WIRE_3) |= ONE_WIRE_3_LINE;					/* Pin as output (0) */
      PORT(ONE_WIRE_3) &= ~ONE_WIRE_3_LINE;					/* Set active 0 */
      _delay_us(480);										/* Reset */
      DDR(ONE_WIRE_3) &= ~ONE_WIRE_3_LINE;					/* Pin as input (1) */
      PORT(ONE_WIRE_3) |= ONE_WIRE_3_LINE;					/* Enable pull-up resitor */
      _delay_us(70);										/* Wait for response */
      ret = !(PIN(ONE_WIRE_3) & ONE_WIRE_3_LINE);
    break;
    case 4:
      DDR(ONE_WIRE_4) |= ONE_WIRE_4_LINE;					/* Pin as output (0) */
      PORT(ONE_WIRE_4) &= ~ONE_WIRE_4_LINE;					/* Set active 0 */
      _delay_us(480);										/* Reset */
      DDR(ONE_WIRE_4) &= ~ONE_WIRE_4_LINE;					/* Pin as input (1) */
      PORT(ONE_WIRE_4) |= ONE_WIRE_4_LINE;					/* Enable pull-up resitor */
      _delay_us(70);										/* Wait for response */
      ret = !(PIN(ONE_WIRE_4) & ONE_WIRE_4_LINE);
    break;
  }
  _delay_us(410);
  devCount[chanel - 1] = ret;
  sei();
  return ret;
}
//=============================================================================
static void ds18x20SendBit(uint8_t bit, uint8_t chanel)
{
  cli();
  switch(chanel) {
    case 1:
      DDR(ONE_WIRE_1) |= ONE_WIRE_1_LINE;					/* Pin as output (0) */
      PORT(ONE_WIRE_1) &= ~ONE_WIRE_1_LINE;					/* Set active 0 */
      _delay_us(6);
      if (!bit)	_delay_us(54);
      DDR(ONE_WIRE_1) &= ~ONE_WIRE_1_LINE;					/* Pin as input (1) */
      PORT(ONE_WIRE_1) |= ONE_WIRE_1_LINE;					/* Enable pull-up resitor */
      _delay_us(10);
      if (bit) _delay_us(54);
    break;
    case 2:
      DDR(ONE_WIRE_2) |= ONE_WIRE_2_LINE;					/* Pin as output (0) */
      PORT(ONE_WIRE_2) &= ~ONE_WIRE_2_LINE;					/* Set active 0 */
      _delay_us(6);
      if (!bit)	_delay_us(54);
      DDR(ONE_WIRE_2) &= ~ONE_WIRE_2_LINE;					/* Pin as input (1) */
      PORT(ONE_WIRE_2) |= ONE_WIRE_2_LINE;					/* Enable pull-up resitor */
      _delay_us(10);
      if (bit) _delay_us(54);
    break;
    case 3:
      DDR(ONE_WIRE_3) |= ONE_WIRE_3_LINE;					/* Pin as output (0) */
      PORT(ONE_WIRE_3) &= ~ONE_WIRE_3_LINE;					/* Set active 0 */
      _delay_us(6);
      if (!bit)	_delay_us(54);
      DDR(ONE_WIRE_3) &= ~ONE_WIRE_3_LINE;					/* Pin as input (1) */
      PORT(ONE_WIRE_3) |= ONE_WIRE_3_LINE;					/* Enable pull-up resitor */
      _delay_us(10);
      if (bit) _delay_us(54);
    break;
    case 4:
      DDR(ONE_WIRE_4) |= ONE_WIRE_4_LINE;					/* Pin as output (0) */
      PORT(ONE_WIRE_4) &= ~ONE_WIRE_4_LINE;					/* Set active 0 */
      _delay_us(6);
      if (!bit)	_delay_us(54);
      DDR(ONE_WIRE_4) &= ~ONE_WIRE_4_LINE;					/* Pin as input (1) */
      PORT(ONE_WIRE_4) |= ONE_WIRE_4_LINE;					/* Enable pull-up resitor */
      _delay_us(10);
      if (bit) _delay_us(54);
    break;
  }
  sei();
  return;
}
//=============================================================================
static uint8_t ds18x20GetBit(uint8_t chanel)
{
  uint8_t ret;
  cli();
  switch(chanel) {
    case 1:
      DDR(ONE_WIRE_1) |= ONE_WIRE_1_LINE;				/* Pin as output (0) */
      PORT(ONE_WIRE_1) &= ~ONE_WIRE_1_LINE;				/* Set active 0 */
      _delay_us(6);										/* Strob */
      DDR(ONE_WIRE_1) &= ~ONE_WIRE_1_LINE;				/* Pin as input (1) */
      PORT(ONE_WIRE_1) |= ONE_WIRE_1_LINE;				/* Enable pull-up resitor */
      _delay_us(9);
      ret = PIN(ONE_WIRE_1) & ONE_WIRE_1_LINE;
      _delay_us(55);
    break;
    case 2:
      DDR(ONE_WIRE_2) |= ONE_WIRE_2_LINE;				/* Pin as output (0) */
      PORT(ONE_WIRE_2) &= ~ONE_WIRE_2_LINE;				/* Set active 0 */
      _delay_us(6);										/* Strob */
      DDR(ONE_WIRE_2) &= ~ONE_WIRE_2_LINE;				/* Pin as input (1) */
      PORT(ONE_WIRE_2) |= ONE_WIRE_2_LINE;				/* Enable pull-up resitor */
      _delay_us(9);
      ret = PIN(ONE_WIRE_2) & ONE_WIRE_2_LINE;
      _delay_us(55);
    break;
    case 3:
      DDR(ONE_WIRE_3) |= ONE_WIRE_3_LINE;				/* Pin as output (0) */
      PORT(ONE_WIRE_3) &= ~ONE_WIRE_3_LINE;				/* Set active 0 */
      _delay_us(6);										/* Strob */
      DDR(ONE_WIRE_3) &= ~ONE_WIRE_3_LINE;				/* Pin as input (1) */
      PORT(ONE_WIRE_3) |= ONE_WIRE_3_LINE;				/* Enable pull-up resitor */
      _delay_us(9);
      ret = PIN(ONE_WIRE_3) & ONE_WIRE_3_LINE;
      _delay_us(55);
    break;
    case 4:
      DDR(ONE_WIRE_4) |= ONE_WIRE_4_LINE;				/* Pin as output (0) */
      PORT(ONE_WIRE_4) &= ~ONE_WIRE_4_LINE;				/* Set active 0 */
      _delay_us(6);										/* Strob */
      DDR(ONE_WIRE_4) &= ~ONE_WIRE_4_LINE;				/* Pin as input (1) */
      PORT(ONE_WIRE_4) |= ONE_WIRE_4_LINE;				/* Enable pull-up resitor */
      _delay_us(9);
      ret = PIN(ONE_WIRE_4) & ONE_WIRE_4_LINE;
      _delay_us(55);
    break;
  }
  sei();
  return ret;
}
//=============================================================================
void ds18x20SendByte(uint8_t byte, uint8_t chanel)
{
  uint8_t i;
  for (i = 0; i < 8; i++) {
	ds18x20SendBit(byte & 0x01, chanel);
	byte >>= 1;
  }
  return;
}
//=============================================================================
uint8_t ds18x20GetByte(uint8_t chanel)
{
  uint8_t i, ret;
  ret = 0;
  for (i = 0; i < 8; i++) {
	ret >>= 1;
	if (ds18x20GetBit(chanel)) ret |= 0x80;
  }
  return ret;
}
//=============================================================================
void ds18x20ReadStratchPad(uint8_t chanel)
{
  uint8_t j;
  uint8_t crc;
  static uint8_t arr[DS18X20_SCRATCH_LEN];
  if (ds18x20IsOnBus(chanel))
  {
    ds18x20SendByte(DS18X20_CMD_SKIP_ROM, chanel);
    ds18x20SendByte(DS18X20_CMD_READ_SCRATCH, chanel);
    crc = 0;
	for (j = 0; j < DS18X20_SCRATCH_LEN; j++) {
	  arr[j] = ds18x20GetByte(chanel);
	  crc = _crc_ibutton_update(crc, arr[j]);
	}
  	if (crc == 0) {
	  for (j = 0; j < DS18X20_SCRATCH_TEMP_LEN; j++) { devs[chanel - 1].sp[j] = arr[j]; }
	}
  }
  return;
}
//=============================================================================
void ds18x20ConvertTemp(uint8_t chanel)
{
  if (ds18x20IsOnBus(chanel))
  {
    ds18x20SendByte(DS18X20_CMD_SKIP_ROM, chanel);
    ds18x20SendByte(DS18X20_CMD_CONVERT, chanel);
  }
#ifdef DS18X20_PARASITE_POWER
  /* Set active 1 on port for at least 750ms as parasitic power */
  switch(chanel) {
    case 1:
      PORT(ONE_WIRE_1) |= ONE_WIRE_1_LINE;
      DDR(ONE_WIRE_1) |= ONE_WIRE_1_LINE;
    break;
    case 2:
      PORT(ONE_WIRE_2) |= ONE_WIRE_2_LINE;
      DDR(ONE_WIRE_2) |= ONE_WIRE_2_LINE;
    break;
    case 3:
      PORT(ONE_WIRE_3) |= ONE_WIRE_3_LINE;
      DDR(ONE_WIRE_3) |= ONE_WIRE_3_LINE;
    break;
    case 4:
      PORT(ONE_WIRE_4) |= ONE_WIRE_4_LINE;
      DDR(ONE_WIRE_4) |= ONE_WIRE_4_LINE;
    break;
  }
#endif
  return;
}
//=============================================================================
int16_t ds18x20GetTemp(uint8_t chanel)
{
  int16_t ret;
  uint16_t tmp_L = devs[chanel - 1].sp[0], tmp_H = devs[chanel - 1].sp[1];
  int16_t tmp = 0;
  ret = ((tmp_H << 8) | tmp_L);
  tmp = (tmp_L & 15);
  tmp = (tmp << 1) + (tmp << 3); // Умножаем на 10
  tmp = (tmp >> 4);              //делим на 16 или умножаем на 0.0625  
  ret = (ret / 16 * 10) + tmp;
  return ret;
}
//=============================================================================
uint8_t ds18x20GetDevCount(uint8_t chanel)
{
  return devCount[chanel - 1];
}
//=============================================================================
void ds18x20SetDevCount(uint8_t chanel, uint8_t val)
{
  devCount[chanel - 1] = val;
}
//=============================================================================
