/*
 * timer1.c
 *
 *  Created on: Mar 10, 2024
 *      Author: abdalla
 */

#include "timer1.h"
#include "common_macros.h" 	/* To use the macros like SET_BIT */
#include <avr/io.h> 		/* To use Timer1 Registers */
#include <avr/interrupt.h> 	/* For TIMER1 ISR */

/*******************************************************************************
 *                           Global Variables                                  *
 *******************************************************************************/

/* Global variables to hold the address of the call back function in the application */
static volatile void (*g_CallBackFunction)(void) = NULL_PTR;

/*******************************************************************************
 *                      Functions and ISRS Definitions                                  *
 *******************************************************************************/


/* Interrupt Service Routine for timer1 compare mode handle call back function */
ISR(TIMER1_COMPA_vect)
{
	/* check if the pointer is not equal null */
	if (g_CallBackFunction != NULL_PTR)
	{
		/* call back the function passed to you */
		(*g_CallBackFunction)();
	}
}

/* Interrupt Service Routine for timer1 overflow mode to handle call back function */
ISR(TIMER1_OVF_vect)
{
	/* check if the pointer is not equal null */
	if (g_CallBackFunction != NULL_PTR)
	{
		/* call back the function passed to you */
		(*g_CallBackFunction)();
	}
}


/*
 * Description : Function to initialize the Timer1
 * 	1. Set timer1 initial count.
 * 	2. Set the required Mode.
 * 	3. Set the required clock.
 * 	4. Set the required compare value.
 * 	5. Initialize Timer1 Registers.
 */
void TIMER1_init(const TIMER1_ConfigType * config_ptr)
{
	/* Set timer1 initial count as configured */
	TCNT1 = config_ptr->initial_value;

	/*
	 * Configure timer control register TCCR1A
	 * 1. Disconnect OC1A and OC1B  COM1A1=0 COM1A0=0 COM1B0=0 COM1B1=0
	 * 2. FOC1A=1 FOC1B=0 NON PWM mode
	 */
	TCCR1A = (1<<FOC1A);

	/* If the configured mode is overflow mode */
	if (config_ptr->mode == OVERFLOW_MODE)
	{
		/* Set the mode of the timer to Normal mode */
		CLEAR_BIT(TCCR1B, WGM12);

		/* Enable Timer1 overflow Interrupt */
		SET_BIT(TIMSK, TOIE1);
	}

	/* If the configured mode is compare mode */
	else if (config_ptr->mode == COMPARE_MODE)
	{
		/* Set the Compare value as configured */
		OCR1A = config_ptr->compare_value;

		/* Set the mode of the timer to (CTC mode number 4) */
		SET_BIT(TCCR1B, WGM12);

		/* Enable Timer1 Compare A Interrupt */
		SET_BIT(TIMSK, OCIE1A);
	}

	/*
	 * Configure timer control register TCCR1B
	 * Set the prescaler as configured.
	 * Prescaler bits CS10, CS11, CS12.
	 */
	TCCR1B = (TCCR1B & 0xF8)|(config_ptr->prescaler & 0x07);
}


/*
 * Description: Function to disable & stop Timer1.
 */
void TIMER1_deinit(void)
{
	/* Clear All Timer1 Registers Turn off the timer clock */
	TCNT1  = 0;
	OCR1A  = 0;
	TCCR1A = 0;
	TCCR1B = 0;

	/* Disable COMPARE A interrupt */
	CLEAR_BIT(TIMSK, OCIE1A);

	/* Disable OVERFLOW interrupt */
	CLEAR_BIT(TIMSK, TOIE1);
}

/*
 * Description: Function to set the Call Back function address.
 */
void TIMER1_setCallBack(void (*ptr_func)(void))
{
	/* Save the address of the Call back function in a global variable */
	g_CallBackFunction = (void*) ptr_func;
}

