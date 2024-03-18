/*
 * lcd.c
 *	Description: Source file for the LCD driver
 *  Created on: Feb 10, 2024
 *      Author: abdalla
 */

#include "lcd.h"

#include <util/delay.h>    	/* For the delay functions */

#include "common_macros.h"	/* For GET_BIT Macro */
#include "gpio.h"

/*******************************************************************************
 *                      Functions Definitions                                  *
 *******************************************************************************/

/*
 * Function to send a command to the LCD
 */
void LCD_SendCommand(uint8 command)
{
    GPIO_writePin(LCD_RS_PORT_ID, LCD_RS_PIN_ID, LOGIC_LOW);
    _delay_us(2);
    GPIO_writePin(LCD_ENABLE_PORT_ID, LCD_ENABLE_PIN_ID, LOGIC_HIGH);
    _delay_us(2);

    /*
     * Check the LCD bit mode and send the command accordingly
     */
    #if (LCD_BIT_MODE == 8)
        GPIO_writePort(LCD_DATA_PORT_ID, command);
        _delay_us(2);
    #elif (LCD_BIT_MODE == 4)

        /* Send high bit command */
        GPIO_writePin(LCD_DATA_PORT_ID, LCD_DATA_FIRST_PIN_ID, GET_BIT(command, 4));
        GPIO_writePin(LCD_DATA_PORT_ID, LCD_DATA_FIRST_PIN_ID + 1, GET_BIT(command, 5));
        GPIO_writePin(LCD_DATA_PORT_ID, LCD_DATA_FIRST_PIN_ID + 2, GET_BIT(command, 6));
        GPIO_writePin(LCD_DATA_PORT_ID, LCD_DATA_FIRST_PIN_ID + 3, GET_BIT(command, 7));

        /* delay */
        _delay_us(2);

        /* Disable */
        GPIO_writePin(LCD_ENABLE_PORT_ID, LCD_ENABLE_PIN_ID, LOGIC_LOW);
        _delay_us(2);

        /* enable */
        GPIO_writePin(LCD_ENABLE_PORT_ID, LCD_ENABLE_PIN_ID, LOGIC_HIGH);
        _delay_us(2);

        /* send low bit */
        GPIO_writePin(LCD_DATA_PORT_ID, LCD_DATA_FIRST_PIN_ID, GET_BIT(command, 0));
        GPIO_writePin(LCD_DATA_PORT_ID, LCD_DATA_FIRST_PIN_ID + 1, GET_BIT(command, 1));
        GPIO_writePin(LCD_DATA_PORT_ID, LCD_DATA_FIRST_PIN_ID + 2, GET_BIT(command, 2));
        GPIO_writePin(LCD_DATA_PORT_ID, LCD_DATA_FIRST_PIN_ID + 3, GET_BIT(command, 3));

        /* delay */
        _delay_us(2);
    #endif

    GPIO_writePin(LCD_ENABLE_PORT_ID, LCD_ENABLE_PIN_ID, LOGIC_LOW);
    _delay_us(2);
}

/*
 * Function to display a character on the LCD
 */
void LCD_displayCharacter(uint8 data)
{
    GPIO_writePin(LCD_RS_PORT_ID, LCD_RS_PIN_ID, LOGIC_HIGH);
    _delay_us(2);
    GPIO_writePin(LCD_ENABLE_PORT_ID, LCD_ENABLE_PIN_ID, LOGIC_HIGH);
    _delay_us(2);

    /*
     * Check the LCD bit mode and send the data accordingly
     */
    #if (LCD_BIT_MODE == 8)
        GPIO_writePort(LCD_DATA_PORT_ID, data);
        _delay_us(2);
    #elif (LCD_BIT_MODE == 4)

        /* Send high command */
        GPIO_writePin(LCD_DATA_PORT_ID, LCD_DATA_FIRST_PIN_ID, GET_BIT(data, 4));
        GPIO_writePin(LCD_DATA_PORT_ID, LCD_DATA_FIRST_PIN_ID + 1, GET_BIT(data, 5));
        GPIO_writePin(LCD_DATA_PORT_ID, LCD_DATA_FIRST_PIN_ID + 2, GET_BIT(data, 6));
        GPIO_writePin(LCD_DATA_PORT_ID, LCD_DATA_FIRST_PIN_ID + 3, GET_BIT(data, 7));

        /* delay */
        _delay_us(2);

        /* Disable */
        GPIO_writePin(LCD_ENABLE_PORT_ID, LCD_ENABLE_PIN_ID, LOGIC_LOW);
        _delay_us(2);

        /* enable */
        GPIO_writePin(LCD_ENABLE_PORT_ID, LCD_ENABLE_PIN_ID, LOGIC_HIGH);
        _delay_us(2);

        /* send low command */
        GPIO_writePin(LCD_DATA_PORT_ID, LCD_DATA_FIRST_PIN_ID, GET_BIT(data, 0));
        GPIO_writePin(LCD_DATA_PORT_ID, LCD_DATA_FIRST_PIN_ID + 1, GET_BIT(data, 1));
        GPIO_writePin(LCD_DATA_PORT_ID, LCD_DATA_FIRST_PIN_ID + 2, GET_BIT(data, 2));
        GPIO_writePin(LCD_DATA_PORT_ID, LCD_DATA_FIRST_PIN_ID + 3, GET_BIT(data, 3));

        /* delay */
        _delay_us(2);
    #endif

    GPIO_writePin(LCD_ENABLE_PORT_ID, LCD_ENABLE_PIN_ID, LOGIC_LOW);
    _delay_us(2);
}

/*
 * Function to initialize the LCD
 */
void LCD_init(void)
{
	/* Configure the direction for RS and E pins as output pins */
    GPIO_setupPinDirection(LCD_RS_PORT_ID, LCD_RS_PIN_ID, PIN_OUTPUT);
    GPIO_setupPinDirection(LCD_ENABLE_PORT_ID, LCD_ENABLE_PIN_ID, PIN_OUTPUT);
    _delay_ms(20); /* LCD Power ON delay */

    #if (LCD_BIT_MODE == 8)

    	/* Configure the data port as output port */
        GPIO_setupPortDirection(LCD_DATA_PORT_ID, PORT_OUTPUT);

        /* use 2-lines LCD + 8-bits Data Mode + 5*7 dot display Mode */
        LCD_SendCommand(LCD_8BIT);
    #elif (LCD_BIT_MODE == 4)

        /* Configure 4 pins in the data port as output pins */
        GPIO_setupPinDirection(LCD_DATA_PORT_ID, LCD_DATA_FIRST_PIN_ID, PIN_OUTPUT);
        GPIO_setupPinDirection(LCD_DATA_PORT_ID, LCD_DATA_FIRST_PIN_ID + 1, PIN_OUTPUT);
        GPIO_setupPinDirection(LCD_DATA_PORT_ID, LCD_DATA_FIRST_PIN_ID + 2, PIN_OUTPUT);
        GPIO_setupPinDirection(LCD_DATA_PORT_ID, LCD_DATA_FIRST_PIN_ID + 3, PIN_OUTPUT);

        LCD_SendCommand(LCD_4BIT);	/* use 2-lines LCD + 4-bits Data Mode + 5*7 dot display Mode */
        LCD_SendCommand(LCD_4BIT1);	/* Send for 4 bit initialization of LCD  */
        LCD_SendCommand(LCD_4BIT2);
    #endif

    LCD_SendCommand(LCD_CURSOR_OFF);		/* cursor off */
    LCD_SendCommand(LCD_CLEAR_COMMAND);		/* clear LCD at the beginning */
}

/*
 * Function to display a string on the LCD
 */
void LCD_displayString(const char* string)
{
    uint8 i = 0;
    while (string[i] != '\0')
    {
        LCD_displayCharacter(string[i]);
        i++;
    }
}

/*
 * Function to move the cursor to a specific row and column on the LCD
 */
void LCD_moveCursor(uint8 row, uint8 col)
{
    uint8 address;
    switch (row)
    {
    case 0:
        address = col;
        break;
    case 1:
        address = col + 0x40;
        break;
    case 2:
        address = col + 0x10;
        break;
    case 3:
        address = col + 0x50;
        break;
    }
    LCD_SendCommand(address | LCD_MOVE_CURSOR);
    _delay_us(2);
}

/*
 * Function to display a string at a specific row and column on the LCD
 */
void LCD_displaySringRowColumn(const char* string, uint8 row, uint8 col)
{
    LCD_moveCursor(row, col);
    LCD_displayString(string);
}

/*
 * Function to clear the LCD screen
 */
void LCD_clearScreen(void)
{
    LCD_SendCommand(LCD_CLEAR_COMMAND);
}

/*
 * Function to convert an integer to a string and display it on the LCD
 */
void LCD_intgerToString(int data)
{
   char buff[16];
   itoa(data, buff, 10);
   LCD_displayString(buff);
}
