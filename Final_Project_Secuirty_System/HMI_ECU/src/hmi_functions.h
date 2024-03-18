/*
 * hmi_functions.h
 *
 *  Created on: Mar 11, 2024
 *      Author: abdalla
 *
 * Description: Header file for the HMI functions used in the ECU. It includes function prototypes,
 *              macro definitions for password operations, and timer settings.
 */

#ifndef HMI_FUNCTIONS_H_
#define HMI_FUNCTIONS_H_

#include "std_types.h" /* Include standard data types */


/*******************************************************************************
 *                                Definitions                                  *
 *******************************************************************************/

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
#define MAX_ATTEMPTS        		4 	/* Maximum number of password attempts */
#define ZERO_ATTEMPTS       		0 	/* Reset the number of attempts to zero */

/*
 * Timer tick calculation for one second
 * 31250 timer tick
 * each tick = 32 micro sec
 * 32 * 31250 = 1000000 micro sec
 * 1000000 micro sec = 1 sec
 */
#define ONE_SECOND          31250 /* Number of timer ticks for one second */

/* Macros for time durations in seconds */
#define ZERO_SECONDS        0  /* Zero seconds */
#define TWO_SECONDS         2  /* Two seconds */
#define THREE_SECONDS       3  /* Three seconds */
#define FIFTEEN_SECONDS     15 /* Fifteen seconds */
#define ONE_MINUTE          60 /* Sixty seconds or one minute */

/* Global variable for tracking password attempts */
extern uint8 g_attempt;

/*******************************************************************************
 *                      Functions Prototypes                                   *
 *******************************************************************************/

/* Initialize HMI components */
void Init_Function (void);

 /* Take the first password input */
void Take_firstPassword (void);

/* Take the second password input for confirmation */
void Take_secondPassword (void);

/* Send the password to the control unit */
void send_Password(void);

/* Receive a command from the control unit */
uint8 receive_command(void);

/* Send a command to the control unit */
void send_command(uint8 command);

/* Display the main menu and handle user selection */
void Main_Menu (void);

/* Initiate the password change process */
void Change_Password (void);

/* Handle new password entry and sending */
void Taking_newPassword (void);

/* Check the entered password based on the command */
void Checking_Password(uint8 command);

/* Simulate the door opening process */
void Open_Door(void);

/* Trigger the alarm and display an error message */
void Alarm(void);

/* Callback function to count seconds using Timer1 */
void Count_Seconds(void);

/* Count a specified number of seconds using Timer1 */
void Timer1_countSeconds(uint8 seconds);

#endif /* SRC_HMI_FUNCTIONS_H_ */

