/*
 * uart.h
 *
 *  Created on: Feb 24, 2024
 *      Author: abdalla
 */

#ifndef UART_H_
#define UART_H_

# include "std_types.h"

/*******************************************************************************
 *                                Definitions                                  *
 *******************************************************************************/
/* Define the character used to denote the end of a string */
#define STRING_END '@'

/* Define the a byte to indicate that the MCU is ready to receive DATA */
#define READY_TO_RECEVIE 0xFF

/*******************************************************************************
 *                               Types Declaration                             *
 *******************************************************************************/
/* Enum defining UART Parity types */
typedef enum
{
	NO_PARITY,      /* No parity */
	EVEN_PARITY = 2,/* Even parity */
	ODD_PARITY      /* Odd parity */
} UART_Parity;

/* Enum defining UART Stop Bit options */
typedef enum
{
	ONE_STOP_BIT,   /* One stop bit */
	TWO_STOP_BIT    /* Two stop bits */
} UART_StopBit;

/* Enum defining UART Data Size options */
typedef enum
{
	FIVE_BIT,       /* 5-bit data */
	SIX_BIT,        /* 6-bit data */
	SEVEN_BIT,      /* 7-bit data */
	EIGHT_BIT       /* 8-bit data */
} UART_DataSize;

/* Structure defining UART Configuration */
typedef struct
{
	UART_Parity parity;     /* Parity setting */
	UART_StopBit stop_bit;  /* Stop bit setting */
	UART_DataSize data_size;/* Data size setting */
	uint32 baud_rate;       /* Baud rate setting */
} UART_ConfigType;


/*******************************************************************************
 *                      Functions Prototypes                                   *
 *******************************************************************************/

/*
 * Description :
 * Functional responsible for Initialize the UART device by:
 * 1. Setup the Frame format like number of data bits, parity bit type and number of stop bits.
 * 2. Enable the UART.
 * 3. Setup the UART baud rate.
 */
void UART_init(const UART_ConfigType *config_ptr);

/*
 * Description :
 * Functional responsible for send byte to another UART device.
 */
void UART_sendByte(const uint8 data);

/*
 * Description :
 * Functional responsible for receive byte from another UART device.
 */
uint8 UART_recieveByte(void);

/*
 * Description :
 * Send the required string through UART to the other UART device.
 */
void UART_sendString(const uint8 *Str);

/*
 * Description :
 * Receive the required string until the STIRNG_END symbol through UART from the other UART device.
 */
void UART_receiveString(uint8 *Str);

/*
 * Description :
 * Send the required Array of data through UART to the other UART device.
 */
void UART_sendData(const uint8 *arr, uint8 arr_size);

/*
 * Description :
 * Receive the required Array of data through UART to the other UART device.
 */
void UART_receiveData(uint8 *arr, uint8 arr_size);



#endif /* UART_H_ */
