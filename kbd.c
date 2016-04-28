//=============================================================================
#include "pins.h"
#include "rtos.h"
#include "list_event.h"
#include "kbd.h"
//=============================================================================
unsigned char kbd_type = 1;
//=============================================================================
void KBD_init(void)
{
  DDR(BUT_PLUS) &= ~BUT_PLUS_LINE;
  PORT(BUT_PLUS) |= BUT_PLUS_LINE;
  DDR(BUT_SET) &= ~BUT_SET_LINE;
  PORT(BUT_SET) |= BUT_SET_LINE;
  DDR(BUT_MINUS) &= ~BUT_MINUS_LINE;
  PORT(BUT_MINUS) |= BUT_MINUS_LINE;
  kbd_type = 1;
}
//=============================================================================
void KBD_set_type(uint8_t val)
{
  kbd_type = val;
}
//=============================================================================
unsigned char debounce_sw_plus(void)
{
  static uint16_t state1 = 0; //holds present state
  state1 = (state1 << 1) | (! bit_is_clear(PIN(BUT_PLUS), BUT_PLUS_PIN)) | 0xE000;
  if (state1 == 0xF000) return 1;
  if (state1 == 0xE000) return 2;
  return 0;
}
//=============================================================================
unsigned char debounce_sw_set(void)
{
  static uint16_t state2 = 0; //holds present state
  state2 = (state2 << 1) | (! bit_is_clear(PIN(BUT_SET), BUT_SET_PIN)) | 0xE000;
  if (state2 == 0xF000) return 1;
  if (state2 == 0xE000) return 2;
  return 0;
}
//=============================================================================
unsigned char debounce_sw_minus(void)
{
  static uint16_t state3 = 0; //holds present state
  state3 = (state3 << 1) | (! bit_is_clear(PIN(BUT_MINUS), BUT_MINUS_PIN)) | 0xE000;
  if (state3 == 0xF000) return 1;
  if (state3 == 0xE000) return 2;
  return 0;
}
//=============================================================================
void KBD_scan(void)
{
  if (kbd_type == 1) {
    if (debounce_sw_plus() == 1) { BEEPER_TICK(); RTOS_setTask(EVENT_KEY_PLUS, 0, 0); }
    if (debounce_sw_minus() == 1) { BEEPER_TICK(); RTOS_setTask(EVENT_KEY_MINUS, 0, 0); }
    RTOS_setTask(EVENT_KEY_POLL, 10, 0);         // запускаем сканирование клавиатуры
  } else {
    if (debounce_sw_plus() >= 1) { BEEPER_TICK(); RTOS_setTask(EVENT_KEY_PLUS, 0, 0); }
    if (debounce_sw_minus() >= 1) { BEEPER_TICK(); RTOS_setTask(EVENT_KEY_MINUS, 0, 0); }
    RTOS_setTask(EVENT_KEY_POLL, 50, 0);         // запускаем сканирование клавиатуры
  }
  if (debounce_sw_set() == 1) { 
    BEEPER_TICK(); 
	RTOS_setTask(EVENT_KEY_SET, 0, 0); 
    RTOS_setTask(EVENT_KEY_POLL, 10, 0);         // запускаем сканирование клавиатуры
  }
}
//=============================================================================
 
