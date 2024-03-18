/*
 * control_functions.c
 *
 *  Created on: Mar 11, 2024
 *      Author: abdalla
 *
 * Description: This file contains the implementation of control functions for the Control ECU.
 *              It includes initialization of system components, password handling, and door operation logic.
 */

#include "control_functions.h" /* Include header for control function prototypes */
#include "uart.h"              /* UART communication functions */
#include "std_types.h"         /* Standard data types */
#include "timer1.h"            /* Timer1 control functions */
#include "dc_motor.h"          /* DC motor control functions */
#include "buzzer.h"            /* Buzzer control functions */
#include "i2c.h"               /* I2C communication functions */
#include "eeprom.h"            /* EEPROM interaction functions */
#include <avr/io.h>            /* AVR IO definitions (SREG) */
#include <util/delay.h>        /* Delay functions */

/*******************************************************************************
 *                          Global Variables                                   *
 *******************************************************************************/

/* Global variables for password storage and attempt tracking */
uint8 g_firstPass[PASSWORD_SIZE] = {0};
uint8 g_secondPass[PASSWORD_SIZE] = {0};
uint8 g_attempt = 0, g_seconds = 0;

/*******************************************************************************
 *                      Functions  Definitions                                 *
 *******************************************************************************/

/* Function to initialize system components */
void Init_Function (void)
{
	/* Configuration structures for UART and TWI */
	UART_ConfigType uart_struct = {EVEN_PARITY, ONE_STOP_BIT, EIGHT_BIT, 9600};
	TWI_ConfigType twi_struct = {FAST_MODE, CONTROL_ECU_ADDRESS};

	/* Initialize UART, TWI, DC motor, buzzer, and Timer1 with callback */
	TWI_init(&twi_struct);
	UART_init(&uart_struct);
	DcMotor_init();
	BUZZER_init();
	TIMER1_setCallBack(Count_Seconds);

	/* Enable global interrupts */
	SREG |= (1<<7);
}

/* Function to send a command via UART */
void send_command(uint8 command)
{
	/* Wait for readiness and send command */
	while(UART_recieveByte() != READY_TO_RECEVIE);
	UART_sendByte(command);
}

/* Function to receive a command via UART */
uint8 receive_command(void)
{
	/* Send readiness signal and receive response */
	uint8 response;
	UART_sendByte(READY_TO_RECEVIE);
	response = UART_recieveByte();
	return response;
}

/* Function to check if a password is stored in EEPROM */
uint8 Find_Password(void)
{
	/* Read the password indicator from EEPROM */
	uint8 resulte;
	EEPROM_readByte(PASSWORD_INDICATOR, &resulte);
	return resulte;
}

/* Function to handle receiving and validating passwords */
uint8 Receiving_Passwords (void)
{
	/* Local variable for loop control */
	uint8 i;

	/* Increment attempt counter and check for max attempts */
	g_attempt++;

	if (g_attempt == MAX_ATTEMPTS)
	{
		return PASSWORDS_UNMATCH;
	}
	else
	{
		/* Receive and compare two passwords */
		UART_sendByte(READY_TO_RECEVIE);
		UART_receiveData(g_firstPass, PASSWORD_SIZE);
		UART_sendByte(READY_TO_RECEVIE);
		UART_receiveData(g_secondPass, PASSWORD_SIZE);

		/* Validate passwords and write to EEPROM if they match */
		for(i = 0; i < PASSWORD_SIZE; i++)
		{
			if (g_firstPass[i] != g_secondPass[i])
			{
				/* Write in the PASSWORD_INDICATOR location in the eeprom that is no password is saved */
				EEPROM_writeByte(PASSWORD_INDICATOR, NO_PASSWORD_FOUND);
				return PASSWORDS_UNMATCH;
			}
		}

		/* Store the password in the eeprom */
		EEPROM_writeData(PASSWORD_LOCATION, g_firstPass, PASSWORD_SIZE);

		/* Wait 10 millesec to be able to write to the eeprom again*/
		_delay_ms(10);

		/* Write in the PASSWORD_INDICATOR location in the eeprom that there is a password saved */
		EEPROM_writeByte(PASSWORD_INDICATOR, PASSWORD_FOUND);
		g_attempt = ZERO_ATTEMPTS; /* Reset attempt counter */
		return PASSWORDS_MATCH;
	}
}

/* Function to check if the entered password matches the stored password */
uint8 Checking_Password(uint8 command)
{
	/* Variable to iterate through the password array */
	uint8 i;
	/* Increment the global attempt counter */
	g_attempt++;

	/* Send a byte to indicate readiness to receive data */
	UART_sendByte(READY_TO_RECEVIE);
	/* Receive the first password attempt from the user */
	UART_receiveData(g_firstPass, PASSWORD_SIZE);

	/* Read the stored password from EEPROM */
	EEPROM_readData(PASSWORD_LOCATION, g_secondPass, PASSWORD_SIZE);

	/* Loop through each digit of the password */
	for(i = 0; i < PASSWORD_SIZE; i++)
	{
		/* If the digits match, continue to the next digit */
		if (g_firstPass[i] == g_secondPass[i])
		{
			continue;
		}
		else
		{
			/* If any digit does not match, determine the command and return the corresponding error */
			switch(command)
			{
				case OPEN_DOOR:
					/* Password does not match for 'Open Door' command */
					return PASSWORD_UNMATCH_OPEN;
					break;
				case CHANGING_PASSWORD:
					/* Password does not match for 'Change Password' command */
					return PASSWORD_UNMATCH_CHANGE;
					break;
			}
		}
	}

	/* If all digits matched, determine the command and return the corresponding success code */
	switch(command)
	{
		case OPEN_DOOR:
			/* Reset the attempt counter and return success for 'Open Door' */
			g_attempt = ZERO_ATTEMPTS;
			return PASSWORD_MATCH_OPEN;
			break;
		case CHANGING_PASSWORD:
			/* Reset the attempt counter and return success for 'Change Password' */
			g_attempt = ZERO_ATTEMPTS;
			return PASSWORD_MATCH_CHANGE;
			break;
	}
	/* Return 0 if none of the cases are met (should not happen in normal operation) */
	return 0;
}
/* Function to simulate the door opening and closing process */
void Open_Door (void)
{
	/* Rotate motor to open, hold, and close the door */
	DcMotor_rotate(MOTOR_CW, FULL_SPEED);
	Timer1_countSeconds(FIFTEEN_SECONDS);

	DcMotor_rotate(MOTOR_OFF, ZERO_SPEED);
	Timer1_countSeconds(THREE_SECONDS);

	DcMotor_rotate(MOTOR_CCW, FULL_SPEED);
	Timer1_countSeconds(FIFTEEN_SECONDS);

	DcMotor_rotate(MOTOR_OFF, ZERO_SPEED);
}

/* Function to activate the alarm for one minute */
void Alarm (void)
{
	BUZZER_on();
	Timer1_countSeconds(ONE_MINUTE);
	BUZZER_off();
}

/* Callback function to count seconds using Timer1 */
void Count_Seconds(void)
{
	g_seconds++;
}

/* Function to count a specified number of seconds using Timer1 */
void Timer1_countSeconds(uint8 seconds)
{
	/* Configure and start Timer1 */
	TIMER1_ConfigType count_sec = {COMPARE_MODE, F_CPU_256, 0, ONE_SECOND};
	TIMER1_init(&count_sec);

	/* Wait for the specified number of seconds */
	while(g_seconds != seconds);

	/* Reset seconds counter and stop Timer1 */
	g_seconds = ZERO_SECONDS;
	TIMER1_deinit();
}
