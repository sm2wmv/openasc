/*! \file 4cx1500b/ctrl_box_front/event_handler.c '
 *  \brief Event handler of various things
 *  \ingroup amp_front_panel_group
 *  \author Mikael Larsmark, SM2WMV
 *  \date 2010-09-04
 *  \code #include "4cx1500b/ctrl_box_front/event_handler.c" \endcode
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
#include <string.h>

#include "event_handler.h"
#include "main.h"
#include "board.h"
#include "display.h"
#include "glcd.h"
#include "led_control.h"
#include "../../delay.h"
#include "../../i2c.h"
#include "../../global.h"
#include "rotary_encoder.h"
#include "glcd.h"
#include "../../internal_comm.h"
#include "../../wmv_bus/bus_ping.h"

//#define DEBUG_WMV_BUS 1


//#define ERROR_DEBUG

extern unsigned int main_flags;

unsigned char ascii_comm_device_addr = 0;

/*! \brief Function which will parse the internal communication message 
 *  \param message The message that we wish to parse */
void event_internal_comm_parse_message(UC_MESSAGE message) {
	#ifdef INT_COMM_DEBUG
		printf("0x%02X\n",message.cmd);
	#endif
	
	//Init the sequence of saving all data and disable all outputs activated by this unit
	switch(message.cmd) {
		case UC_COMM_MSG_ACK:
			internal_comm_message_acked();
			break;
		case UC_COMM_MSG_NACK:
			internal_comm_message_nacked();
			break;
		case INT_COMM_TURN_DEVICE_OFF:
			event_add_message((void *)shutdown_device,3000,0);
			break;
		case INT_COMM_GET_BAND_BCD_STATUS:
			
      break;
    case INT_COMM_PC_CONNECT_TO_ADDR:
      ascii_comm_device_addr = message.data[0];
      break;
    case INT_COMM_PC_SEND_TO_ADDR:
      if (ascii_comm_device_addr != 0x00) {
        bus_add_tx_message(bus_get_address(), ascii_comm_device_addr,(1<<BUS_MESSAGE_FLAGS_NEED_ACK),BUS_CMD_ASCII_DATA,message.length,message.data);
      }
      break;
		default:
			break;
	}
}

/*! \brief The pulse sensor was turned up */
void event_pulse_sensor_up(void) {
	clear_screensaver_timer();
	
}

/*! \brief The pulse sensor was turned down */
void event_pulse_sensor_down(void) {
	clear_screensaver_timer();
	
}

/*! \brief Function to be called if we wish to update the display */
void event_update_display(void) {

}

/*! \brief Function which will poll all buttons and perform the proper action depending on their state */
void event_poll_buttons(void) {
	
}

/*! \brief Parse a message from the communication bus */
void event_bus_parse_message(void) {
	BUS_MESSAGE bus_message = rx_queue_get();
	
	#ifdef DEBUG_WMV_BUS
		printf("DEBUG-> From addr: 0x%02X\n",bus_message.from_addr);
		printf("DEBUG-> To addr:   0x%02X\n",bus_message.to_addr);
		printf("DEBUG-> Command:   0x%02X\n",bus_message.cmd);
		printf("DEBUG-> Length:    0x%02X\n",bus_message.length);
	
		for (unsigned char i=0;i<bus_message.length;i++)
			printf("DEBUG-> Data #%02i:  0x%02X\n",i,bus_message.data[i]);
	#endif
	
	if (bus_message.cmd == BUS_CMD_ACK)
		bus_message_acked(bus_message.from_addr);
	else if (bus_message.cmd == BUS_CMD_NACK)
		bus_message_nacked(bus_message.from_addr, bus_message.data[0]);
	
	//Drop the message
	rx_queue_drop();
	
	#ifdef DEBUG_WMV_BUS
		printf("DEBUG-> Message dropped\n");
	#endif	
}