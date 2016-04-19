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
  devCount[chanel - 1] = ret;
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
void ds18x20GetAllTemps(void)
{
  uint8_t i, j;
  uint8_t crc;
  static uint8_t arr[DS18X20_SCRATCH_LEN];
  for (i = 0; i < DS18X20_MAX_DEV - 1; i++)
  {
	if (ds18x20IsOnBus(i + 1))
	{
	  ds18x20SendByte(DS18X20_CMD_READ_SCRATCH, i + 1);
  	  crc = 0;
	  for (j = 0; j < DS18X20_SCRATCH_LEN; j++) {
		arr[j] = ds18x20GetByte(i + 1);
		crc = _crc_ibutton_update(crc, arr[j]);
	  }
  	  if (crc == 0)
	  {
		for (j = 0; j < DS18X20_SCRATCH_LEN; j++)  devs[i].sp[j] = arr[j];
	  }
	}
  }
  return;
}
//=============================================================================
void ds18x20ConvertTemp(uint8_t chanel)
{
  ds18x20SendByte(DS18X20_CMD_SKIP_ROM, chanel);
  ds18x20SendByte(DS18X20_CMD_CONVERT, chanel);
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
  int16_t ret = devs[chanel - 1].temp;
  if (devs[chanel - 1].id[0] == 0x28) /* DS18B20 */
	ret = ret * 5 / 8;
  else if (devs[chanel - 1].id[0] == 0x10) /* DS18S20 */
	ret = ret * 5;
	/* Return value is in 0.1°C units */
  return ret;
}
//=============================================================================
uint8_t ds18x20GetDevCount(uint8_t chanel)
{
  return devCount[chanel - 1];
}
//=============================================================================
