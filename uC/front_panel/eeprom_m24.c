/*! \file eeprom_m24.c \brief EEPROM interface
 * \author Mikael Larsmark, SM2WMV
 * \date 2008-12-09
 */
//    Copyright (C) 2008  Mikael Larsmark, SM2WMV
//
//    This program is free software: you can redistribute it and/or modify
//    it under the terms of the GNU General Public License as published by
//    the Free Software Foundation, either version 3 of the License, or
//    (at your option) any later version.
//
//    This program is distributed in the hope that it will be useful,
//    but WITHOUT ANY WARRANTY; without even the implied warranty of
//    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//    GNU General Public License for more details.
//
//    You should have received a copy of the GNU General Public License
//    along with this program.  If not, see <http://www.gnu.org/licenses/>.

#include <stdio.h>
#include <stdlib.h>
#include <avr/io.h>
#include <string.h>
#include <avr/interrupt.h>

#include "eeprom_m24.h"
#include "../i2c.h"
#include "../i2cconf.h"

void __inline__ eeprom_tiny_delay(void) {
	for (volatile unsigned char i=0;i<50;i++)
		for (volatile unsigned char j=0;j<255;j++)
			__asm__ __volatile__("nop");
}

unsigned char eeprom_m24_write_byte(unsigned int eeprom_address, unsigned char value) {
	unsigned char temp[3];
	temp[0] = ((eeprom_address & 0xFF00) >> 8);
	temp[1] = eeprom_address & 0x00FF;
	temp[2] = value;
	
	unsigned char retval = I2C_OK;

	// disable TWI interrupt
	cbi(TWCR, TWIE);

	// send start condition
	i2cSendStart();
	i2cWaitForComplete();

	// send device address with write
	i2cSendByte((EEPROM_M24_ADDR & 0xFE));
	i2cWaitForComplete();

	// check if device is present and live
	if(inb(TWSR) == TW_MT_SLA_ACK)
	{
			i2cSendByte(temp[0]);
			i2cWaitForComplete();
			
			i2cSendByte(temp[1]);
			i2cWaitForComplete();
			
			i2cSendByte(temp[2]);
			i2cWaitForComplete();
	}
	else
	{
		// device did not ACK it's address,
		// data will not be transferred
		// return error
		retval = I2C_ERROR_NODEV;
	}

	// transmit stop condition
	// leave with TWEA on for slave receiving
	i2cSendStop();
	while( !(inb(TWCR) & BV(TWSTO)) );

	// enable TWI interrupt
	sbi(TWCR, TWIE);

	return retval;
}

unsigned char eeprom_m24_read_byte(unsigned int eeprom_address) {
	unsigned char temp_addr[2];
	
	temp_addr[0] = ((eeprom_address & 0xFF00) >> 8);
	temp_addr[1] = eeprom_address & 0x00FF;
	
	unsigned char data = 0;
	
	// disable TWI interrupt
	cbi(TWCR, TWIE);

	// send start condition
	i2cSendStart();
	i2cWaitForComplete();

	// send device address with read
	i2cSendByte(EEPROM_M24_ADDR & 0xFE);
	i2cWaitForComplete();	
	
	i2cSendByte(temp_addr[0]);
	i2cWaitForComplete();	
	i2cSendByte(temp_addr[1]);
	i2cWaitForComplete();	
	
	i2cSendStart();
	i2cWaitForComplete();
	
	i2cSendByte(EEPROM_M24_ADDR | 0x01);
	i2cWaitForComplete();	
	
	// check if device is present and live
	if(inb(TWSR) == TW_MR_SLA_ACK) {
		// accept receive data and ack it
		i2cReceiveByte(FALSE);
		i2cWaitForComplete();
		data = i2cGetReceivedByte();
	}
	
	i2cSendStop();

	// enable TWI interrupt
	sbi(TWCR, TWIE);

	return(data);
}

unsigned char eeprom_m24_write_block(unsigned int start_address, unsigned int length, unsigned char *data) {
	
	unsigned int current_address,last_address;
	unsigned char retval = I2C_OK;

	// disable TWI interrupt
	cbi(TWCR, TWIE);

	current_address = start_address;
	
	while(length > 0) {
		last_address = current_address;
		
		// send start condition
		i2cSendStart();
		i2cWaitForComplete();
	
		// send device address with write
		i2cSendByte((EEPROM_M24_ADDR & 0xFE));
		i2cWaitForComplete();
	
		// check if device is present and live
		if(inb(TWSR) == TW_MT_SLA_ACK)
		{
			i2cSendByte((current_address & 0xFF00)>>8);
			i2cWaitForComplete();
			
			i2cSendByte((current_address & 0x00FF));
			i2cWaitForComplete();
	
			while (length > 0) {
				i2cSendByte(*(data+(current_address-start_address)));
				i2cWaitForComplete();
				current_address++;
				
				length--;
				
				//If we are about to wrap around a page
				if ((current_address & 0xFFC0) != (last_address & 0xFFC0)) {
					break;
				}
			}
		}
		else
		{
			// device did not ACK it's address,
			// data will not be transferred
			// return error
			retval = I2C_ERROR_NODEV;
		}
	
		// transmit stop condition
		// leave with TWEA on for slave receiving
		i2cSendStop();
		while( !(inb(TWCR) & BV(TWSTO)) );
		
		eeprom_tiny_delay();
	}

	// enable TWI interrupt
	sbi(TWCR, TWIE);

	return retval;
}

unsigned char eeprom_m24_read_block(unsigned int start_address, unsigned int length, unsigned char *data) {
//	if ((start_address & 0xFFC0) != ((start_address+length) & 0xFFC0)) 
	unsigned char temp_addr[2];
	
	temp_addr[0] = ((start_address & 0xFF00) >> 8);
	temp_addr[1] = start_address & 0x00FF;
	
	unsigned int current_address = start_address,last_address = start_address;
	
	unsigned char ptr_pos=0;
	
	unsigned char retval = I2C_OK;
	
	// disable TWI interrupt
	cbi(TWCR, TWIE);
	
	while(length > 0) {
		last_address = current_address;
		// send start condition
		i2cSendStart();
		i2cWaitForComplete();
	
		// send device address with read
		i2cSendByte(EEPROM_M24_ADDR & 0xFE);
		i2cWaitForComplete();	
		
		i2cSendByte((current_address & 0xFF00)>>8);
		i2cWaitForComplete();	
		i2cSendByte(current_address & 0x00FF);
		i2cWaitForComplete();	
		
		i2cSendStart();
		i2cWaitForComplete();
		
		i2cSendByte(EEPROM_M24_ADDR | 0x01);
		i2cWaitForComplete();	
		// check if device is present and live
		if(inb(TWSR) == TW_MR_SLA_ACK) {
			// accept receive data and ack it
			while(length > 1)
			{
				i2cReceiveByte(TRUE);
				i2cWaitForComplete();
				*(data+ptr_pos++) = i2cGetReceivedByte();
				
				// decrement length
				length--;
				current_address++;
				
				//If we are about to wrap around a page
				if ((current_address & 0xFFC0) != (last_address & 0xFFC0)) {
					//TODO: Finish this wrap option
					break;
				}
			}
		}
		else
			retval = I2C_ERROR_NODEV;
		
		i2cReceiveByte(FALSE);
		i2cWaitForComplete();
		*(data+ptr_pos++) = i2cGetReceivedByte();
		length--;
		current_address++;
		
		i2cSendStop();
	}

	// enable TWI interrupt
	sbi(TWCR, TWIE);

	return(retval);
}