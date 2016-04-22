#ifndef PINS_H
#define PINS_H
//=============================================================================
#include <avr/io.h>
#include <util/delay.h>
//=============================================================================
#define CONCAT(x,y)			x ## y
#define DDR(x)				CONCAT(DDR,x)
#define PORT(x)				CONCAT(PORT,x)
#define PIN(x)				CONCAT(PIN,x)
//=============================================================================
// Датчики 18х20
#define ONE_WIRE_1			B
#define ONE_WIRE_1_PIN		1
#define ONE_WIRE_1_LINE		(1 << ONE_WIRE_1_PIN)
#define ONE_WIRE_2			B
#define ONE_WIRE_2_PIN		0
#define ONE_WIRE_2_LINE		(1 << ONE_WIRE_2_PIN)
#define ONE_WIRE_3			D
#define ONE_WIRE_3_PIN		7
#define ONE_WIRE_3_LINE		(1 << ONE_WIRE_3_PIN)
#define ONE_WIRE_4			B
#define ONE_WIRE_4_PIN		2
#define ONE_WIRE_4_LINE		(1 << ONE_WIRE_4_PIN)
//=============================================================================
// Выходы каналов управления
#define OUT_1				D
#define OUT_1_PIN			3
#define OUT_1_LINE			(1 << OUT_1_PIN)
#define OUT_1_0()			PORT(OUT_1) &= ~(OUT_1_LINE)
#define OUT_1_1()			PORT(OUT_1) |= (OUT_1_LINE)
#define OUT_1_init()		DDR(OUT_1) |= (OUT_1_LINE); OUT_1_0();

#define OUT_2				D
#define OUT_2_PIN			2
#define OUT_2_LINE			(1 << OUT_1_PIN)
#define OUT_2_0()			PORT(OUT_2) &= ~(OUT_2_LINE)
#define OUT_2_1()			PORT(OUT_2) |= (OUT_2_LINE)
#define OUT_2_init()		DDR(OUT_2) |= (OUT_2_LINE); OUT_2_0();
//=============================================================================
// Спикер
#define BEEPER				C
#define BEEPER_PIN			0
#define BEEPER_LINE			(1 << BEEPER_PIN)
#define BEEPER_OFF()		PORT(BEEPER) &= ~(BEEPER_LINE)
#define BEEPER_ON()			PORT(BEEPER) |= (BEEPER_LINE)
#define BEEPER_init()		DDR(BEEPER) |= (BEEPER_LINE); BEEPER_OFF();
#define BEEPER_TICK()		BEEPER_ON(); _delay_ms(10); BEEPER_OFF();
#define BEEPER_TICK_LONG()	BEEPER_ON(); _delay_ms(80); BEEPER_OFF();
//=============================================================================
// Кнопки
#define BUT_PLUS			D
#define BUT_PLUS_PIN		4
#define BUT_PLUS_LINE		(1 << BUT_PLUS_PIN)
#define BUT_SET				D
#define BUT_SET_PIN			5
#define BUT_SET_LINE		(1 << BUT_SET_PIN)
#define BUT_MINUS			D
#define BUT_MINUS_PIN		6
#define BUT_MINUS_LINE		(1 << BUT_MINUS_PIN)
//=============================================================================
// MAX7219
#define MAX7219_LOAD		C
#define MAX7219_LOAD_PIN	1
#define MAX7219_LOAD_LINE	(1 << MAX7219_LOAD_PIN)
#define MAX7219_CLK			C
#define MAX7219_CLK_PIN		2
#define MAX7219_CLK_LINE	(1 << MAX7219_CLK_PIN)
#define MAX7219_DIN			C
#define MAX7219_DIN_PIN		3
#define MAX7219_DIN_LINE	(1 << MAX7219_DIN_PIN)

#define MAX_DIG_1			3
#define MAX_DIG_2			7
#define MAX_DIG_3			5
#define MAX_DIG_4			1
#define MAX_DIG_5			2
#define MAX_DIG_6			6
#define MAX_DIG_7			4
#define MAX_DIG_8			0

#define SEG_H				7
#define SEG_A				6
#define SEG_B				5
#define SEG_C				4
#define SEG_D				3
#define SEG_E				2
#define SEG_F				1
#define SEG_G				0
#define IND_BLANK   		((0<<SEG_A)|(0<<SEG_B)|(0<<SEG_C)|(0<<SEG_D)|(0<<SEG_E)|(0<<SEG_F)|(0<<SEG_G)|(0<<SEG_H))
//=============================================================================
#define SHOW_TIME			2000
#define SCAN_TIME			2000
//=============================================================================

#endif /* PINS_H */
