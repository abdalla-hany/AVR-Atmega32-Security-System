/*
 * control_functions.h
 *
 *  Created on: Mar 11, 2024
 *      Author: abdalla
 *
 * Description: Header file for the control functions used in the Control ECU. It includes function prototypes,
 *              macro definitions for password operations, and timer settings.
 */

#ifndef CONTROL_FUNCTIONS_H_
#define CONTROL_FUNCTIONS_H_

#include "std_types.h" /* Include standard data types */


/*******************************************************************************
 *                                Definitions                                  *
 *******************************************************************************/

/* Address for the Control ECU in the TWI network */
#define CONTROL_ECU_ADDRESS 0x10

/* Macro for the size of the password array */
#define PASSWORD_SIZE 5

/* Macros for various password and command states */
#define NO_PASSWORD_FOUND           0x10 /* No password is set in the system */
#define PASSWORD_FOUND              0x11 /* A password is found in the system */
#define PASSWORDS_MATCH             0x12 /* Entered passwords match */
#define PASSWORDS_UNMATCH           0x13 /* Entered passwords do not match */
#define PASSWORD_MATCH_OPEN         0x14 /* Password match for 'Open Door' command */
#define PASSWORD_MATCH_CHANGE       0x15 /* Password match for 'Change Password' command */
#define PASSWORD_UNMATCH            0x16 /* Password does not match */
#define OPEN_DOOR                   0x17 /* Command to open the door */
#define SENDING_PASSWORDS           0x19 /* Command to send passwords to the control unit */
#define CHECKING_PASSWORD_OPEN      0x21 /* Command to check password for opening the door */
#define CHECKING_PASSWORD_CHANGE    0x22 /* Command to check password for changing password */
#define CHANGING_PASSWORD           0x23 /* Command to change the password */
#define PASSWORD_UNMATCH_OPEN       0x24 /* Password does not match for 'Open Door' command */
#define PASSWORD_UNMATCH_CHANGE     0x25 /* Password does not match for 'Change Password' command */

/* Macros for attempt limits */
#define MAX_ATTEMPTS        	4 	/* Maximum number of password attempts */
#define ZERO_ATTEMPTS       	0 	/* Reset the number of attempts to zero */

/*
 * Timer tick calculation for one second
 * 31250 timer tick
 * each tick = 32 micro sec
 * 32 * 31250 = 1000000 micro sec
 * 1000000 micro sec = 1 sec
 */
#define ONE_SECOND          31250 /* Number of timer ticks for one second */

/* Macros for time durations in seconds */
#define ZERO_SECONDS        0  		/* Zero seconds */
#define TWO_SECONDS         2  		/* Two seconds */
#define THREE_SECONDS       3  		/* Three seconds */
#define FIFTEEN_SECONDS     15 		/* Fifteen seconds */
#define ONE_MINUTE          60 		/* Sixty seconds or one minute */

/* Speed definitions for the DC motor */
#define FULL_SPEED          100 	/* Full speed setting for the motor */
#define ZERO_SPEED          0   	/* Zero speed setting, effectively turning off the motor */

/* EEPROM locations for storing password and its status indicator */
#define PASSWORD_INDICATOR  0x0309 	/* EEPROM location for the password existence indicator */
#define PASSWORD_LOCATION   0x0311 	/* EEPROM starting location for the stored password */

/* Global variable for tracking the number of password attempts */
extern uint8 g_attempt;

/*******************************************************************************
 *                      Functions Prototypes                                   *
 *******************************************************************************/

/* Initialize control ECU components */
void Init_Function (void);

/* Send a command to the HMI ECU */
void send_command(uint8 command);

/* Receive a command from the HMI ECU */
uint8 receive_command(void);

/* Check if a password is stored in EEPROM */
uint8 Find_Password(void);

/* Handle receiving and validating new passwords */
uint8 Receiving_Passwords (void);

/* Check the entered password against the stored one */
uint8 Checking_Password(uint8 command);

/* Operate the door mechanism */
void Open_Door (void);

/* Activate the alarm system */
void Alarm (void);

/* Timer callback function to count seconds */
void Count_Seconds(void);

/* Count a specified number of seconds using Timer1 */
void Timer1_countSeconds(uint8 seconds);


#endif /* SRC_CONTROL_FUNCTIONS_H_ */
