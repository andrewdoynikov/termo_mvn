#include <avr/eeprom.h>
//=============================================================================
#include "pins.h"
#include "rtos.h"
#include "list_event.h"
#include "kbd.h"
#include "max7219.h"
#include "interface.h"
//=============================================================================
extern void (*pState)(unsigned char event);
//=============================================================================
unsigned char blink = 0;
//=============================================================================
#define SET_STATE(a) pState = a // макрос для смены состояния
//=============================================================================
void run_start(unsigned char event)
{
  switch(event) {
    case EVENT_KEY_PLUS:
    case EVENT_KEY_PLUS_LONG:
    case EVENT_KEY_PLUS_HOLD:
	  MAX7219_printNum(1, event, 3, '0');
    break;
    case EVENT_KEY_SET:
    case EVENT_KEY_SET_LONG:
    case EVENT_KEY_SET_HOLD:
	  MAX7219_printNum(1, event, 3, '0');
    break;
    case EVENT_KEY_MINUS:
    case EVENT_KEY_MINUS_LONG:
    case EVENT_KEY_MINUS_HOLD:
	  MAX7219_printNum(1, event, 3, '0');
    break;
	default:
	  events_default(event);
    break;
  }    
}
//=============================================================================
void run_main(unsigned char event)
{
}
//=============================================================================
void run_menu(unsigned char event)
{
}
//=============================================================================
void events_default(unsigned char event)
{
  switch(event) {
    case EVENT_TIMER_SECOND:
	  blink = !blink;
      MAX7219_setCommaPos(8, blink);
    break;
    case EVENT_KEY_POLL: 
	  KBD_scan();
    break;

  }
}
//=============================================================================
