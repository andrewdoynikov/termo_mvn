#ifndef DS18X20_H
#define DS18X20_H
//=============================================================================
#include <inttypes.h>
#include "pins.h"
//=============================================================================
#define DS18X20_PARASITE_POWER
//=============================================================================
/* DS18X20 commands */
#define DS18X20_CMD_SEARCH_ROM		0xF0
#define DS18X20_CMD_READ_ROM		0x33
#define DS18X20_CMD_MATCH_ROM		0x55
#define DS18X20_CMD_SKIP_ROM		0xCC
#define DS18X20_CMD_ALARM_SEARCH	0xEC
#define DS18X20_CMD_CONVERT			0x44
#define DS18X20_CMD_WRITE_SCRATCH	0x4E
#define DS18X20_CMD_READ_SCRATCH	0xBE
#define DS18X20_CMD_COPY_SCRATCH	0x48
#define DS18X20_CMD_RECALL_EE		0xB8
#define DS18X20_CMD_READ_POWER		0xB4
//=============================================================================
#define DS18S20_FAMILY_CODE			0x10
#define DS18B20_FAMILY_CODE			0x28
//=============================================================================
#define DS18X20_MAX_DEV				4
#define DS18X20_SCRATCH_LEN			9
#define DS18X20_ID_LEN				8
//=============================================================================
typedef union {
  int16_t temp;
  uint8_t port;
  struct {
	uint8_t sp[DS18X20_SCRATCH_LEN];
	uint8_t id[DS18X20_ID_LEN];
  };
} ds18x20Dev;
//=============================================================================
uint8_t ds18x20IsOnBus(uint8_t chanel);
void ds18x20SendByte(uint8_t byte, uint8_t chanel);
uint8_t ds18x20GetByte(uint8_t chanel);
int16_t ds18x20GetTemp(uint8_t chanel);
uint8_t ds18x20GetDevCount(uint8_t chanel);
void ds18x20ConvertTemp(uint8_t chanel);
void ds18x20GetAllTemps(void);
//=============================================================================
#endif /* DS18X20_H */
