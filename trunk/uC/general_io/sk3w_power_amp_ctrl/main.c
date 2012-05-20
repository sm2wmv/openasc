/*! \file general_io/main.c 
 *  \ingroup general_io_group
 *  \brief Main file of the General I/O card 
 *  \author Mikael Larsmark, SM2WMV
 *  \date 2010-05-18
 *  \code #include "general_io/main.c" \endcode
 */
//    Copyright (C) 2009  Mikael Larsmark, SM2WMV
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
#include <string.h>
#include <avr/io.h>
#include <avr/interrupt.h>


#include "board.h"
#include "qpn_port.h"
#include "pa.h"
#include "bus_handler.h"
#include <i2c.h>
#include <delay.h>


/*! Main function of the General I/O */
int main(void)
{
	/*
	DDRC = 0x3f;
	DDRF = 0x00;
	PORTF = 0xff;
	for (;;)
	{
		PORTC = PINF;
	}
	*/
	
	pa_init();
  QF_run();        /* transfer control to QF-nano */

  return 0;
}
