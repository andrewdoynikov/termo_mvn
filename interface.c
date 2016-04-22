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
unsigned char blink = 0;
unsigned char ds_count = 0;
unsigned char one_sensor_flag = 0;
unsigned char dscount = 0;
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
      RTOS_setTask(EVENT_SCAN_SENSOR, 0, 10); 
      RTOS_setTask(EVENT_SHOW_SENSOR, 0, SHOW_TIME); 
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
  static uint8_t chanel = 1;
  switch(event) {
    case EVENT_SHOW_SENSOR:
	  MAX7219_printChar(2, 'd');
	  MAX7219_printChar(3, '-');
	  MAX7219_printChar(4, 0x30 + chanel);
      print_temperature(5, ds18x20GetTemp(chanel));
      MAX7219_setCommaPos(7, 1);
      MAX7219_setCommaPos(4, one_sensor_flag);
      if (!one_sensor_flag) {
        if (chanel < 4) {
	      chanel++; 
	    } else { 
	      chanel = 1;
        }
	  }
    break;
    case EVENT_KEY_PLUS:
    break;
    case EVENT_KEY_SET:
	  RTOS_deleteTask(EVENT_SHOW_SENSOR);
      one_sensor_flag = !one_sensor_flag;
	  if (one_sensor_flag) {
	    if (chanel > 1) chanel--; else chanel = 4;
	  }
      RTOS_setTask(EVENT_SHOW_SENSOR, 0, SHOW_TIME); 
    break;
    case EVENT_KEY_SET_HOLD:
    break;
    case EVENT_KEY_MINUS:
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
    case EVENT_KEY_PLUS:
    case EVENT_KEY_PLUS_LONG:
    case EVENT_KEY_PLUS_HOLD:
    break;
    case EVENT_KEY_SET:
    case EVENT_KEY_SET_LONG:
    case EVENT_KEY_SET_HOLD:
    break;
    case EVENT_KEY_MINUS:
    case EVENT_KEY_MINUS_LONG:
    case EVENT_KEY_MINUS_HOLD:
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
	  dscount = 100;
	  ds_state = 1;
    break;
    case 1:
      ds18x20ReadStratchPad(1);
      ds18x20ReadStratchPad(2);
      ds18x20ReadStratchPad(3);
      ds18x20ReadStratchPad(4);
	  dscount = SCAN_TIME / 10;
	  ds_state = 0;
    break;
  }
}
//=============================================================================
