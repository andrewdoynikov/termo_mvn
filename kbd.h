#ifndef BUT_H
#define BUT_H
//=============================================================================
#include <avr/io.h>
#include <avr/interrupt.h>
#include <stdint.h>
//=============================================================================
#include "pins.h"
//=============================================================================
// инициализация. вызывается в начале программы
void KBD_init(void);
void KBD_scan(void);
void KBD_set_type(uint8_t val);
unsigned char debounce_sw_plus(void);
unsigned char debounce_sw_set(void);
unsigned char debounce_sw_minus(void);
//=============================================================================
#endif //BUT_H

