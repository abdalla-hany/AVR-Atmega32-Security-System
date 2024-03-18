/*
 * pwm.h
 *	Description: header file for the PWM MINI driver
 *  Created on: Feb 21, 2024
 *      Author: abdalla
 */

#ifndef PWM_H_
#define PWM_H_

#include "std_types.h"

/*******************************************************************************
 *                      Functions Prototypes                                   *
 *******************************************************************************/

/*
 * Description :
 * Function responsible To start the pwm signal using timer0 compare mode
 */
void PWM_Timer0_Start(uint8 duty_cycle);

/*
 * Description :
 * Function responsible To End the pwm signal.
 */
void PWM_Timer0_Stop (void);


#endif /* SRC_PWM_H_ */
