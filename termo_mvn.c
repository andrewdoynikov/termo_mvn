#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include <avr/eeprom.h>
#include <avr/pgmspace.h>
//=============================================================================
#include "pins.h"
#include "list_event.h"
#include "rtos.h"
#include "interface.h"
#include "kbd.h"
#include "max7219.h"
//=============================================================================
#define	DEBUG	0
//=============================================================================
int main(void)
{
  MAX7219_Init();
  KBD_init();
  OUT_1_init();
  OUT_2_init();
  BEEPER_init();
  RTOS_init();
  RTOS_setTask(EVENT_TIMER_SECOND, 0, 1000); // ��������� ��������� ������,
  RTOS_setTask(EVENT_KEY_POLL, 0, 5);        // ��������� ������������ ����������
  RTOS_setTask(EVENT_SEARCH_SENSOR, 0, 0);   // ��������� ������������ ��������

#if (DEBUG == 1)
  while(1) {

  MAX7219_clearDisplay();
  uint16_t j;
  for (j = 0; j < 999; j++) {
    MAX7219_printNum(5, j, 3, ' ');
    MAX7219_setCommaPos(6,1);
    _delay_ms(100);
  }
  MAX7219_clearDisplay();
  }
#endif
  sei();
  while(1) {
    RTOS_dispatchTask();        
  }
}
//=============================================================================
