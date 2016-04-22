#ifndef INTERFACE_H
#define INTERFACE_H
//=============================================================================
void run_start(unsigned char event);
void run_main(unsigned char event);
void run_menu(unsigned char event);
void events_default(unsigned char event);
//=============================================================================
void print_temperature(uint8_t pos, int temp);
void DS18x20_scan(void);
//=============================================================================
// EEPROM
//=============================================================================
//=============================================================================
#endif
