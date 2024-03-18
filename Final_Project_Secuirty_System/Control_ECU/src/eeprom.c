/*
 * eeprom.c
 *	Source file for the External EEPROM Memory
 *  Created on: Mar 9, 2024
 *      Author: abdalla
 */

#include "eeprom.h"
#include "i2c.h"
#include <avr/io.h>
#include "util/delay.h"

/*
 * Function: EEPROM_writeByte
 * ----------------------------
 * Writes a single byte of data to the EEPROM at the specified memory address.
 *
 * memory_address: The memory address where the data will be written.
 * byte: The data byte to be written.
 *
 * returns: SUCCESS if the operation succeeds, ERROR otherwise.
 */
uint8 EEPROM_writeByte(uint16 memory_address, uint8 byte)
{
	/* Variable to hold device address*/
	uint8 device_address;

	/* Send the Start Bit */
	TWI_start();

	/* If the Status of the TWI not as Required return error */
	if(TWI_getStatus() != TWI_START)
		return ERROR;
	/*
	 * Calculate the device address of the EEPROM M24C16.
   	 * The device address consists of 1010 (fixed), followed by three address bits A10, A9, A8, and the read/write bit.
   	 * The left-shift operation by 3 bits accommodates the structure of the device address.
	 *
	 * Extract the relevant bits (A10, A9, A8) from the 'memory address' variable using bitwise AND with the mask 0x0700.
   	 * This operation isolates bits 8 to 10 of the 'memory address'.
   	 * After the bitwise AND operation, the result is shifted right by 8 bits to properly align with the device address structure.
	 *
	 * The resulting device address is formed by combining the shifted 'twi->address' with the processed part of the 'memory address' variable
   	 * using bitwise OR.
   	 *								device    memory address
   	 *								address	  most 3 bit
   	 * final format (7bit address) : 1010      a10 a9 a8
	 * */
	device_address = ((uint8)((EEPROM_ADDRESS) << 3 |((memory_address & 0x0700) >> 8)));

	TWI_sendSlaveAddress(device_address, WRITE);

	/* If the Status of the TWI not as Required return error */
    if (TWI_getStatus() != TWI_MT_SLA_W_ACK)
        return ERROR;

    /* Send the remaining A7 -> A0 memory location */
    TWI_writeByte((uint8)(memory_address & 0x00FF));

    /* If the Status of the TWI not as Required return error */
    if (TWI_getStatus() != TWI_MT_DATA_ACK)
    	return ERROR;

    /* write byte to eeprom */
    TWI_writeByte(byte);

    /* If the Status of the TWI not as Required return error */
    if (TWI_getStatus() != TWI_MT_DATA_ACK)
    	return ERROR;

    /* Send the Stop Bit */
    TWI_stop();

    /* Return success as indication of write succeed */
    return SUCCESS;
}

/*
 * Function: EEPROM_readByte
 * --------------------------
 * Reads a single byte of data from the EEPROM at the specified memory address.
 *
 * memory_address: The memory address from which the data will be read.
 * byte: Pointer to store the read data byte.
 *
 * returns: SUCCESS if the operation succeeds, ERROR otherwise.
 */
uint8 EEPROM_readByte(uint16 memory_address, uint8 *byte)
{
	/* Variable to hold device address*/
	uint8 device_address;

	/* Send the Start Bit */
	TWI_start();

	/* If the Status of the TWI not as Required return error */
	if(TWI_getStatus() != TWI_START)
		return ERROR;
	/*
	 * Calculate the device address of the EEPROM M24C16.
   	 * The device address consists of 1010 (fixed), followed by three address bits A10, A9, A8, and the read/write bit.
   	 * The left-shift operation by 3 bits accommodates the structure of the device address.
	 *
	 * Extract the relevant bits (A10, A9, A8) from the 'memory address' variable using bitwise AND with the mask 0x0700.
   	 * This operation isolates bits 8 to 10 of the 'memory address'.
   	 * After the bitwise AND operation, the result is shifted right by 8 bits to properly align with the device address structure.
	 *
	 * The resulting device address is formed by combining the shifted 'twi->address' with the processed part of the 'memory address' variable
   	 * using bitwise OR.
   	 *								device    memory address
   	 *								address	  most 3 bit
   	 * final format (7bit address) : 1010      a10 a9 a8
	 * */
	device_address = ((uint8)((EEPROM_ADDRESS) << 3 |((memory_address & 0x0700) >> 8)));

	TWI_sendSlaveAddress(device_address, WRITE);

	/* If the Status of the TWI not as Required return error */
    if (TWI_getStatus() != TWI_MT_SLA_W_ACK)
        return ERROR;

    /* Send the remaining A7 -> A0 memory location */
    TWI_writeByte((uint8)(memory_address & 0x00FF));

    /* If the Status of the TWI not as Required return error */
    if (TWI_getStatus() != TWI_MT_DATA_ACK)
    	return ERROR;

	/* Send repeated Start Bit */
	TWI_start();

	/* If the Status of the TWI not as Required return error */
	if(TWI_getStatus() != TWI_REP_START)
		return ERROR;

	/* Access the memory location but for read this time */
	TWI_sendSlaveAddress(device_address, READ);

	/* If the Status of the TWI not as Required return error */
    if (TWI_getStatus() != TWI_MT_SLA_R_ACK)
        return ERROR;

    *byte = TWI_readByteWithNACK();

	/* If the Status of the TWI not as Required return error */
    if (TWI_getStatus() != TWI_MR_DATA_NACK)
        return ERROR;

    /* Send the Stop Bit */
    TWI_stop();

    /* Return success as indication of write succeed */
    return SUCCESS;
}

/*
 * Function: EEPROM_writeData
 * ---------------------------
 * Writes an array of data to the EEPROM starting from the specified memory address.
 *
 * memory_address: The starting memory address where the data will be written.
 * arr_data: Pointer to the array of data to be written.
 * data_size: The size of the data array.
 *
 * returns: SUCCESS if the operation succeeds, ERROR otherwise.
 */
uint8 EEPROM_writeData(uint16 memory_address, uint8* arr_data, uint8 data_size)
{
	/* Variable to hold device address*/
	uint8 device_address, i;

	/* Send the Start Bit */
	TWI_start();

	/* If the Status of the TWI not as Required return error */
	if(TWI_getStatus() != TWI_START)
		return ERROR;
	/*
	 * Calculate the device address of the EEPROM M24C16.
   	 * The device address consists of 1010 (fixed), followed by three address bits A10, A9, A8, and the read/write bit.
   	 * The left-shift operation by 3 bits accommodates the structure of the device address.
	 *
	 * Extract the relevant bits (A10, A9, A8) from the 'memory address' variable using bitwise AND with the mask 0x0700.
   	 * This operation isolates bits 8 to 10 of the 'memory address'.
   	 * After the bitwise AND operation, the result is shifted right by 8 bits to properly align with the device address structure.
	 *
	 * The resulting device address is formed by combining the shifted 'twi->address' with the processed part of the 'memory address' variable
   	 * using bitwise OR.
   	 *								device    memory address
   	 *								address	  most 3 bit
   	 * final format (7bit address) : 1010      a10 a9 a8
	 * */
	device_address = ((uint8)((EEPROM_ADDRESS) << 3 |((memory_address & 0x0700) >> 8)));

	TWI_sendSlaveAddress(device_address, WRITE);

	/* If the Status of the TWI not as Required return error */
    if (TWI_getStatus() != TWI_MT_SLA_W_ACK)
        return ERROR;

    /* Send the remaining A7 -> A0 memory location */
    TWI_writeByte((uint8)(memory_address & 0x00FF));

    /* If the Status of the TWI not as Required return error */
    if (TWI_getStatus() != TWI_MT_DATA_ACK)
    	return ERROR;

    /*
     * Loop through the hole array of data and write each one in the eeprom.
     * the pointer in the eeprom increase automatically after each write operation.
     *  */
    for (i = 0; i < data_size; i++)
    {
		/* write byte to eeprom */
		TWI_writeByte(arr_data[i]);

		/* If the Status of the TWI not as Required return error */
		if (TWI_getStatus() != TWI_MT_DATA_ACK)
			return ERROR;

        /* Delay 10 ms for the write time */
		_delay_ms(10);
    }

    /* Send the Stop Bit */
    TWI_stop();

    /* Return success as indication of write succeed */
    return SUCCESS;
}

/*
 * Function: EEPROM_readData
 * --------------------------
 * Reads an array of data from the EEPROM starting from the specified memory address.
 *
 * memory_address: The starting memory address from which the data will be read.
 * arr_data: Pointer to store the read data array.
 * data_size: The size of the data array to be read.
 *
 * returns: SUCCESS if the operation succeeds, ERROR otherwise.
 */
uint8 EEPROM_readData(uint16 memory_address, uint8* arr_data, uint8 data_size)
{
	/* Variable to hold device address*/
	uint8 device_address, i;

	/* Send the Start Bit */
	TWI_start();

	/* If the Status of the TWI not as Required return error */
	if(TWI_getStatus() != TWI_START)
		return ERROR;
	/*
	 * Calculate the device address of the EEPROM M24C16.
   	 * The device address consists of 1010 (fixed), followed by three address bits A10, A9, A8, and the read/write bit.
   	 * The left-shift operation by 3 bits accommodates the structure of the device address.
	 *
	 * Extract the relevant bits (A10, A9, A8) from the 'memory address' variable using bitwise AND with the mask 0x0700.
   	 * This operation isolates bits 8 to 10 of the 'memory address'.
   	 * After the bitwise AND operation, the result is shifted right by 8 bits to properly align with the device address structure.
	 *
	 * The resulting device address is formed by combining the shifted 'twi->address' with the processed part of the 'memory address' variable
   	 * using bitwise OR.
   	 *								device    memory address
   	 *								address	  most 3 bit
   	 * final format (7bit address) : 1010      a10 a9 a8
	 * */
	device_address = ((uint8)((EEPROM_ADDRESS) << 3 |((memory_address & 0x0700) >> 8)));

	TWI_sendSlaveAddress(device_address, WRITE);

	/* If the Status of the TWI not as Required return error */
    if (TWI_getStatus() != TWI_MT_SLA_W_ACK)
        return ERROR;

    /* Send the remaining A7 -> A0 memory location */
    TWI_writeByte((uint8)(memory_address & 0x00FF));

    /* If the Status of the TWI not as Required return error */
    if (TWI_getStatus() != TWI_MT_DATA_ACK)
    	return ERROR;

	/* Send repeated Start Bit */
	TWI_start();

	/* If the Status of the TWI not as Required return error */
	if(TWI_getStatus() != TWI_REP_START)
		return ERROR;

	/* Access the memory location but for read this time */
	TWI_sendSlaveAddress(device_address, READ);

	/* If the Status of the TWI not as Required return error */
    if (TWI_getStatus() != TWI_MT_SLA_R_ACK)
        return ERROR;

    /* Loop through the hole data array */
    for (i = 0; i < data_size - 1; i++)
    {
    	/* Read each byte in the eeprom
    	 * the pointer inside the eeprom increase automatically.
    	 * */
        arr_data[i] = TWI_readByteWithACK();

    	/* If the Status of the TWI not as Required return error */
        if (TWI_getStatus() != TWI_MR_DATA_ACK)
            return ERROR;
    }

    /* Read the last byte in the eeprom with NACK to indicate the last byte */
    arr_data[i] = TWI_readByteWithNACK();

	/* If the Status of the TWI not as Required return error */
    if (TWI_getStatus() != TWI_MR_DATA_NACK)
        return ERROR;

    /* Send the Stop Bit */
    TWI_stop();

    /* Return success as indication of write succeed */
    return SUCCESS;
}
