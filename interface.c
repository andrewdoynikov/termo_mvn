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
#define MMENU_MAX 3
char *setmenu[] = {
 "D-1 ",
 "D-2 ",
 "BR- ",
 "End "
};
#define MM_D1	      	0
#define MM_D2	      	1
#define MM_BRIGHTNES   	2
#define MM_END	   		3
#define T_MAX			0 	
#define T_MIN			1 	
#define T_OTSLED_MAX	1 	
#define T_OTSLED_MIN	0 	
#define T_HEATER		1 	
#define T_FAN			0 	
//=============================================================================
uint8_t blink = 1, blink05 = 1, wait_menu = WAIT_MENU_TIME;
uint8_t ds_count = 0, one_sensor_flag = 0, dscount = 0, error_cnt = ERROR_TIME;
static uint8_t chanel = 1;
uint8_t brightnes = 2;
int16_t temps[2][2] = { {0, 0}, {0, 0} };
uint8_t types[2] = {1, 1};
uint8_t status[2] = {T_OTSLED_MAX, T_OTSLED_MAX};
uint8_t outs[2] = {0, 0};
uint8_t m_menu = MM_D1, n_edit = 0;
//=============================================================================
#define SET_STATE(a) pState = a // ������ ��� ����� ���������
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
	if (ds18x20GetDevCount(1) == 1) {
	  if (ds18x20GetTemp(1) < temps[0][0]) {
	    status[0] = T_OTSLED_MAX;
		if (types[0] == T_HEATER) set_outport(1, 1); else set_outport(1, 0);
	  } else {
	    status[0] = T_OTSLED_MIN;
		if (types[0] == T_HEATER) set_outport(1, 0); else set_outport(1, 1);
	  }
    } else {
      set_outport(1, 0);
	}
	if (ds18x20GetDevCount(2) == 1) {
	  if (ds18x20GetTemp(2) < temps[1][0]) {
	    status[1] = T_OTSLED_MAX;
		if (types[1] == T_HEATER) set_outport(2, 1); else set_outport(2, 0);
	  } else {
	    status[1] = T_OTSLED_MIN;
		if (types[1] == T_HEATER) set_outport(2, 0); else set_outport(2, 1);
	  }
    } else {
      set_outport(2, 0);
	}
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
void set_outport(uint8_t chanel, uint8_t val)
{
  if (chanel == 1) {
    if (val == 1) {
	  OUT_1_1();
	  outs[0] = 1;
	} else {
	  OUT_1_0();
	  outs[0] = 0;
	}
  }
  if (chanel == 2) {
    if (val == 1) {
	  OUT_2_1();
	  outs[1] = 1;
	} else {
	  OUT_2_0();
	  outs[1] = 0;
	}
  }
}
//=============================================================================
void check_temp(uint8_t chanel)
{
  int16_t t = ds18x20GetTemp(chanel);
  if (status[chanel - 1] == T_OTSLED_MAX) {
 // ����������� ���������� ���������
    if (t > temps[chanel - 1][T_MAX]) {
      status[chanel - 1] = T_OTSLED_MIN;
      if (types[chanel - 1] == T_HEATER) {
        set_outport(chanel, 0);
	  } else {
        set_outport(chanel, 1);
	  }
    }
  } else {
   // ����������� ���������� ��������
    if (t < temps[chanel - 1][T_MIN]) {
      status[chanel - 1] = T_OTSLED_MAX;
      if (types[chanel - 1] == T_HEATER) {
        set_outport(chanel, 1);
	  } else {
        set_outport(chanel, 0);
	  }
    }
  }
}
//=============================================================================
void run_main(unsigned char event)
{
  switch(event) {
    case EVENT_SHOW_SENSOR:
	  if (ds18x20GetDevCount(chanel) == 1) {
	    MAX7219_printChar(3, 'd');
	    MAX7219_printChar(4, 0x30 + chanel);
        print_temperature(5, ds18x20GetTemp(chanel));
		if (chanel < 3) {
		  if (types[chanel - 1]) {
	        MAX7219_printChar(1, 'H');
		  } else {
	        MAX7219_printChar(1, 'F');
		  }
          MAX7219_setCommaPos(1, outs[chanel - 1]);
		} else {
	      MAX7219_printChar(1, ' ');
          MAX7219_setCommaPos(1, 0);
		}
        MAX7219_setCommaPos(7, 1);
        MAX7219_setCommaPos(4, one_sensor_flag);
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
    case EVENT_KEY_MINUS:
    case EVENT_KEY_PLUS:
	  RTOS_deleteTask(EVENT_SHOW_SENSOR);
      one_sensor_flag = !one_sensor_flag;
	  if (one_sensor_flag) {
	    if (chanel > 1) chanel--; else chanel = 4;
	  }
      RTOS_setTask(EVENT_SHOW_SENSOR, 0, 0); 
    break;
    case EVENT_KEY_SET:
	  m_menu = MM_D1;
	  n_edit = 0;
      MAX7219_clearDisplay();
      SET_STATE(run_menu);
      wait_menu = WAIT_MENU_TIME;
	  show_menu();
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
	  if (wait_menu > 0) wait_menu--; else {
        BEEPER_TICK(); 
        RTOS_setTask(EVENT_EXIT_MENU, 0, 0); 
	  }
	  if (m_menu == MM_BRIGHTNES) {
        MAX7219_printNum(4, brightnes, 2, ' ');
	  } else {
        MAX7219_printStr(4, "   ");
	  }
    break;
    case EVENT_KEY_PLUS:
      wait_menu = WAIT_MENU_TIME;
	  if (m_menu > 0) m_menu--; else m_menu = MMENU_MAX;
	  show_menu();
      MAX7219_printStr(5, "    ");
    break;
    case EVENT_KEY_SET:
      wait_menu = WAIT_MENU_TIME;
	  n_edit = 0;
      MAX7219_clearDisplay();
	  if (m_menu == MM_D1) {
		KBD_set_type(0);
        SET_STATE(run_set_1);
	  } else if (m_menu == MM_D2) {
		KBD_set_type(0);
        SET_STATE(run_set_2);
	  } else if (m_menu == MM_BRIGHTNES) {
		KBD_set_type(1);
        SET_STATE(run_set_brightnes);
        MAX7219_printNum(4, brightnes, 2, ' ');
	  } else if (m_menu == MM_END) {
		KBD_set_type(1);
        RTOS_setTask(EVENT_EXIT_MENU, 0, 0); 
	  }
    break;
    case EVENT_EXIT_MENU:
      MAX7219_clearDisplay();
	  chanel = 1;
	  if (ds18x20GetTemp(1) < temps[0][0]) status[0] = T_OTSLED_MAX; else status[0] = T_OTSLED_MIN;
	  if (ds18x20GetTemp(2) < temps[1][0]) status[1] = T_OTSLED_MAX; else status[1] = T_OTSLED_MIN;
      check_temp(1);
      check_temp(2);
      SET_STATE(run_main);
      RTOS_setTask(EVENT_SHOW_SENSOR, 0, SHOW_TIME); 
    break;
    case EVENT_KEY_MINUS:
      wait_menu = WAIT_MENU_TIME;
      MAX7219_printStr(5, "    ");
	  if (m_menu < MMENU_MAX) m_menu++; else m_menu = 0;
	  show_menu();
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
	  dscount = 10;
	  ds_state = 2;
    break;
    case 2:
	if (ds18x20GetDevCount(1) == 1) {
      ds18x20GetTemp(1);
      check_temp(1);
    } else {
      set_outport(1, 0);
    }
	if (ds18x20GetDevCount(2) == 1) {
      ds18x20GetTemp(2);
      check_temp(2);
    } else {
      set_outport(2, 0);
    }
	if (ds18x20GetDevCount(3) == 1) {
      ds18x20GetTemp(3);
    }
	if (ds18x20GetDevCount(4) == 1) {
      ds18x20GetTemp(4);
    }
	  dscount = SCAN_TIME / 100;
	  ds_state = 0;
    break;
  }
}
//=============================================================================
void save_max_temp_1(uint16_t temp)
{
  eeprom_write_word((uint16_t*)MAX_TEMP_1_ADDR, temp);
}
//=============================================================================
void save_max_temp_2(uint16_t temp)
{
  eeprom_write_word((uint16_t*)MAX_TEMP_2_ADDR, temp);
}
//=============================================================================
void save_min_temp_1(uint16_t temp)
{
  eeprom_write_word((uint16_t*)MIN_TEMP_1_ADDR, temp);
}
//=============================================================================
void save_min_temp_2(uint16_t temp)
{
  eeprom_write_word((uint16_t*)MIN_TEMP_2_ADDR, temp);
}
//=============================================================================
int16_t load_max_temp_1(void)
{
  int16_t tu;
  uint16_t t = eeprom_read_word((uint16_t*)MAX_TEMP_1_ADDR);
  tu = (int16_t)t;
  if ((tu > 800) && (tu < -45)) tu = 250;
  temps[0][0] = (int16_t)tu;
  return (int16_t)tu;
}
//=============================================================================
int16_t load_min_temp_1(void)
{
  int16_t tu;
  uint16_t t = eeprom_read_word((uint16_t*)MIN_TEMP_1_ADDR);
  tu = (int16_t)t;
  if ((tu > 800) && (tu < -45)) tu = 250;
  temps[0][1] = (int16_t)tu;
  return (int16_t)tu;
}
//=============================================================================
int16_t load_max_temp_2(void)
{
  int16_t tu;
  uint16_t t = eeprom_read_word((uint16_t*)MAX_TEMP_2_ADDR);
  tu = (int16_t)t;
  if ((tu > 800) && (tu < -45)) tu = 250;
  temps[1][0] = (int16_t)tu;
  return (int16_t)tu;
}
//=============================================================================
int16_t load_min_temp_2(void)
{
  int16_t tu;
  uint16_t t = eeprom_read_word((uint16_t*)MIN_TEMP_2_ADDR);
  tu = (int16_t)t;
  if ((tu > 800) && (tu < -45)) tu = 250;
  temps[1][1] = (int16_t)tu;
  return (int16_t)tu;
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
void show_menu(void)
{
  MAX7219_printStr(1, setmenu[m_menu]);
}
//=============================================================================
void run_set_1(unsigned char event)
{
  switch(event) {
    case EVENT_TIMER_SECOND:
	  blink = !blink;
	  if (wait_menu > 0) wait_menu--; else {
        BEEPER_TICK(); 
	    KBD_set_type(1);
        MAX7219_clearDisplay();
        SET_STATE(run_menu);
	    show_menu();
	  }
    break;
    case EVENT_TIMER_SECOND05:
	  blink05 = !blink05;
	  show_set_temp(0);
    break;
    case EVENT_KEY_MINUS:
      wait_menu = WAIT_MENU_TIME;
	  blink05 = 1;
	  if (n_edit == 0) {
        if (temps[0][0] > -550) {
		  temps[0][0]--;
		}
	  }
	  if (n_edit == 1) {
        if (temps[0][1] > -550) {
		  temps[0][1]--;
		}
	  }
	  if (n_edit == 2) {
        types[0] = !types[0];
	  }
	  show_set_temp(0);
    break;
    case EVENT_KEY_SET:
      wait_menu = WAIT_MENU_TIME;
      MAX7219_clearDisplay();
	  if (n_edit < 2) n_edit++; else RTOS_setTask(EVENT_EXIT_MENU, 0, 0); 
	  if (n_edit == 2) KBD_set_type(1); else KBD_set_type(0);
	  show_set_temp(0);
    break;
    case EVENT_EXIT_MENU:
      wait_menu = WAIT_MENU_TIME;
	  if (temps[0][0] > temps[0][1]) {
        MAX7219_clearDisplay();
        SET_STATE(run_menu);
        save_max_temp_1(temps[0][0]);
        save_min_temp_1(temps[0][1]);
	    save_type_1(types[0]);
        show_menu();
	    KBD_set_type(1);
	  } else {
        MAX7219_clearDisplay();
        SET_STATE(run_error);
        error_cnt = ERROR_TIME;
        RTOS_setTask(EVENT_TIMER_SECOND05, 0, 500); 
	  }
    break;
    case EVENT_KEY_PLUS:
      wait_menu = WAIT_MENU_TIME;
	  blink05 = 1;
	  if (n_edit == 0) {
        if (temps[0][0] < 790) {
		  temps[0][0]++;
		}
	  }
	  if (n_edit == 1) {
        if (temps[0][1] < 790) {
		  temps[0][1]++;
		}
	  }
	  if (n_edit == 2) {
        types[0] = !types[0];
	  }
	  show_set_temp(0);
    break;
	default:
	  events_default(event);
    break;
  }    
}
//=============================================================================
void run_set_2(unsigned char event)
{
  switch(event) {
    case EVENT_TIMER_SECOND:
	  blink = !blink;
	  if (wait_menu > 0) wait_menu--; else {
        BEEPER_TICK(); 
	    KBD_set_type(1);
        MAX7219_clearDisplay();
        SET_STATE(run_menu);
	    show_menu();
	  }
    break;
    case EVENT_TIMER_SECOND05:
	  blink05 = !blink05;
	  show_set_temp(1);
    break;
    case EVENT_KEY_MINUS:
      wait_menu = WAIT_MENU_TIME;
	  blink05 = 1;
	  if (n_edit == 0) {
        if (temps[1][0] > -550) {
		  temps[1][0]--;
		}
	  }
	  if (n_edit == 1) {
        if (temps[1][1] > -550) {
		  temps[1][1]--;
		}
	  }
	  if (n_edit == 2) {
        types[1] = !types[1];
	  }
	  show_set_temp(1);
    break;
    case EVENT_KEY_SET:
      wait_menu = WAIT_MENU_TIME;
      MAX7219_clearDisplay();
	  if (n_edit < 2) n_edit++; else RTOS_setTask(EVENT_EXIT_MENU, 0, 0); 
	  if (n_edit == 2) KBD_set_type(1); else KBD_set_type(0);
	  show_set_temp(1);
    break;
    case EVENT_EXIT_MENU:
      wait_menu = WAIT_MENU_TIME;
	  if (temps[1][0] > temps[1][1]) {
        MAX7219_clearDisplay();
        SET_STATE(run_menu);
        save_max_temp_2(temps[1][0]);
        save_min_temp_2(temps[1][1]);
	    save_type_2(types[1]);
        show_menu();
	    KBD_set_type(1);
	  } else {
        MAX7219_clearDisplay();
        SET_STATE(run_error);
        error_cnt = ERROR_TIME;
        RTOS_setTask(EVENT_TIMER_SECOND05, 0, 500); 
	  }
    break;
    case EVENT_KEY_PLUS:
      wait_menu = WAIT_MENU_TIME;
	  blink05 = 1;
	  if (n_edit == 0) {
        if (temps[1][0] < 790) {
		  temps[1][0]++;
		}
	  }
	  if (n_edit == 1) {
        if (temps[1][1] < 790) {
		  temps[1][1]++;
		}
	  }
	  if (n_edit == 2) {
        types[1] = !types[1];
	  }
	  show_set_temp(1);
    break;
	default:
	  events_default(event);
    break;
  }    
}
//=============================================================================
void show_set_temp(uint8_t temp)
{
  if (n_edit == 0) {
    if (blink05) {
      print_temperature(1, temps[temp][0]);
      MAX7219_setCommaPos(3, 1);
    } else {
      MAX7219_printStr(1, "    ");
    }
    print_temperature(5, temps[temp][1]);
    MAX7219_setCommaPos(7, 1);
  }
  if (n_edit == 1) {
    if (blink05) {
      print_temperature(5, temps[temp][1]);
      MAX7219_setCommaPos(7, 1);
    } else {
      MAX7219_printStr(5, "    ");
	}
    print_temperature(1, temps[temp][0]);
    MAX7219_setCommaPos(3, 1);
  }
  if (n_edit == 2) {
    MAX7219_printStr(1, "out-");
    if (blink05) {
	  if (types[temp]) MAX7219_printChar(5, 'H'); else MAX7219_printChar(5, 'F');
    } else {
	  MAX7219_printChar(5, ' ');
	}
  }
}
//=============================================================================
void run_set_brightnes(unsigned char event)
{
  switch(event) {
    case EVENT_TIMER_SECOND:
	  blink = !blink;
	  if (wait_menu > 0) wait_menu--; else {
        MAX7219_clearDisplay();
        SET_STATE(run_menu);
	  }
    break;
    case EVENT_TIMER_SECOND05:
	  blink05 = !blink05;
	  if (blink05) {
        MAX7219_printNum(4, brightnes, 2, ' ');
	  } else {
        MAX7219_printStr(4, "   ");
	  }
    break;
    case EVENT_KEY_MINUS:
      wait_menu = WAIT_MENU_TIME;
	  if (brightnes > 0) {
	    brightnes--;
	    MAX7219_SendCmd(MAX7219_INTENSITY, brightnes);
      }
    break;
    case EVENT_KEY_SET:
      wait_menu = WAIT_MENU_TIME;
	  save_brightnes(brightnes);
      MAX7219_clearDisplay();
      SET_STATE(run_menu);
      show_menu();
    break;
    case EVENT_KEY_PLUS:
      wait_menu = WAIT_MENU_TIME;
	  if (brightnes < 15) {
	    brightnes++;
	    MAX7219_SendCmd(MAX7219_INTENSITY, brightnes);
      }
    break;
	default:
	  events_default(event);
    break;
  }    
}
//=============================================================================
void run_error(unsigned char event)
{
  switch(event) {
    case EVENT_TIMER_SECOND:
	  blink = !blink;
	  if (error_cnt > 0) error_cnt--; else {
        wait_menu = WAIT_MENU_TIME;
        SET_STATE(run_menu);
        MAX7219_clearDisplay();
        show_menu();
	  }
    break;
    case EVENT_TIMER_SECOND05:
	  blink05 = !blink05;
	  if (blink05) {
        MAX7219_printStr(1, "Err ");
	  } else {
        MAX7219_printStr(1, "    ");
	  }
    break;
    case EVENT_KEY_SET:
    case EVENT_KEY_MINUS:
    case EVENT_KEY_PLUS:
      wait_menu = WAIT_MENU_TIME;
      SET_STATE(run_menu);
      MAX7219_clearDisplay();
      show_menu();
    break;
  }
}
//=============================================================================


