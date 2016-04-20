#ifndef EVENT_LIST_H
#define EVENT_LIST_H
//=============================================================================
#define EVENT_NULL       	   	0
//=============================================================================
#define EVENT_KEY_PLUS  		10
#define EVENT_KEY_PLUS_LONG		11
#define EVENT_KEY_PLUS_HOLD		12

#define EVENT_KEY_SET       	20
#define EVENT_KEY_SET_LONG  	21
#define EVENT_KEY_SET_HOLD  	22

#define EVENT_KEY_MINUS  		30
#define EVENT_KEY_MINUS_LONG	31
#define EVENT_KEY_MINUS_HOLD	32
//=============================================================================
// Сообщения начального запуска
#define EVENT_SEARCH_SENSOR		100
#define EVENT_PRINT_COUNT		101
#define EVENT_RUN_MAIN			102
#define EVENT_START_CONVERTIONS	103
#define EVENT_GET_TEMPERATURE	104
#define EVENT_SHOW_TEMP_1		105
#define EVENT_SHOW_TEMP_2		106
#define EVENT_SHOW_TEMP_3		107
#define EVENT_SHOW_TEMP_4		108
//=============================================================================
// Системные сообщения
//=============================================================================
#define EVENT_KEY_POLL     		200
#define EVENT_TIMER_SECOND		201
#define EVENT_TIMER_SAVE		202
//=============================================================================
#endif // EVENT_LIST_H
