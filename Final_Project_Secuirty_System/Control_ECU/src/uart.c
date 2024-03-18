/*
 * uart.c
 *
 *  Created on: Feb 24, 2024
 *      Author: abdalla
 */

#include "uart.h"
#include <avr/io.h>
#include "common_macros.h"

/*******************************************************************************
 *                      Functions Definitions                                  *
 *******************************************************************************/

/*
 * Description :
 * Functional responsible for Initialize the UART device by:
 * 1. Setup the Frame format like number of data bits, parity bit type and number of stop bits.
 * 2. Enable the UART.
 * 3. Setup the UART baud rate.
 */
void UART_init(const UART_ConfigType *config_ptr)
{
	uint16 ubbr;

	UCSRA = (1<<U2X); /* U2X = 1 for double transmission speed */

	/************************** UCSRB Description **************************
	 * RXCIE = 0 Disable USART RX Complete Interrupt Enable
	 * TXCIE = 0 Disable USART Tx Complete Interrupt Enable
	 * UDRIE = 0 Disable USART Data Register Empty Interrupt Enable
	 * RXEN  = 1 Receiver Enable
	 * RXEN  = 1 Transmitter Enable
	 * UCSZ2 = 0 For 8-bit data mode
	 * RXB8 & TXB8 not used for 8-bit data mode
	 ***********************************************************************/
	UCSRB = (1<<RXEN)|(1<<TXEN);

	/************************** UCSRC Description **************************
	 * URSEL   = 1 The URSEL must be one when writing the UCSRC
	 * UMSEL   = 0 Asynchronous Operation
	 * UCPOL   = 0 Used with the Synchronous operation only
	 ***********************************************************************/
	UCSRC = (1<<URSEL);

	/* Set the parity bits (UPM1:0) based on the configuration */
	UCSRC = (UCSRC & 0xCF) | ((config_ptr->parity & 0x03) << 4);

	/* Set the stop bit (USBS) based on the configuration */
	if (config_ptr->stop_bit)
	{
		UCSRC |= (1<<USBS); /* Two stop bits */
	}
	else
	{
		UCSRC &= ~(1<<USBS); /* One stop bit */
	}

	/* Set the data size bits (UCSZ1:0) based on the configuration */
	UCSRC = (UCSRC & 0xF9) | ((config_ptr->data_size & 0x03) << 1);

	/* calculating the 12 bit number baud rate */
	ubbr = (uint16) (((float)(F_CPU)/(8 * config_ptr->baud_rate)) - 1);

	/* First 8 bits from the BAUD_PRESCALE inside UBRRL and last 4 bits in UBRRH*/
	/* URSEL = 0  The URSEL must be one when writing the UCSRC */
	UBRRH = ubbr>>8;
	UBRRL = ubbr;
}

/*
 * Description :
 * Functional responsible for send byte to another UART device.
 */
void UART_sendByte(const uint8 data)
{
	/*
	 * UDRE flag is set when the Tx buffer (UDR) is empty and ready for
	 * transmitting a new byte so wait until this flag is set to one
	 */
	while (BIT_IS_CLEAR(UCSRA,UDRE));

	/*
	 * Put the required data in the UDR register and it also clear the UDRE flag as
	 * the UDR register is not empty now
	 */
	UDR = data;
}

/*
 * Description :
 * Functional responsible for receive byte from another UART device.
 */
uint8 UART_recieveByte(void)
{
	/* RXC flag is set when the UART receive data so wait until this flag is set to one */
	while(BIT_IS_CLEAR(UCSRA,RXC));

	/*
	 * Read the received data from the Rx buffer (UDR)
	 * The RXC flag will be cleared after read the data
	 */
    return UDR;
}

/*
 * Description :
 * Send the required string through UART to the other UART device.
 */
void UART_sendString(const uint8 *Str)
{
	uint8 i = 0;

	/* Loop until the end of the string */
	while(Str[i] != '\0')
	{
		/* Send each character at a time */
		UART_sendByte(Str[i]);
		i++;
	}

	/* Send the last STRING_END SYMBOL*/
	UART_sendByte(STRING_END);
}

/*
 * Description :
 * Receive the required string until the symbol in the header file  through UART from the other UART device.
 */
void UART_receiveString(uint8 *Str)
{
	uint8 i = 0;

	/* Receive the whole string until the STRING_END symbol */
	do
	{
		/* Receive the first byte */
		Str[i] = UART_recieveByte();
		i++;

		/* Check if the last received byte is the STRING_END symbol*/
	}while(Str[i - 1] != STRING_END);

	/* After receiving the whole string plus the STRING_END symbol, replace this symbol with '\0' */
	Str[i - 1] = '\0';
}

/*
 * Description :
 * Send the required Array of data through UART to the other UART device.
 */
void UART_sendData(const uint8 *arr, uint8 arr_size)
{
	uint8 i = 0;

	/* Loop until the end of the array */
	for (i = 0; i < arr_size; i++)
	{
		/* Send each Byte at a time */
		UART_sendByte(arr[i]);
	}
}

/*
 * Description :
 * Receive the required Array of data through UART to the other UART device.
 */
void UART_receiveData(uint8 *arr, uint8 arr_size)
{
	uint8 i = 0;

	/* Loop until the end of the array */
	for (i = 0; i < arr_size; i++)
	{
		/* receive each Byte at a time */
		arr[i] = UART_recieveByte();
	}
}
