/*
 * i2c.c
 *	Description: Source file for the TWI(I2C) AVR driver
 *  Created on: Mar 7, 2024
 *      Author: abdalla
 */

#include "i2c.h"
#include "common_macros.h"
#include <avr/io.h>

/*******************************************************************************
 *                      Functions Definitions                                  *
 *******************************************************************************/

/* Initialize TWI (I2C) communication */
void TWI_init(const TWI_ConfigType * Config_Ptr)
{
	/* Configure the Bit rate of this device in the bus by the user*/
	TWBR = ((F_CPU/Config_Ptr->bit_rate * 2) - 8); /* This Equation for zero prescaler only */

	/* enable TWI */
	TWCR |= (1<<TWEN);

	/* Zero pre-scaler TWPS=00 */
	TWSR = 0x00;

	/* Configure the address of this device in the bus by the user*/
	TWAR = (TWAR & 0x01)|((Config_Ptr->address & 0x7F) << 1);
}

/* Transmit start condition */
void TWI_start(void)
{
    /*
	 * Clear the TWINT flag before sending the start bit TWINT=1
	 * send the start bit by TWSTA=1
	 * Enable TWI Module TWEN=1
	 */
	TWCR = (1<<TWINT)|(1<<TWSTA)|(1<<TWEN);

    /* Wait for TWINT flag set in TWCR Register (start bit is send successfully) */
	while (BIT_IS_CLEAR(TWCR, TWINT));
}

/* Transmit stop condition */
void TWI_stop(void)
{
    /*
	 * Clear the TWINT flag before sending the stop bit TWINT=1
	 * send the stop bit by TWSTO=1
	 * Enable TWI Module TWEN=1
	 */
	TWCR = (1<<TWINT)|(1<<TWEN)|(1<<TWSTO);
}

/* Send slave address and specify read/write direction */
void TWI_sendSlaveAddress(uint8 slave_address,TWI_SlaveReadWrite read_write)
{

	TWDR = ((slave_address & 0x7F) << 1)|(read_write & 0x01);

    /*
	 * Clear the TWINT flag before sending the data TWINT=1
	 * Enable TWI Module TWEN=1
	 */
	TWCR = (1<<TWINT) | (1<<TWEN);

    /* Wait for TWINT flag set in TWCR Register(data is send successfully) */
	while (BIT_IS_CLEAR(TWCR, TWINT));
}

/* Write a byte to TWI bus */
void TWI_writeByte(uint8 byte)
{
    /* Put data On TWI data Register */
	TWDR = byte;

    /*
	 * Clear the TWINT flag before sending the data TWINT=1
	 * Enable TWI Module TWEN=1
	 */
	TWCR = (1<<TWINT) | (1<<TWEN);

    /* Wait for TWINT flag set in TWCR Register(data is send successfully) */
	while (BIT_IS_CLEAR(TWCR, TWINT));
}

/* Read a byte from TWI bus with ACK */
uint8 TWI_readByteWithACK(void)
{
	/*
	 * Clear the TWINT flag before reading the data TWINT=1
	 * Enable sending ACK after reading or receiving data TWEA=1
	 * Enable TWI Module TWEN=1
	 */
	TWCR = (1 << TWINT) | (1 << TWEN) | (1 << TWEA);

    /* Wait for TWINT flag set in TWCR Register (data received successfully) */
	while (BIT_IS_CLEAR(TWCR, TWINT));

    /* Read Data */
	return TWDR;
}

/* Read a byte from TWI bus with NACK */
uint8 TWI_readByteWithNACK(void)
{
	/*
	 * Clear the TWINT flag before reading the data TWINT=1
	 * Enable sending NACK after reading or receiving data TWEA=0
	 * Enable TWI Module TWEN=1
	 */
	TWCR = (1 << TWINT) | (1 << TWEN);

    /* Wait for TWINT flag set in TWCR Register (data received successfully) */
	while (BIT_IS_CLEAR(TWCR, TWINT));

	/* Read Data */
	return TWDR;
}

/* Get current status of TWI bus */
uint8 TWI_getStatus(void)
{
    /* masking to eliminate first 3 bits and get the last 5 bits (status bits) */
	return (TWSR & 0xF8);
}
