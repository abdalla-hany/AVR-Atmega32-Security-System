/*
 * HMI_ECU.c
 *
 *  Created on: Mar 9, 2024
 *      Author: abdalla
 *
 * Description: This file contains the main function for the Human-Machine Interface (HMI)
 *              of an Electronic Control Unit (ECU). It handles user inputs from keypad
 *              and displays corresponding outputs on an LCD.
 *
 */

#include "hmi_functions.h" 	/* Include header for HMI-related functions */
#include "lcd.h"          	/* Include header for LCD-related functions */

int main(void)
{
	uint8 g_status; 	/* Declare a variable to store the status of received commands */
	Init_Function(); 	/* Initialize HMI-related functions and LCD */

	/* Infinite loop to continuously check for and execute commands */
	while(1)
	{
		g_status = receive_command(); 	/* Receive command and update status */
		switch (g_status) 				/* Switch case based on the received command status */
		{
			case NO_PASSWORD_FOUND:
				Taking_newPassword(); 	/* Prompt user to set a new password if none is found */
				break;
			case PASSWORD_FOUND:
				Main_Menu(); 			/* Display main menu if password is found */
				break;
			case PASSWORDS_MATCH:
				LCD_clearScreen(); 									/* Clear the LCD screen */
				LCD_displaySringRowColumn("PASSWORD SAVED", 0, 1); 	/* Display success message */
				LCD_displaySringRowColumn("SUCCESSFULLY", 1, 2); 	/* Continue success message */
				Timer1_countSeconds(TWO_SECONDS); 					/* Wait for two seconds */
				g_attempt = ZERO_ATTEMPTS; 							/* Reset password attempt counter */
				Main_Menu(); 										/* Return to main menu */
				break;
			case PASSWORDS_UNMATCH:
				LCD_clearScreen(); 				/* Clear the LCD screen */
				if (g_attempt == MAX_ATTEMPTS) 	/* Check if maximum attempts reached */
				{
					Taking_newPassword(); 		/* Prompt user to set a new password */
				}
				else
				{
					LCD_displaySringRowColumn("PASSWORD UNMATCH", 0, 0); /* Display error message */
					LCD_displaySringRowColumn("TRY AGAIN", 1, 3); 		 /* Prompt to try again */
					Timer1_countSeconds(TWO_SECONDS); 					 /* Wait for two seconds */
					Taking_newPassword(); 								 /* Prompt user to set a new password */
				}

				break;
			case PASSWORD_MATCH_OPEN:
				LCD_clearScreen(); 			/* Clear the LCD screen */
				g_attempt = ZERO_ATTEMPTS; 	/* Reset password attempt counter */
				Open_Door(); 				/* Open the door if password matches */
				Main_Menu(); 				/* Return to main menu */
				break;
			case PASSWORD_MATCH_CHANGE:
				LCD_clearScreen();							 			/* Clear the LCD screen */
				LCD_displaySringRowColumn("PASSWORD IS", 0, 2); 		/* Display confirmation message */
				LCD_displaySringRowColumn("CORRECT WELCOME", 0, 1); 	/* Continue confirmation message */
				Timer1_countSeconds(TWO_SECONDS); 						/* Wait for two seconds */
				g_attempt = ZERO_ATTEMPTS; 								/* Reset password attempt counter */
				Change_Password(); 										/* Proceed to change password */
				break;
			case PASSWORD_UNMATCH_OPEN:
				if (g_attempt == MAX_ATTEMPTS)	 	/* Check if maximum attempts reached */
				{
					LCD_clearScreen(); 				/* Clear the LCD screen */
					Checking_Password(OPEN_DOOR); 	/* Check password for opening door */
				}
				else
				{
					LCD_clearScreen(); 										/* Clear the LCD screen */
					LCD_displaySringRowColumn("PASSWORD ISN'T", 0, 1); 		/* Display error message */
					LCD_displaySringRowColumn("CORRECT TRY AGAIN", 1, 0); 	/* Prompt to try again */
					Timer1_countSeconds(TWO_SECONDS); 						/* Wait for two seconds */
					Checking_Password(OPEN_DOOR); 							/* Check password for opening door */
				}
				break;
			case PASSWORD_UNMATCH_CHANGE:
				if (g_attempt == MAX_ATTEMPTS) 				/* Check if maximum attempts reached */
				{
					LCD_clearScreen(); 						/* Clear the LCD screen */
					Checking_Password(CHANGING_PASSWORD); 	/* Check password for changing password */
				}
				else
				{
					LCD_clearScreen(); 										/* Clear the LCD screen */
					LCD_displaySringRowColumn("PASSWORD ISN'T", 0, 1); 		/* Display error message */
					LCD_displaySringRowColumn("CORRECT TRY AGAIN", 1, 0); 	/* Prompt to try again */
					Timer1_countSeconds(TWO_SECONDS); 						/* Wait for two seconds */
					Checking_Password(CHANGING_PASSWORD); 					/* Check password for changing password */
				}
				break;
		}
	}
}
