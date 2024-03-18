/*
 * hmi_functions.c
 *
 *  Created on: Mar 11, 2024
 *      Author: abdalla
 *
 * Description: This file contains the implementation of functions for the Human-Machine Interface (HMI)
 *              of an Electronic Control Unit (ECU). It includes initialization, password handling,
 *              command sending and receiving, and user interface interactions.
 */

#include "hmi_functions.h" /* HMI function prototypes */
#include "lcd.h"           /* LCD control functions */
#include "keypad.h"        /* Keypad input functions */
#include "std_types.h"     /* Standard data types */
#include "uart.h"          /* UART communication functions */
#include "timer1.h"        /* Timer1 control functions */
#include <util/delay.h>    /* For the delay functions */
#include <avr/io.h>        /* For AVR device-specific IO definitions (SREG) */

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

/* Function to initialize HMI components */
void Init_Function (void)
{
	/* UART configuration structure */
	UART_ConfigType uart = {EVEN_PARITY, ONE_STOP_BIT, EIGHT_BIT, 9600};

	/* Initialize UART, LCD, and Timer1 with callback function */
	UART_init(&uart);
	LCD_init();
	TIMER1_setCallBack(Count_Seconds);

	/* Enable global interrupts */
	SREG |= (1<<7);
}

/* Function to take the first password input from the user */
void Take_firstPassword (void)
{
	/* Local variables for keypad input and loop control */
	uint8 number, i;

	/* Clear the LCD and prompt for password input */
	LCD_clearScreen();
	LCD_displayString("plz enter pass:");
	LCD_moveCursor(1,5);

	/* Loop to read PASSWORD_SIZE digits from the keypad */
	for (i = 0; i < PASSWORD_SIZE; i++)
	{
		/* Get a single digit and validate it */
		number = KEYPAD_getPressedKey();
		while(number > 9 )
		{
			number = KEYPAD_getPressedKey();
		}
		/* Store the digit and display an asterisk */
		g_firstPass[i] = number;
		LCD_displayCharacter('*');
		_delay_ms(500);
	}
}

/* Function to take the second password input for confirmation */
void Take_secondPassword (void)
{
	/* Local variables for keypad input and loop control */
	uint8 number, i;

	/* Clear the LCD and prompt for password re-entry */
	LCD_clearScreen();
	LCD_displayString("plz re-enter the");
	LCD_displaySringRowColumn("same pass:", 1, 0);

	/* Loop to read PASSWORD_SIZE digits from the keypad */
	for (i = 0; i < PASSWORD_SIZE; i++)
	{
		/* Get a single digit and validate it */
		number = KEYPAD_getPressedKey();
		while(number > 9 )
		{
			number = KEYPAD_getPressedKey();
		}
		/* Store the digit and display an asterisk */
		g_secondPass[i] = number;
		LCD_displayCharacter('*');
		_delay_ms(500);
	}
}

/* Function to send both entered passwords to the control ECU */
void send_Password(void)
{
	/* Wait for control ECU to be ready, then send first password */
	while(UART_recieveByte() != READY_TO_RECEVIE);
	UART_sendData(g_firstPass, PASSWORD_SIZE);

	/* Wait for control ECU to be ready, then send second password */
	while(UART_recieveByte() != READY_TO_RECEVIE);
	UART_sendData(g_secondPass, PASSWORD_SIZE);
}

/* Function to receive a command from the control ECU */
uint8 receive_command(void)
{
	/* Local variable to store the received command */
	uint8 response;

	/* Send ready signal and receive a command */
	UART_sendByte(READY_TO_RECEVIE);
	response = UART_recieveByte();
	return response;
}

/* Function to send a command to the control ECU */
void send_command(uint8 command)
{
	/* Wait for control ECU to be ready, then send the command */
	while(UART_recieveByte() != READY_TO_RECEVIE);
	UART_sendByte(command);
}

/* Function to display the main menu and handle user selection */
void Main_Menu (void)
{
	/* Local variable to store the user's menu selection */
	uint8 option;

	/* Clear the LCD and display menu options */
	LCD_clearScreen();
	LCD_displayString("+ : Open Door");
	LCD_displaySringRowColumn("- : Change Pass", 1, 0);

	/* Get the user's selection and validate it */
	option = KEYPAD_getPressedKey();
	_delay_ms(500);
	while (option != '+' && option != '-')
	{
		option = KEYPAD_getPressedKey();
	}

	/* Handle the selected option */
	switch (option)
	{
		case '+':
			Checking_Password(OPEN_DOOR);
			break;
		case '-':
			Checking_Password(CHANGING_PASSWORD);
	}
}

/* Function to initiate the password change process */
void Change_Password (void)
{
	Taking_newPassword();
}

/* Function to handle new password entry and send it to the control ECU */
void Taking_newPassword (void)
{
	/* Increment the attempt counter */
	g_attempt++;

	/* If maximum attempts reached, trigger the alarm and reset */
	if (g_attempt == 5)
	{
		Alarm();
		g_attempt = ZERO_ATTEMPTS;
		Take_firstPassword();
		Take_secondPassword();
		send_command(SENDING_PASSWORDS);
		send_Password();
	}
	else
	{
		/* Otherwise, take new password entries and send them */
		Take_firstPassword();
		Take_secondPassword();
		send_command(SENDING_PASSWORDS);
		send_Password();
	}
}

/* Function to check the entered password based on the given command */
void Checking_Password(uint8 command)
{
	/* Increment the attempt counter */
	g_attempt++;

	/* If maximum attempts reached, trigger the alarm and return to main menu */
	if (g_attempt == 5)
	{
		Alarm();
		g_attempt = ZERO_ATTEMPTS;
		Main_Menu();
	}
	else
	{
		/* Otherwise, take the password and send the command for verification */
		Take_firstPassword();

		/* Send the appropriate command based on the action to be taken */
		switch(command)
		{
			case OPEN_DOOR:
				send_command(CHECKING_PASSWORD_OPEN);
				break;
			case CHANGING_PASSWORD:
				send_command(CHECKING_PASSWORD_CHANGE);
				break;
		}
		/* Wait for control ECU to be ready, then send the entered password */
		while(UART_recieveByte() != READY_TO_RECEVIE);
		UART_sendData(g_firstPass, PASSWORD_SIZE);
	}
}

/* Function to simulate the door opening process */
void Open_Door(void)
{
	/* Clear the LCD and display the door opening message */
	LCD_clearScreen();
	LCD_displayString("OPNING THE DOOR");
	Timer1_countSeconds(FIFTEEN_SECONDS);

	/* Display the door holding message */
	LCD_clearScreen();
	LCD_displayString("HOLDING THE DOOR");
	Timer1_countSeconds(THREE_SECONDS);

	/* Display the door closing message */
	LCD_clearScreen();
	LCD_displayString("CLOSING THE DOOR");
	Timer1_countSeconds(FIFTEEN_SECONDS);
}

/* Function to trigger the alarm and display an error message */
void Alarm(void)
{
	/* Clear the LCD and display the error message */
	LCD_clearScreen();
	LCD_displaySringRowColumn("ERROR", 0, 5);
	Timer1_countSeconds(ONE_MINUTE);
}

/* Callback function to count seconds using Timer1 */
void Count_Seconds(void)
{
	/* Increment the seconds counter */
	g_seconds++;
}

/* Function to count a specified number of seconds using Timer1 */
void Timer1_countSeconds(uint8 seconds)
{
	/* Timer1 configuration structure */
	TIMER1_ConfigType count_sec = {COMPARE_MODE, F_CPU_256, 0, ONE_SECOND};

	/* Initialize Timer1 with the specified configuration */
	TIMER1_init(&count_sec);

	/* Wait until the specified number of seconds has passed */
	while(g_seconds != seconds);

	/* Reset the seconds counter and deinitialize Timer1 */
	g_seconds = ZERO_SECONDS;
	TIMER1_deinit();
}
