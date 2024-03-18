/*
 * buzzer.h
 *	Description: header file for the buzzer driver
 *  Created on: Feb 27, 2024
 *      Author: abdalla
 */

#ifndef BUZZER_H_
#define BUZZER_H_

#include "std_types.h"

/*******************************************************************************
 *                                Definitions                                  *
 *******************************************************************************/

#define BUZZER_PORT		PORTA_ID
#define BUZZER_PIN	  	PIN0_ID

/*******************************************************************************
 *                      Functions Prototypes                                   *
 *******************************************************************************/

/*
 * Description :
 * Function to initialize the buzzer
 */
void BUZZER_init(void);

/*
 * Description :
 * Function to turn on the buzzer.
 */
void BUZZER_on(void);

/*
 * Description :
 * Function to turn off the buzzer.
 */
void BUZZER_off(void);

#endif /* SRC_BUZZER_H_ */
