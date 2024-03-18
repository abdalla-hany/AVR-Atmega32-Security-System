/*
 * keypad.h
 *
 *  Created on: Feb 12, 2024
 *      Author: abdalla
 */

#ifndef KEYPAD_H_
#define KEYPAD_H_

#include "std_types.h"
/*******************************************************************************
 *                                Definitions                                  *
 *******************************************************************************/
#undef STANDARD_KEYPAD

/* Keypad configurations for number of rows and columns */
#define KEYPAD_NUM_COLS         4
#define KEYPAD_NUM_ROWS         4

/* Keypad Port Configurations */
#define KEYPAD_ROW_PORT_ID      PORTA_ID
#define KEYPAD_COLUMN_PORT_ID   PORTA_ID

#define KEYPAD_ROW_PIN_ID		PIN0_ID
#define KEYPAD_COLUMN_PIN_ID	PIN4_ID

/* Keypad button logic configurations */
#define KEYPAD_BUTTON_PRESSED	LOGIC_LOW
#define KEYPAD_BUTTON_RELEASED	LOGIC_HIGH

/*******************************************************************************
 *                      Functions Prototypes                                   *
 *******************************************************************************/
/*
 * Description :
 * Get the Keypad pressed button
 */
uint8 KEYPAD_getPressedKey(void);
#endif /* KEYPAD_H_ */
