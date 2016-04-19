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
        RTOS_setTask(EVENT_RUN_MAIN, 3000, 0);
	  }
    break;
    case EVENT_RUN_MAIN:
      MAX7219_clearDisplay();
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
