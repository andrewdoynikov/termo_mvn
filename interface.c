#include <avr/eeprom.h>
//=============================================================================
#include "pins.h"
#include "rtos.h"
#include "list_event.h"
#include "kbd.h"
#include "max7219.h"
#include "ds18x20.h"
#include "interface.h"
//=============================================================================
extern void (*pState)(unsigned char event);
//=============================================================================
uint8_t blink = 0;
uint8_t ds_count = 0, one_sensor_flag = 0, dscount = 0;
static uint8_t chanel = 1;
uint8_t brightnes = 2;
int16_t temps[2][2] = { {0, 0}, {0, 0} };
uint8_t types[2] = {1, 1};
//=============================================================================
#define SET_STATE(a) pState = a // макрос для смены состояния
//=============================================================================
void print_temperature(uint8_t pos, int temp)
{
  uint8_t sign = 0;
  if (temp < 0) { 
    sign = 1;
	temp = -temp;
  }
  MAX7219_printNum(pos, temp, 3, ' ');
  MAX7219_setCommaPos(pos + 2,1);
  if (sign == 1) MAX7219_printChar(pos, '-');
}
//=============================================================================
void run_start(unsigned char event)
{
  switch(event) {
    case EVENT_SEARCH_SENSOR:
	  MAX7219_printStr(1, "dc");
	  MAX7219_printChar(3, '-');
	  ds_count = 0;
      ds18x20IsOnBus(1);
      ds18x20IsOnBus(2);
      ds18x20IsOnBus(3);
      ds18x20IsOnBus(4);
	  if (ds18x20GetDevCount(1) == 1) { ds_count++; MAX7219_printChar(5, '1'); } else { MAX7219_printChar(5, ' '); }
	  if (ds18x20GetDevCount(2) == 1) { ds_count++; MAX7219_printChar(6, '1'); } else { MAX7219_printChar(6, ' '); }
	  if (ds18x20GetDevCount(3) == 1) { ds_count++; MAX7219_printChar(7, '1'); } else { MAX7219_printChar(7, ' '); }
	  if (ds18x20GetDevCount(4) == 1) { ds_count++; MAX7219_printChar(8, '1'); } else { MAX7219_printChar(8, ' '); }
	  if (ds_count == 0) {
        RTOS_setTask(EVENT_SEARCH_SENSOR, 3000, 0);
	  } else {
	    MAX7219_printNum(3, ds_count, 1, ' ');
	    if (ds18x20GetDevCount(1) == 1) { ds18x20ConvertTemp(1); }
	    if (ds18x20GetDevCount(2) == 1) { ds18x20ConvertTemp(2); }
	    if (ds18x20GetDevCount(3) == 1) { ds18x20ConvertTemp(3); }
	    if (ds18x20GetDevCount(4) == 1) { ds18x20ConvertTemp(4); }
        RTOS_setTask(EVENT_GET_TEMPERATURE, 1000, 0);
	  }
    break;
    case EVENT_GET_TEMPERATURE:
	  if (ds18x20GetDevCount(1) == 1) { ds18x20ReadStratchPad(1); }
	  if (ds18x20GetDevCount(2) == 1) { ds18x20ReadStratchPad(2); }
	  if (ds18x20GetDevCount(3) == 1) { ds18x20ReadStratchPad(3); }
	  if (ds18x20GetDevCount(4) == 1) { ds18x20ReadStratchPad(4); }
      RTOS_setTask(EVENT_RUN_MAIN, 100, 0);
    break;
    case EVENT_RUN_MAIN:
      MAX7219_clearDisplay();
      RTOS_setTask(EVENT_SCAN_SENSOR, 0, 100); 
      RTOS_setTask(EVENT_SHOW_SENSOR, 0, 0); 
      SET_STATE(run_main);
    break;
	default:
	  events_default(event);
    break;
  }    
}
//=============================================================================
void run_main(unsigned char event)
{
  switch(event) {
    case EVENT_SHOW_SENSOR:
	  if (ds18x20GetDevCount(chanel) == 1) {
	    MAX7219_printChar(2, 'd');
	    MAX7219_printChar(3, '-');
	    MAX7219_printChar(4, 0x30 + chanel);
        print_temperature(5, ds18x20GetTemp(chanel));
		if (chanel < 3) {
		  if (types[chanel - 1]) {
	        MAX7219_printChar(1, '^');
		  } else {
	        MAX7219_printChar(1, '_');
		  }
		} else {
	      MAX7219_printChar(1, ' ');
		}
        MAX7219_setCommaPos(7, 1);
        MAX7219_setCommaPos(4, one_sensor_flag);
        RTOS_setTask(EVENT_OUT_CHECK, 0, 0); 
	  }
      if (!one_sensor_flag) {
        if (chanel < 4) {
          chanel++; 
	    } else { 
	      chanel = 1;
        }
	  }
	  if (ds18x20GetDevCount(chanel) == 0) {
        RTOS_setTask(EVENT_SHOW_SENSOR, 0, 0); 
	  } else {
        RTOS_setTask(EVENT_SHOW_SENSOR, SHOW_TIME, 0); 
	  }
    break;
    case EVENT_KEY_PLUS:
    break;
    case EVENT_KEY_SET:
	  RTOS_deleteTask(EVENT_SHOW_SENSOR);
	  BEEPER_TICK();
      one_sensor_flag = !one_sensor_flag;
	  if (one_sensor_flag) {
	    if (chanel > 1) chanel--; else chanel = 4;
	  }
      RTOS_setTask(EVENT_SHOW_SENSOR, 0, 0); 
    break;
    case EVENT_KEY_SET_LONG:
      MAX7219_clearDisplay();
      SET_STATE(run_menu);
    break;
    case EVENT_KEY_MINUS:
    break;
    case EVENT_OUT_CHECK:
    break;
	default:
	  events_default(event);
    break;
  }    
}
//=============================================================================
void run_menu(unsigned char event)
{
  switch(event) {
    case EVENT_TIMER_SECOND:
	  blink = !blink;
      MAX7219_setCommaPos(1, blink);
    break;
    case EVENT_KEY_PLUS:
    break;
    case EVENT_KEY_SET:
    break;
    case EVENT_KEY_SET_LONG:
      MAX7219_clearDisplay();
	  chanel = 1;
      SET_STATE(run_main);
      RTOS_setTask(EVENT_SHOW_SENSOR, 0, SHOW_TIME); 
    break;
    case EVENT_KEY_MINUS:
    break;
	default:
	  events_default(event);
    break;
  }    
}
//=============================================================================
void events_default(unsigned char event)
{
  switch(event) {
    case EVENT_TIMER_SECOND:
	  blink = !blink;
    break;
    case EVENT_KEY_POLL: 
	  KBD_scan();
    break;
    case EVENT_SCAN_SENSOR: 
	  DS18x20_scan();
    break;
  }
}
//=============================================================================
void DS18x20_scan(void)
{
  static uint8_t ds_state = 0;
  if (dscount > 0) { 
    dscount--;
	return;
  }
  switch(ds_state) {
    case 0:
      ds18x20ConvertTemp(1);
      ds18x20ConvertTemp(2);
      ds18x20ConvertTemp(3);
      ds18x20ConvertTemp(4);
	  dscount = 10;
	  ds_state = 1;
    break;
    case 1:
      ds18x20ReadStratchPad(1);
      ds18x20ReadStratchPad(2);
      ds18x20ReadStratchPad(3);
      ds18x20ReadStratchPad(4);
	  dscount = SCAN_TIME / 100;
	  ds_state = 0;
    break;
  }
}
//=============================================================================
void save_max_temp_1(int16_t temp)
{
  eeprom_write_byte((uint8_t*)MAX_TEMP_1_ADDR, ((temp & 0xF0) >> 8));
  eeprom_write_byte((uint8_t*)MAX_TEMP_1_ADDR + 1, (temp & 0x0F));
}
//=============================================================================
int16_t load_max_temp1(void)
{
  uint8_t th = eeprom_read_byte((uint8_t*)(uint8_t*)MAX_TEMP_1_ADDR);
  uint8_t tl = eeprom_read_byte((uint8_t*)(uint8_t*)MAX_TEMP_1_ADDR + 1);
  uint16_t t = th;
  t = (t << 8) + tl;
  if (t > 800) t = 250;
  temps[0][0] = (int16_t)t;
  return (int16_t)t;
}
//=============================================================================
void save_max_temp_2(int16_t temp)
{
  eeprom_write_byte((uint8_t*)MAX_TEMP_2_ADDR, ((temp & 0xF0) >> 8));
  eeprom_write_byte((uint8_t*)MAX_TEMP_2_ADDR + 1, (temp & 0x0F));
}
//=============================================================================
int16_t load_max_temp2(void)
{
  uint8_t th = eeprom_read_byte((uint8_t*)(uint8_t*)MAX_TEMP_2_ADDR);
  uint8_t tl = eeprom_read_byte((uint8_t*)(uint8_t*)MAX_TEMP_2_ADDR + 1);
  uint16_t t = th;
  t = (t << 8) + tl;
  if (t > 800) t = 250;
  temps[1][0] = (int16_t)t;
  return (int16_t)t;
}
//=============================================================================
void save_min_temp_1(int16_t temp)
{
  eeprom_write_byte((uint8_t*)MIN_TEMP_1_ADDR, ((temp & 0xF0) >> 8));
  eeprom_write_byte((uint8_t*)MIN_TEMP_1_ADDR + 1, (temp & 0x0F));
}
//=============================================================================
int16_t load_min_temp1(void)
{
  uint8_t th = eeprom_read_byte((uint8_t*)(uint8_t*)MIN_TEMP_1_ADDR);
  uint8_t tl = eeprom_read_byte((uint8_t*)(uint8_t*)MIN_TEMP_1_ADDR + 1);
  uint16_t t = th;
  t = (t << 8) + tl;
  if (t > 800) t = 230;
  temps[0][1] = (int16_t)t;
  return (int16_t)t;
}
//=============================================================================
void save_min_temp_2(int16_t temp)
{
  eeprom_write_byte((uint8_t*)MIN_TEMP_2_ADDR, ((temp & 0xF0) >> 8));
  eeprom_write_byte((uint8_t*)MIN_TEMP_2_ADDR + 1, (temp & 0x0F));
}
//=============================================================================
int16_t load_min_temp2(void)
{
  uint8_t th = eeprom_read_byte((uint8_t*)(uint8_t*)MIN_TEMP_2_ADDR);
  uint8_t tl = eeprom_read_byte((uint8_t*)(uint8_t*)MIN_TEMP_2_ADDR + 1);
  uint16_t t = th;
  t = (t << 8) + tl;
  if (t > 800) t = 230;
  temps[1][1] = (int16_t)t;
  return (int16_t)t;
}
//=============================================================================
void save_brightnes(uint8_t val)
{
  eeprom_write_byte((uint8_t*)BRIGHTNES_ADDR, val);
}
//=============================================================================
uint8_t load_brightnes(void)
{
  brightnes = eeprom_read_byte((uint8_t*)BRIGHTNES_ADDR);
  if (brightnes > 15) brightnes = 2;
  return brightnes;
}
//=============================================================================
void save_type_1(uint8_t temp)
{
  eeprom_write_byte((uint8_t*)OUT_TYPE_1_ADDR, temp);
}
//=============================================================================
uint8_t load_type1(void)
{
  uint8_t t = eeprom_read_byte((uint8_t*)OUT_TYPE_1_ADDR);
  if (t > 1) t = 1;
  types[0] = t;
  return t;
}
//=============================================================================
void save_type_2(uint8_t temp)
{
  eeprom_write_byte((uint8_t*)OUT_TYPE_2_ADDR, temp);
}
//=============================================================================
uint8_t load_type2(void)
{
  uint8_t t = eeprom_read_byte((uint8_t*)OUT_TYPE_2_ADDR);
  if (t > 1) t = 1;
  types[1] = t;
  return t;
}
//=============================================================================
