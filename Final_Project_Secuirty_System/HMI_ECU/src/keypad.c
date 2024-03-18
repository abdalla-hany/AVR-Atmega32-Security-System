/*
 * keypad.c
 *
 *  Created on: Feb 12, 2024
 *      Author: abdalla
 */

#include "gpio.h"
#include "keypad.h"
#include <util/delay.h>

/*******************************************************************************
 *                      Functions Prototypes(For keypad file only)            *
 ******************************************************************************/

#ifndef STANDARD_KEYPAD

#if (KEYPAD_NUM_COLS == 3)
/*
 * Function: KEYPAD_4x3_adjustKeyNumber
 * ----------------------------
 *   Maps the switch number in the keypad to its corresponding functional number in the Proteus for 4x3 keypad
 *   button_number: the switch number in the keypad
 *   returns: the adjusted key number
 */
static uint8 KEYPAD_4x3_adjustKeyNumber(uint8 button_number);
#elif (KEYPAD_NUM_COLS == 4)
/*
 * Function: KEYPAD_4x4_adjustKeyNumber
 * ----------------------------
 *   Maps the switch number in the keypad to its corresponding functional number in the Proteus for 4x4 keypad
 *   button_number: the switch number in the keypad
 *   returns: the adjusted key number
 */
static uint8 KEYPAD_4x4_adjustKeyNumber(uint8 button_number);
#endif

#endif /* STANDARD_KEYPAD */

/*******************************************************************************
 *                      Functions Definitions                                  *
 ******************************************************************************/

/*
 * Function: KEYPAD_getPressedKey
 * ----------------------------
 *   Retrieves the pressed key on the keypad
 *   returns: the pressed key
 */
uint8 KEYPAD_getPressedKey(void)
{
	uint8 row, column;

	/* Set up pin directions for rows and columns */
	for (row = 0; row < KEYPAD_NUM_ROWS; row++)
		GPIO_setupPinDirection(KEYPAD_ROW_PORT_ID, KEYPAD_ROW_PIN_ID + row, PIN_INPUT);

	for (column = 0; column < KEYPAD_NUM_COLS; column++)
		GPIO_setupPinDirection(KEYPAD_COLUMN_PORT_ID, KEYPAD_COLUMN_PIN_ID + column, PIN_INPUT);

	while (1)
	{
		/* loop for rows */
		for (row = 0; row < KEYPAD_NUM_ROWS; row++)
		{
			/* Set the current row pin as output */
			GPIO_setupPinDirection(KEYPAD_ROW_PORT_ID, KEYPAD_ROW_PIN_ID + row, PIN_OUTPUT);
			/* Set/Clear the row output pin */
			GPIO_writePin(KEYPAD_ROW_PORT_ID, KEYPAD_ROW_PIN_ID + row, KEYPAD_BUTTON_PRESSED);

			/* loop for columns */
			for (column = 0; column < KEYPAD_NUM_COLS; column++)
			{
				/* Check if the switch is pressed in this column */
				if (GPIO_readPin(KEYPAD_COLUMN_PORT_ID, KEYPAD_COLUMN_PIN_ID + column) == KEYPAD_BUTTON_PRESSED)
				{
					#if (KEYPAD_NUM_COLS == 3)
						#ifdef STANDARD_KEYPAD
							return ((row * KEYPAD_NUM_COLS) + column + 1 );
						#else
							return KEYPAD_4x3_adjustKeyNumber((row *KEYPAD_NUM_COLS) + column + 1);
						#endif
					#elif (KEYPAD_NUM_COLS == 4)
						#ifdef STANDARD_KEYPAD
							return ((row * KEYPAD_NUM_COLS) + column +1);
						#else
							return KEYPAD_4x4_adjustKeyNumber((row *KEYPAD_NUM_COLS) + column + 1);
						#endif
					#endif
				}
			}
			/* Restore row pins as inputs */
			GPIO_setupPinDirection(KEYPAD_ROW_PORT_ID, KEYPAD_ROW_PIN_ID + row, PIN_INPUT);
			_delay_ms(5);
		}
	}
}

#ifndef STANDARD_KEYPAD

#if (KEYPAD_NUM_COLS == 3)
/*
 * Function: KEYPAD_4x3_adjustKeyNumber
 * ----------------------------
 *   Adjusts the keypad pressed button value for a 4x3 keypad
 *   button_number: the button number pressed on the keypad
 *   returns: the adjusted key value
 */
static uint8 KEYPAD_4x3_adjustKeyNumber (uint8 button_number)
{
	uint8 key = 0;
	switch(button_number)
	{
		case 10 : key = '*';
			break;
		case 11 : key = 0;
			break;
		case 12 : key = '#';
			break;
		default: key = button_number;
	}
	return key;
}

#elif (KEYPAD_NUM_COLS == 4)
/*
 * Function: KEYPAD_4x4_adjustKeyNumber
 * ----------------------------
 *   Adjusts the keypad pressed button value for a 4x4 keypad
 *   button_number: the button number pressed on the keypad
 *   returns: the adjusted key value
 */
static uint8 KEYPAD_4x4_adjustKeyNumber (uint8 button_number)
{
	uint8 key = 0;
	switch(button_number)
	{
		case 1 : key = 7;
			break;
		case 2 : key = 8;
			break;
		case 3 : key = 9;
			break;
		case 4 : key = '/';
			break;
		case 5 : key = 4;
			break;
		case 6 : key = 5;
			break;
		case 7 : key = 6;
			break;
		case 8 : key = '*';
			break;
		case 9 : key = 1;
			break;
		case 10: key = 2;
			break;
		case 11: key = 3;
			break;
		case 12: key = '-';
			break;
		case 13: key = 13;
			break;
		case 14: key = 0;
			break;
		case 15: key = '=';
			break;
		case 16: key = '+';
			break;
		default: key = button_number;
	}
	return key;
}
#endif

#endif /* STANDARD_KEYPAD */
