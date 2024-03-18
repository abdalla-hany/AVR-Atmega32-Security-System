/*
 * buzzer.c
 *	Description: source file for the buzzer driver
 *  Created on: Feb 27, 2024
 *      Author: abdalla
 */

/* Include header files */
#include "buzzer.h" /* Include header file for buzzer functions */
#include "gpio.h" 	/* Include header file for GPIO functions */
#include "pwm.h" 	/* Include header file for PWM functions */

/*******************************************************************************
 *                      Functions Definitions                                  *
 *******************************************************************************/

/*
 * Description :
 * Function to initialize the buzzer
 */
void BUZZER_init(void)
{
	/*  Setup the direction for the buzzer pin */
    GPIO_setupPinDirection(BUZZER_PORT, BUZZER_PIN, PIN_OUTPUT); 	/* Set buzzer pin as output */

    /* Turn off the buzzer at the beginning */
    GPIO_writePin(BUZZER_PORT, BUZZER_PIN, LOGIC_LOW); 				/* Set initial state of buzzer pin to low */
}

/*
 * Description :
 * Function to turn on the buzzer.
 */
void BUZZER_on (void)
{
	/* Turn on the buzzer*/
	GPIO_writePin(BUZZER_PORT, BUZZER_PIN, LOGIC_HIGH);
}

/*
 * Description :
 * Function to turn off the buzzer.
 */
void BUZZER_off(void)
{
	/* Turn off the buzzer*/
	GPIO_writePin(BUZZER_PORT, BUZZER_PIN, LOGIC_LOW);
}


