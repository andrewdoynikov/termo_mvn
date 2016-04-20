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
      RTOS_setTask(EVENT_PRINT_COUNT, 0, 0);
    break;
    case EVENT_PRINT_COUNT:
	  if (ds18x20GetDevCount(1) == 1) { ds_count++; MAX7219_printChar(5, '1'); } else { MAX7219_printChar(5, ' '); }
	  if (ds18x20GetDevCount(2) == 1) { ds_count++; MAX7219_printChar(6, '1'); } else { MAX7219_printChar(6, ' '); }
	  if (ds18x20GetDevCount(3) == 1) { ds_count++; MAX7219_printChar(7, '1'); } else { MAX7219_printChar(7, ' '); }
	  if (ds18x20GetDevCount(4) == 1) { ds_count++; MAX7219_printChar(8, '1'); } else { MAX7219_printChar(8, ' '); }
	  if (ds_count == 0) {
        RTOS_setTask(EVENT_SEARCH_SENSOR, 3000, 0);
	  } else {
	    MAX7219_printNum(3, ds_count, 1, ' ');
        RTOS_setTask(EVENT_START_CONVERTIONS, 10, 0);
	  }
    break;
    case EVENT_START_CONVERTIONS:
	  if (ds18x20GetDevCount(1) == 1) { ds18x20ConvertTemp(1); }
	  if (ds18x20GetDevCount(2) == 1) { ds18x20ConvertTemp(2); }
	  if (ds18x20GetDevCount(3) == 1) { ds18x20ConvertTemp(3); }
	  if (ds18x20GetDevCount(4) == 1) { ds18x20ConvertTemp(4); }
      RTOS_setTask(EVENT_GET_TEMPERATURE, 1000, 0);
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
      SET_STATE(run_main);
      RTOS_setTask(EVENT_SHOW_TEMP_1, 0, 0);
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
    case EVENT_SHOW_TEMP_1:
	  MAX7219_printChar(2, 'd');
	  MAX7219_printChar(3, '-');
	  MAX7219_printChar(4, '1');
      print_temperature(5, ds18x20GetTemp(1));
      MAX7219_setCommaPos(7,1);
      RTOS_setTask(EVENT_SHOW_TEMP_2, 1000, 0);
    break;
    case EVENT_SHOW_TEMP_2:
	  MAX7219_printChar(2, 'd');
	  MAX7219_printChar(3, '-');
	  MAX7219_printChar(4, '2');
      print_temperature(5, ds18x20GetTemp(2));
      MAX7219_setCommaPos(7,1);
      RTOS_setTask(EVENT_SHOW_TEMP_3, 1000, 0);
    break;
    case EVENT_SHOW_TEMP_3:
	  MAX7219_printChar(2, 'd');
	  MAX7219_printChar(3, '-');
	  MAX7219_printChar(4, '3');
      print_temperature(5, ds18x20GetTemp(3));
      MAX7219_setCommaPos(7,1);
      RTOS_setTask(EVENT_SHOW_TEMP_4, 1000, 0);
    break;
    case EVENT_SHOW_TEMP_4:
	  MAX7219_printChar(2, 'd');
	  MAX7219_printChar(3, '-');
	  MAX7219_printChar(4, '4');
      print_temperature(5, ds18x20GetTemp(4));
      MAX7219_setCommaPos(7,1);
      RTOS_setTask(EVENT_SHOW_TEMP_1, 1000, 0);
    break;
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

  }
}
//=============================================================================
