/*
 * Control_ECU.c
 *
 *  Created on: Mar 9, 2024
 *      Author: abdalla
 *
 * Description: Main file for the Control Electronic Control Unit (ECU). It initializes the system,
 *              checks for existing passwords, and continuously processes incoming commands related
 *              to password management and door operations.
 */

#include "control_functions.h" /* Include the header file for control-related functions */

int main(void)
{
	/* Local Variable declarations */
	uint8 response, resulte, password_indicator;

	/* Initialize system functions */
	Init_Function();

	/* Check if a password is already set in the system */
	password_indicator = Find_Password();

	/* Inform the HMI whether a password is found or Not */
	if (password_indicator == PASSWORD_FOUND)
	{
		send_command(PASSWORD_FOUND);
	}
	else
	{
		send_command(NO_PASSWORD_FOUND);
	}

	/* Main loop to handle commands from HMI */
	while (1)
	{
		/* Receive command from HMI */
		response = receive_command();

		/* Process the received command and get the result */
		switch (response)
		{
			/* Handle new password setup */
			case SENDING_PASSWORDS:
				resulte = Receiving_Passwords();
				break;

			/* Handle password check for opening the door */
			case CHECKING_PASSWORD_OPEN:
				resulte = Checking_Password(OPEN_DOOR);
				break;

			/* Handle password change request */
			case CHECKING_PASSWORD_CHANGE:
				resulte = Checking_Password(CHANGING_PASSWORD);
				break;
		}

		/* Send the result back to the HMI */
		send_command(resulte);

		/* Take action based on the result */
		if (resulte == PASSWORD_MATCH_OPEN)
		{
			Open_Door(); /* Open the door if the password matches */
		}

		/* Activate the alarm after 4 unsuccessful attempts */
		if ((resulte == PASSWORD_UNMATCH_OPEN && g_attempt == MAX_ATTEMPTS) || (resulte == PASSWORD_UNMATCH_CHANGE && g_attempt == MAX_ATTEMPTS))
		{
			g_attempt = ZERO_ATTEMPTS; 	/* Reset attempt counter */
			Alarm();
		}
	}
}
