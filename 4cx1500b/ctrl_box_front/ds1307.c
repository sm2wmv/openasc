/*! \file ds1307.c \brief Main file of the front panel
 * \author Mikael Larsmark, SM2WMV
 * \date 2008-04-30
/*! \file 4cx1500b/ctrl_box_front/ds1307.c
 *  \ingroup amp_front_panel_group 
 *  \brief Realtime clock
 *  \author Mikael Larsmark, SM2WMV
 *  \date 2010-01-25
 *  \code #include "4cx1500b/ctrl_box_front/ds1307.c" \endcode
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
#include <avr/interrupt.h>

#include "ds1307.h"
#include "../../i2c.h"
#include "../../delay.h"

//! Flag which is set to 1 if a read request is allowed to the ds1307
unsigned char allowed_to_read = 0;
//! Variable which contains information of the current time/date
unsigned char *time_data;

/*! \brief Initialize the realtime clock on the front panel */
void ds1307_init(void) {
	time_data = malloc(9);
	
/*	*(time_data+0) = 0x00;
	*(time_data+1) = *time_data & 0x7F;	// enable oscillator (bit 7=0)
  *(time_data+2) = 0x28;	// minute = 59
  *(time_data+3) = 0x20;	// hour = 05 ,24-hour mode(bit 6=0)
  *(time_data+4) = 0x04;	// Day = 1 or sunday
  *(time_data+5) = 0x30;	// Date
  *(time_data+6) = 0x04;	// month
  *(time_data+7) = 0x08;	// year
	*(time_data+8) = 0x00;

	
	i2cMasterSend(DS1307_ADDR,9,(unsigned char *)time_data);*/
	
	allowed_to_read = 1;
}

/*! \brief Set the current time of the realtime clock 
 *  \param data data[0] = seconds, data[1] = minutes, data[2] = hours, data[3] = Day, data[4] = Date, data[5] = month, data[6] = year */
void ds1307_set_time(char *data) {
	//Flag to avoid a read from the real-time clock during 
	//our time adjustment
	allowed_to_read = 0;
		
	unsigned char b0=0,b1=0;
		
	*(time_data+0) = 0x00;							//REG ADDR 0
	
	b1 = data[0] / 10;
	b0 = (data[0] - 10*b1) + (b1<<4);
	*(time_data+1) = b0 & 0x7F;	// seconds, enable oscillator (bit 7=0)
  
	b1 = data[1] / 10;
	b0 = (data[1] - 10*b1) + (b1<<4);
	*(time_data+2) = b0;					// minute
  
	b1 = data[2] / 10;
	b0 = (data[2] - 10*b1) + (b1<<4);
	*(time_data+3) = b0; 				// hour
	
  *(time_data+4) = *(data+3);					// Day 
	
	b1 = data[4] / 10;
	b0 = (data[4] - 10*b1) + (b1<<4);	
	*(time_data+5) = b0;					// Date
	
	b1 = data[5] / 10;
	b0 = (data[5] - 10*b1) + (b1<<4);	

	*(time_data+6) = b0;					// month
	
	b1 = data[6] / 10;
	b0 = (data[6] - 10*b1) + (b1<<4);	
	*(time_data+7) = b0;					// year
	*(time_data+8) = 0x00;
	
	i2cMasterSend(DS1307_ADDR,9,(unsigned char *)time_data);
	
	//Allowed to read from the real-time clock again
	allowed_to_read = 1;
}

/*! \brief Retrieve the hour part of the time from the realtime clock 
 *  \return The current hour */
unsigned char ds1307_get_hours(void) {
	unsigned char temp = *(time_data+2) & 0x0F;
	temp += ((*(time_data+2) & 0x30)>>4)*10;
	return(temp);
}

/*! \brief Retrieve the minute part of the time from the realtime clock 
 *  \return The current minute */
unsigned char ds1307_get_minutes(void) {
	unsigned char temp = *(time_data+1) & 0x0F;
	temp += ((*(time_data+1) & 0x70)>>4)*10;
	return(temp);
}

/*! \brief Retrieve the seconds part of the time from the realtime clock 
 *  \return The current seconds */
unsigned char ds1307_get_seconds(void) {
	unsigned char temp = *(time_data+0) & 0x0F;
	temp += ((*(time_data+0) & 0x70)>>4)*10;
	return(temp);
}

/*! \brief Read the current time/date from the realtime clock. Stores the data and can be retrieved with the get functions in this file */
void ds1307_read(void) {
	if (allowed_to_read)
		i2cMasterReceiveNI(DS1307_ADDR,7,(unsigned char *)time_data);
}