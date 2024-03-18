/*
 * timer1.h
 *
 *  Created on: Mar 10, 2024
 *      Author: abdalla
 */

#ifndef TIMER1_H_
#define TIMER1_H_

#include "std_types.h"

/*******************************************************************************
 *                       Types Declaration                                     *
 *******************************************************************************/

/* Enum defining different modes of operation for Timer1 */
typedef enum
{
    OVERFLOW_MODE,    	/* Normal mode of operation for Timer1 */
    COMPARE_MODE    	/* Compare mode of operation for Timer1 */
}TIMER1_Mode;

/* Enum defining different prescaler options for Timer1 */
typedef enum
{
	NO_CLOCK,       	/* No clock source TIMER STOP */
	F_CPU_CLOCK,    	/* Clock source equals CPU frequency */
	F_CPU_8,        	/* Clock source equals CPU frequency divided by 8 */
	F_CPU_64,       	/* Clock source equals CPU frequency divided by 64 */
	F_CPU_256,      	/* Clock source equals CPU frequency divided by 256 */
	F_CPU_1024,      	/* Clock source equals CPU frequency divided by 1024 */
	EXTERNAL_FALLING, 	/* External clock source on T1 pin. Clock on falling edge.*/
	EXTERNAL_RISING 	/* External clock source on T1 pin. Clock on rising edge.*/
}TIMER1_Prescaler;

/* Structure defining configuration parameters for Timer1 */
typedef struct
{
    TIMER1_Mode mode; 			/* Timer mode (normal mode, compare mode) */
    TIMER1_Prescaler prescaler; /* Prescaler value for timer */
    uint16 initial_value; 		/* Initial value for timer counter */
    uint16 compare_value; 		/* Value to compare with timer counter for comparison */
}TIMER1_ConfigType;

/*******************************************************************************
 *                      Functions Prototypes                                   *
 *******************************************************************************/

/*
 * Description : Function to initialize the Timer1
 * 	1. Set timer1 initial count.
 * 	2. Set the required Mode.
 * 	3. Set the required clock.
 * 	4. Set the required compare value.
 * 	5. Initialize Timer1 Registers.
 */
void TIMER1_init(const TIMER1_ConfigType * config_ptr);

/*
 * Description: Function to disable & stop Timer1.
 */
void TIMER1_deinit(void);

/*
 * Description: Function to set the Call Back function address.
 */
void TIMER1_setCallBack(void (*ptr_func)(void));



#endif /* SRC_TIMER1_H_ */
