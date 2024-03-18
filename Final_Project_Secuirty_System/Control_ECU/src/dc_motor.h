/*
 * dc_motor.h
 *	Description: header file for the DC-motor driver
 *  Created on: Feb 21, 2024
 *      Author: abdalla
 */

#ifndef DC_MOTOR_H_
#define DC_MOTOR_H_

#include "std_types.h" /* Include standard types header file */

/*******************************************************************************
 *                                Definitions                                  *
 *******************************************************************************/

#define MOTOR_PORT PORTB_ID /* Define the port for the DC motor */
#define MOTOR_PIN  PIN0_ID  /* Define the pin for the DC motor */

/*******************************************************************************
 *                               Types Declaration                             *
 *******************************************************************************/
/* Enumeration for the states of the DC motor */
typedef enum
{
    MOTOR_OFF,  /* Motor is turned off */
    MOTOR_CW,   /* Motor rotates clockwise */
    MOTOR_CCW   /* Motor rotates counter-clockwise */
} DcMotor_State;

/*******************************************************************************
 *                      Functions Prototypes                                   *
 *******************************************************************************/
/*
 * Description :
 * Function responsible for initialize the DC-motor driver.
 */
void DcMotor_init(void);

/*
 * Description :
 * Function responsible for Rotating the DC motor.
 */
void DcMotor_rotate(DcMotor_State state, uint8 speed); /* Rotate the DC motor */

#endif /* SRC_DC_MOTOR_H_ */
