/*
 * lcd.h
 *	Description: Header file for the LCD driver
 *  Created on: Feb 10, 2024
 *      Author: abdalla
 */

#ifndef LCD_H_
#define LCD_H_

#include "std_types.h"

/*******************************************************************************
 *                                Definitions                                  *
 *******************************************************************************/
/* LCD Data bits mode configuration, its value should be 4 or 8*/
#define LCD_BIT_MODE 				8

#if((LCD_BIT_MODE != 4) && (LCD_BIT_MODE != 8))

#error "Number of Data bits should be equal to 4 or 8"

#endif

/* LCD HW Ports and Pins Ids */
#define LCD_RS_PORT_ID				PORTB_ID
#define LCD_RS_PIN_ID				PIN0_ID

#define LCD_ENABLE_PORT_ID			PORTB_ID
#define LCD_ENABLE_PIN_ID			PIN1_ID

#define LCD_DATA_PORT_ID			PORTC_ID
#define LCD_DATA_FIRST_PIN_ID		PIN0_ID

/* LCD Commands */
#define LCD_CLEAR_COMMAND			0x01
#define LCD_8BIT			   		0x38
#define LCD_4BIT			   		0x28
#define LCD_4BIT1			   		0x33
#define LCD_4BIT2			   		0x32
#define LCD_ENTRY_MODE			   	0x06
#define LCD_MOVE_CURSOR			   	0x80
#define LCD_CURSOR_ON	   			0x0E
#define LCD_CURSOR_OFF	   			0x0C
#define LCD_CURSOR_BLINK  			0x0F
#define LCD_DISPLAYOFF_CURSOROFF   	0x08

/*******************************************************************************
 *                      Functions Prototypes                                   *
 *******************************************************************************/

/*
 * Description :
 * Send the required command to the screen
 */
void LCD_SendCommand(uint8 command);

/*
 * Description :
 * Display the required character on the screen
 */
void LCD_displayCharacter(uint8 data);
/*
 * Description :
 * Initialize the LCD:
 * 1. Setup the LCD pins directions by use the GPIO driver.
 * 2. Setup the LCD Data Mode 4-bits or 8-bits.
 */
void LCD_init(void);
/*
 * Description :
 * Display the required string on the screen
 */
void LCD_displayString(const char* string);
/*
 * Description :
 * Move the cursor to a specified row and column index on the screen
 */
void LCD_moveCursor(uint8 row, uint8 col);
/*
 * Description :
 * Display the required string in a specified row and column index on the screen
 */
void LCD_displaySringRowColumn(const char* string, uint8 row, uint8 col);
/*
 * Description :
 * Display the required decimal value on the screen
 */
void LCD_intgerToString(int data);
/*
 * Description :
 * Send the clear screen command
 */
void LCD_clearScreen(void);

#endif /* SRC_LCD_H_ */
