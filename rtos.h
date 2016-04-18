#ifndef RTOS_H
#define RTOS_H	          
//=============================================================================
#include "interface.h"
//=============================================================================
#define MAX_TASKS       10              // Количество задач
#define TIMER_COUNTER   TCNT2           // Регистр-счетчик
#define TIMER_START     130             // Константа для таймера Т2
#define TIMER_ISR       TIMER2_OVF_vect
//Начальное состояние
#define INITIAL_STATE   run_start
//=============================================================================
typedef struct task
{   
  unsigned char event;  // событие
  unsigned int delay;   // задержка перед первым запуском задачи
  unsigned int period;  // период запуска задачи
  unsigned char run;    // флаг готовности задачи к запуску
} task;
//=============================================================================
void RTOS_init(void);
void RTOS_setTask(unsigned char event, unsigned int taskDelay, unsigned int taskPeriod);
void RTOS_deleteTask(unsigned char event);
void RTOS_dispatchTask(void);
//=============================================================================
#endif
