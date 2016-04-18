#include <avr/io.h>
#include <avr/interrupt.h>
#include "rtos.h"
#include "kbd.h"
#include "list_event.h"
//=============================================================================
void (*pState)(unsigned char event) = INITIAL_STATE;
unsigned char timerInterrupt = 0; // Флаг прерывания таймера
volatile static task TaskArray[MAX_TASKS]; // очередь задач
volatile static unsigned char arrayTail;   // "хвост" очереди
//=============================================================================
void RTOS_init()
{
  TCCR2B |= (1 << CS22);          // прескалер - 64
  TIFR2 = (1 << TOV2);            // очищаем флаг прерывания таймера Т2
  TIMSK2 |= (1 << TOIE2);         // разрешаем прерывание по переполнению
  TIMER_COUNTER = TIMER_START;   // загружаем начальное зн. в счетный регистр
  arrayTail = 0;                 // "хвост" в 0
}
//=============================================================================
void RTOS_setTask (unsigned char event, unsigned int taskDelay, unsigned int taskPeriod)
{
  unsigned char i;
  if (!event) return;
  for(i = 0; i < arrayTail; i++)              // поиск задачи в текущем списке
  {
    if (TaskArray[i].event == event)          // если нашли, то обновляем переменные
    {
      TaskArray[i].delay  = taskDelay;
      TaskArray[i].period = taskPeriod;
      if (!taskDelay) {                       // если задержка запуска == 0
        TaskArray[i].run = 1;                 // то сразу устанавливаем флаг запуска
      } else TaskArray[i].run = 0;   
      return;                                 // обновив, выходим
    }
  }
  if (arrayTail < MAX_TASKS)                  // если такой задачи в списке нет 
  {                                           // и есть место,то добавляем
    TaskArray[arrayTail].event  = event;
    TaskArray[arrayTail].delay  = taskDelay;
    TaskArray[arrayTail].period = taskPeriod;
    if (!taskDelay) {                         // если задержка запуска == 0
      TaskArray[i].run = 1;                   // то сразу устанавливаем флаг запуска
    } else TaskArray[i].run = 0;   
    arrayTail++;                              // увеличиваем "хвост"
  } 
}
//=============================================================================
void RTOS_deleteTask (unsigned char event)
{
  unsigned char i;
  for (i = 0; i < arrayTail; i++)             // проходим по списку задач
  {
    if (TaskArray[i].event == event)          // если задача в списке найдена
    {
      if (i != (arrayTail - 1))               // переносим последнюю задачу
      {                                       // на место удаляемой
        TaskArray[i] = TaskArray[arrayTail - 1];
      }
      arrayTail--;                            // уменьшаем указатель "хвоста"
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
    timerInterrupt = 0;                      // сбрасываем флаг прерывания
    for (i = 0; i < arrayTail; i++)          // проходим по списку задач
    {      if (TaskArray[i].delay == 0) {               // если время до выполнения истекло
        TaskArray[i].run = 1;                // взводим флаг запуска,
      } else TaskArray[i].delay--;           // иначе уменьшаем время
    }
  }
  for (i = 0; i < arrayTail; i++)            // проходим по списку задач
  {
    if (TaskArray[i].run == 1)               // если флаг на выполнение взведен,
    {                                        // запоминаем задачу, т.к. во
      temp = TaskArray[i].event;             // время выполнения может 
      if(TaskArray[i].period == 0)                 
      {  
        cli();
        RTOS_deleteTask(TaskArray[i].event); // удаляем задачу из списка,
        sei();
      } else {
        TaskArray[i].run = 0;                // иначе снимаем флаг запуска
        if (!TaskArray[i].delay)             // если задача не изменила задержку
        {                                    // задаем ее
          TaskArray[i].delay = TaskArray[i].period - 1; 
        }                                    // задача для себя может сделать паузу  
      }
      pState(temp);                          // выполняем задачу
    }
  }
}
//=============================================================================
ISR(TIMER_ISR)
{
  TIMER_COUNTER = TIMER_START;               // задаем начальное значение таймера
  timerInterrupt = 1;                        // устанавливаем флаг прерывания 
}
//=============================================================================
//=============================================================================
