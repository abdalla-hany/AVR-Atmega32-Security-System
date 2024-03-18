/*
 * i2c.h
 * Description: Header file for the TWI(I2C) AVR driver
 *  Created on: Mar 7, 2024
 *      Author: abdalla
 */

#ifndef I2C_H_
#define I2C_H_

#include "std_types.h"

/*******************************************************************************
 *                      Preprocessor Macros                                    *
 *******************************************************************************/

/* I2C Status Bits in the TWSR Register */
#define TWI_START         0x08 		/* Start condition transmitted */
#define TWI_REP_START     0x10 		/* Repeated start condition transmitted */
#define TWI_MT_SLA_W_ACK  0x18 		/* Master transmit (slave address + Write request) to slave, ACK received from slave */
#define TWI_MT_SLA_R_ACK  0x40 		/* Master transmit (slave address + Read request) to slave, ACK received from slave */
#define TWI_MT_DATA_ACK   0x28 		/* Master transmit data, ACK received from slave */
#define TWI_MR_DATA_ACK   0x50 		/* Master received data, ACK sent to slave */
#define TWI_MR_DATA_NACK  0x58 		/* Master received data, NACK sent to slave */

/*******************************************************************************
 *                       Types Declaration                                     *
 *******************************************************************************/

typedef uint8 TWI_Address;

/* Enum defining TWI BIT Rates Modes */
typedef enum {
    NORMAL_MODE     = 100000UL,   	/* 100 kHz */
    FAST_MODE       = 400000UL,   	/* 400 kHz */
    FAST_MODE_PLUS  = 1000000UL,  	/* 1 MHz */
    HIGH_SPEED_MODE = 3400000UL   	/* 3.4 MHz */
} TWI_BaudRate;

/* Structure defining TWI Configuration */
typedef struct {
    TWI_BaudRate bit_rate; 			/* Bit rate for TWI communication */
    TWI_Address address; 			/* Address of the TWI slave device */
} TWI_ConfigType;

/* Enum defining TWI operation with slave */
typedef enum {
    WRITE,  						/* Write operation for TWI communication */
    READ    						/* Read operation for TWI communication */
} TWI_SlaveReadWrite;

/*******************************************************************************
 *                      Functions Prototypes                                   *
 *******************************************************************************/

/* Initialize TWI (I2C) communication */
void TWI_init(const TWI_ConfigType * Config_Ptr);

/* Transmit start condition */
void TWI_start(void);

/* Transmit stop condition */
void TWI_stop(void);

/* Write a byte to TWI bus */
void TWI_writeByte(uint8 byte);

/* Send slave address and specify read/write direction */
void TWI_sendSlaveAddress(uint8 slave_address, TWI_SlaveReadWrite read_write);

/* Read a byte from TWI bus with ACK */
uint8 TWI_readByteWithACK(void);

/* Read a byte from TWI bus with NACK */
uint8 TWI_readByteWithNACK(void);

/* Get current status of TWI bus */
uint8 TWI_getStatus(void);

#endif /* I2C_H_ */
