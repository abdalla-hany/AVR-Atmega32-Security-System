/*
 * pwm.c
 *	Description: source file for the PWM MINI driver
 *  Created on: Feb 21, 2024
 *      Author: abdalla
 */

#include "pwm.h"
#include "gpio.h"
#include <avr/io.h>

/*******************************************************************************
 *                      Functions Definitions                                  *
 *******************************************************************************/
/*
 * Description :
 * Function responsible To start the pwm signal using timer0 compare mode
 */
void PWM_Timer0_Start(uint8 duty_cycle)
{
	GPIO_setupPinDirection(PORTB_ID, PIN3_ID,PIN_OUTPUT);

	TCNT0 = 0; /* Set Timer Initial value */

	if (duty_cycle > 100)
	{
		duty_cycle = 100;
	}
	else if (duty_cycle < 0)
	{
		duty_cycle = 0;
	}
	OCR0  = (uint8)(((float)(duty_cycle)/(100)) * 255); /* Set Compare Value */

	/* Configure timer control register
	 * 1. Fast PWM mode FOC0=0
	 * 2. Fast PWM Mode WGM01=1 & WGM00=1
	 * 3. Clear OC0 when match occurs (non inverted mode) COM00=0 & COM01=1
	 * 4. clock = F_CPU/8 CS00=0 CS01=1 CS02=0
	 */
	TCCR0 = (1<<WGM00) | (1<<WGM01) | (1<<COM01) | (1<<CS01);
}

/*
 * Description :
 * Function responsible To End the pwm signal.
 */
void PWM_Timer0_Stop (void)
{
	/* Turn off the timer clock & Clear timer registers  */
	TCCR0 = 0;
	OCR0  = 0;
	TCNT0 = 0;
}


