/*
 * dc_motor.c
 *	Description: source file for the DC-motor driver
 *  Created on: Feb 21, 2024
 *      Author: abdalla
 */

#include "dc_motor.h"
#include "gpio.h"
#include "pwm.h"

/*******************************************************************************
 *                      Functions Definitions                                  *
 *******************************************************************************/

/* Function to initialize the DC motor */
void DcMotor_init(void)
{
    /* Setup motor pins as output */
    GPIO_setupPinDirection(MOTOR_PORT, MOTOR_PIN, PIN_OUTPUT);
    GPIO_setupPinDirection(MOTOR_PORT, MOTOR_PIN + 1, PIN_OUTPUT);

    /* Set initial state of motor pins to LOW motor off*/
    GPIO_writePin(MOTOR_PORT, MOTOR_PIN, LOGIC_LOW);
    GPIO_writePin(MOTOR_PORT, MOTOR_PIN + 1, LOGIC_LOW);
}

/* Function to rotate the DC motor based on the specified state and speed */
void DcMotor_rotate(DcMotor_State state, uint8 speed)
{
    /* Switch based on the motor state */
    switch (state)
    {
		case MOTOR_OFF:
			/* Turn off the pwm signal */
			PWM_Timer0_Stop ();

			/* Turn off the motor */
			GPIO_writePin(MOTOR_PORT, MOTOR_PIN, LOGIC_LOW);
			GPIO_writePin(MOTOR_PORT, MOTOR_PIN + 1, LOGIC_LOW);
			break;
		case MOTOR_CW:
		    /* Start PWM signal generation with specified speed */
		    PWM_Timer0_Start(speed);

			/* Rotate the motor clockwise */
			GPIO_writePin(MOTOR_PORT, MOTOR_PIN, LOGIC_LOW);
			GPIO_writePin(MOTOR_PORT, MOTOR_PIN + 1, LOGIC_HIGH);
			break;
		case MOTOR_CCW:
		    /* Start PWM signal generation with specified speed */
		    PWM_Timer0_Start(speed);

			/* Rotate the motor counter-clockwise */
			GPIO_writePin(MOTOR_PORT, MOTOR_PIN, LOGIC_HIGH);
			GPIO_writePin(MOTOR_PORT, MOTOR_PIN + 1, LOGIC_LOW);
    }
}
