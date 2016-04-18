#ifndef RTOS_H
#define RTOS_H	          
//=============================================================================
#include "interface.h"
//=============================================================================
#define MAX_TASKS       10              // ���������� �����
#define TIMER_COUNTER   TCNT2           // �������-�������
#define TIMER_START     130             // ��������� ��� ������� �2
#define TIMER_ISR       TIMER2_OVF_vect
//��������� ���������
#define INITIAL_STATE   run_start
//=============================================================================
typedef struct task
{   
  unsigned char event;  // �������
  unsigned int delay;   // �������� ����� ������ �������� ������
  unsigned int period;  // ������ ������� ������
  unsigned char run;    // ���� ���������� ������ � �������
} task;
//=============================================================================
void RTOS_init(void);
void RTOS_setTask(unsigned char event, unsigned int taskDelay, unsigned int taskPeriod);
void RTOS_deleteTask(unsigned char event);
void RTOS_dispatchTask(void);
//=============================================================================
#endif
