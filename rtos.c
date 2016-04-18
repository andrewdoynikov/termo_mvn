#include <avr/io.h>
#include <avr/interrupt.h>
#include "rtos.h"
#include "kbd.h"
#include "list_event.h"
//=============================================================================
void (*pState)(unsigned char event) = INITIAL_STATE;
unsigned char timerInterrupt = 0; // ���� ���������� �������
volatile static task TaskArray[MAX_TASKS]; // ������� �����
volatile static unsigned char arrayTail;   // "�����" �������
//=============================================================================
void RTOS_init()
{
  TCCR2B |= (1 << CS22);          // ��������� - 64
  TIFR2 = (1 << TOV2);            // ������� ���� ���������� ������� �2
  TIMSK2 |= (1 << TOIE2);         // ��������� ���������� �� ������������
  TIMER_COUNTER = TIMER_START;   // ��������� ��������� ��. � ������� �������
  arrayTail = 0;                 // "�����" � 0
}
//=============================================================================
void RTOS_setTask (unsigned char event, unsigned int taskDelay, unsigned int taskPeriod)
{
  unsigned char i;
  if (!event) return;
  for(i = 0; i < arrayTail; i++)              // ����� ������ � ������� ������
  {
    if (TaskArray[i].event == event)          // ���� �����, �� ��������� ����������
    {
      TaskArray[i].delay  = taskDelay;
      TaskArray[i].period = taskPeriod;
      if (!taskDelay) {                       // ���� �������� ������� == 0
        TaskArray[i].run = 1;                 // �� ����� ������������� ���� �������
      } else TaskArray[i].run = 0;   
      return;                                 // �������, �������
    }
  }
  if (arrayTail < MAX_TASKS)                  // ���� ����� ������ � ������ ��� 
  {                                           // � ���� �����,�� ���������
    TaskArray[arrayTail].event  = event;
    TaskArray[arrayTail].delay  = taskDelay;
    TaskArray[arrayTail].period = taskPeriod;
    if (!taskDelay) {                         // ���� �������� ������� == 0
      TaskArray[i].run = 1;                   // �� ����� ������������� ���� �������
    } else TaskArray[i].run = 0;   
    arrayTail++;                              // ����������� "�����"
  } 
}
//=============================================================================
void RTOS_deleteTask (unsigned char event)
{
  unsigned char i;
  for (i = 0; i < arrayTail; i++)             // �������� �� ������ �����
  {
    if (TaskArray[i].event == event)          // ���� ������ � ������ �������
    {
      if (i != (arrayTail - 1))               // ��������� ��������� ������
      {                                       // �� ����� ���������
        TaskArray[i] = TaskArray[arrayTail - 1];
      }
      arrayTail--;                            // ��������� ��������� "������"
      return;
    }
  }
}
//=============================================================================
void RTOS_dispatchTask()
{
  unsigned char i, temp;
  if (timerInterrupt)
  {  
    timerInterrupt = 0;                      // ���������� ���� ����������
    for (i = 0; i < arrayTail; i++)          // �������� �� ������ �����
    {      if (TaskArray[i].delay == 0) {               // ���� ����� �� ���������� �������
        TaskArray[i].run = 1;                // ������� ���� �������,
      } else TaskArray[i].delay--;           // ����� ��������� �����
    }
  }
  for (i = 0; i < arrayTail; i++)            // �������� �� ������ �����
  {
    if (TaskArray[i].run == 1)               // ���� ���� �� ���������� �������,
    {                                        // ���������� ������, �.�. ��
      temp = TaskArray[i].event;             // ����� ���������� ����� 
      if(TaskArray[i].period == 0)                 
      {  
        cli();
        RTOS_deleteTask(TaskArray[i].event); // ������� ������ �� ������,
        sei();
      } else {
        TaskArray[i].run = 0;                // ����� ������� ���� �������
        if (!TaskArray[i].delay)             // ���� ������ �� �������� ��������
        {                                    // ������ ��
          TaskArray[i].delay = TaskArray[i].period - 1; 
        }                                    // ������ ��� ���� ����� ������� �����  
      }
      pState(temp);                          // ��������� ������
    }
  }
}
//=============================================================================
ISR(TIMER_ISR)
{
  TIMER_COUNTER = TIMER_START;               // ������ ��������� �������� �������
  timerInterrupt = 1;                        // ������������� ���� ���������� 
}
//=============================================================================
//=============================================================================
