/*
 * eeprom.h
 *	Header file for the External EEPROM Memory
 *  Created on: Mar 9, 2024
 *      Author: abdalla
 */

#ifndef EEPROM_H_
#define EEPROM_H_

#include "std_types.h"

/*******************************************************************************
 *                      Preprocessor Macros                                    *
 *******************************************************************************/
#define ERROR 	   		0  	/* Error code indicating failure */
#define SUCCESS    		1  	/* Success code indicating operation success */
#define EEPROM_ADDRESS 0x0A /* Address of the EEPROM device */

/*******************************************************************************
 *                      Functions Prototypes                                   *
 *******************************************************************************/

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
uint8 EEPROM_writeByte(uint16 memory_address, uint8 byte);

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
uint8 EEPROM_readByte(uint16 memory_address, uint8 *byte);

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
uint8 EEPROM_writeData(uint16 memory_address, uint8* arr_data, uint8 data_size);

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
uint8 EEPROM_readData(uint16 memory_address, uint8* arr_data, uint8 data_size);

#endif /* EEPROM_H_ */

