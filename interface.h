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
void save_max_temp_1(int16_t temp);
void save_max_temp_2(int16_t temp);
void save_min_temp_1(int16_t temp);
void save_min_temp_2(int16_t temp);
void save_brightnes(uint8_t val);
void save_type_1(uint8_t temp);
void save_type_2(uint8_t temp);
int16_t load_max_temp1(void);
int16_t load_max_temp2(void);
int16_t load_min_temp1(void);
int16_t load_min_temp2(void);
uint8_t load_brightnes(void);
uint8_t load_type1(void);
uint8_t load_type2(void);
//=============================================================================
#endif
