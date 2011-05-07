/*! \file 4cx1500b/ctrl_box_motherboard//main.c 
 *  \ingroup  amp_motherboard_group
 *  \brief Main file of the motherboard
 *  \author Mikael Larsmark, SM2WMV
 *  \date 2010-01-25
 *  \code #include "4cx1500b/ctrl_box_motherboard//main.c" \endcode
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

#include <avr/parity.h> 

#include "main.h"
#include "board.h"
#include "usart.h"
#include "init.h"
#include "computer_interface.h"
#include "../../delay.h"
#include "../../internal_comm.h"
#include "../../internal_comm_commands.h"
#include "../../wmv_bus/bus_commands.h"

#include "computer_interface.h"

//! Variable used to keep track of the last state of the ON/OFF button so we can see when it has been pressed and released
unsigned char btn_on_off_last_state = 1;

/*! Counter which keeps track of how long time ago it was since we started the box. This is used at startup so that 
		we ignore button actions on the ON/OFF button for a certain time at startup */
unsigned int counter_time_start=0;

unsigned long long global_bcd_count = 0;

//! \brief Tiny delay function
void __inline__ tiny_delay(void) {
	for (volatile unsigned char i=0;i<50;i++)
		for (volatile unsigned char j=0;j<255;j++)
			__asm__ __volatile__("nop");
}

/*! \brief Parse an internal communication message 
 *  \param message The message that we wish to parse */
void parse_internal_comm_message(UC_MESSAGE message) {
	char temp=0;
	unsigned char from_addr = 0;
	
	#ifdef INT_COMM_DEBUG	
		printf("P: 0x%02X\n",message.cmd);	
		
		for (int i=0;i<message.length;i++)
			printf("[%i]: %i\n",i,message.data[i]);
	#endif
		
	switch(message.cmd) {
		case UC_COMM_MSG_ACK:
			internal_comm_message_acked();
			break;
		case UC_COMM_MSG_NACK:
			internal_comm_message_nacked();
			break;
		case INT_COMM_REDIRECT_DATA:
			//computer_interface_send(message.data[0], message.data[1], (void *)message.data[2]);
			break;
		case INT_COMM_GET_BAND_BCD_STATUS:
			//Read the status of the BCD input on the top floor and return it
			/* PF0 - Input  - BCD input Bit 2
			 * PF1 - Input  - BCD input Bit 3
 			 * PF2 - Input  - BCD input Bit 0
			 * PF3 - Input  - BCD input Bit 1 */
			
			temp = (PINF & (1<<2)) >> 2;
			temp |= (PINF & (1<<3)) >> 2;
			temp |= (PINF & (1<<0)) << 2;
			temp |= (PINF & (1<<1)) << 2;
			
			internal_comm_add_tx_message(INT_COMM_GET_BAND_BCD_STATUS, 1, &temp);
			break;
		case INT_COMM_PULL_THE_PLUG:
			//Will drop the voltage to the input relay
			PORTB &= ~(1<<7);
			break;
    case INT_COMM_PC_SEND_TO_ADDR:
      computer_interface_tx_message(message.length,message.data);
      break;
		default:
			break;
	}
}

//! Main function of the motherboard
int main(void) {
	delay_ms(100);
	//Pull the input voltage relay
	DDRB |= (1<<7);
	PORTB |= (1<<7);

	cli();
	
	//Initialize internal communication
	internal_comm_init((void*)parse_internal_comm_message,(void*)usart0_transmit);
	
	computer_interface_init();
	
	init_ports();
	init_timer_0();
	
	//19.2kbaud - computer comm
	usart1_init(47);
	fdevopen((void*)usart1_transmit, (void*)usart1_receive_loopback);
	
	//19.2kbaud - internal comm
	usart0_init(47);
	
	//printf("Motherboard started\n");
	
	sei();
	
	delay_ms(100);
	
	while(1) {
		computer_interface_send_data();
		computer_interface_parse_data();
		
		//Poll the RX queue in the internal comm to see if we have any new messages to be PARSED
		internal_comm_poll_rx_queue();
		
		//Poll the TX queue in the internal comm to see if we have any new messages to be SENT
		internal_comm_poll_tx_queue();
		
		//Check if ON/OFF button was pressed, if so we send a message to the front panel
		//which in it's turn will send a message to turn the device off after all settings have been saved
		//and messages have been sent out to all slave cards
		if ((btn_on_off_last_state == 0) && (PINB & (1<<6))) {
			while ((PINB & (1<<6)));
			delay_s(1);
			internal_comm_add_tx_message(INT_COMM_TURN_DEVICE_OFF,0,0);
			btn_on_off_last_state = 1;
		}
	}
}

/*! \brief Output compare 0 interrupt - "called" with 1ms intervals */
ISR(SIG_OUTPUT_COMPARE0) {
	counter_time_start++;
	
	computer_interface_1ms_tick();
	
  internal_comm_1ms_timer();
}

/*! \brief Output overflow 0 interrupt */
ISR(SIG_OVERFLOW0) {
	
}