/*! \file main.c \brief Main file of the motherboard
 * \author Mikael Larsmark, SM2WMV
 * \date 2008-04-06
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
#include "../delay.h"
#include "../internal_comm.h"
#include "../internal_comm_commands.h"
#include "../wmv_bus/bus_commands.h"

#include "computer_interface.h"

#define PS2_CLK_LOW		PORTE &= ~(1<<6)
#define PS2_CLK_HIGH	PORTE |= (1<<6)
#define PS2_DATA_LOW	PORTA &= ~(1<<3)
#define PS2_DATA_HIGH	PORTA |= (1<<3)

unsigned char temp_count = 0;

unsigned int driver_output_state = 0;
unsigned int driver_output_type[12];

unsigned char btn_on_off_last_state = 1;

unsigned int counter_time_start=0;
unsigned int counter_ps2=0;

PS2_STRUCT ps2;

void __inline__ tiny_delay(void) {
	for (volatile unsigned char i=0;i<50;i++)
		for (volatile unsigned char j=0;j<255;j++)
			__asm__ __volatile__("nop");
}

/*! \brief Activate a driver output
* This function is used to activate an output on the relay driver output in the openASC
* It controls both the sink and source output at the same time.
* \param index The index of which output to activate (1-12)
*/
void activate_output(unsigned char index, unsigned char type) {
	driver_output_state |= (1<<(index-1));
	driver_output_type[index-1] = type;
	
	switch (index) {
		case 1 :	PORTC |= (1<<DRIVER_OUTPUT_1);
							break;
		case 2 :	PORTC |= (1<<DRIVER_OUTPUT_2);
							break;
		case 3 :	PORTC |= (1<<DRIVER_OUTPUT_3);
							break;
		case 4 :	PORTC |= (1<<DRIVER_OUTPUT_4);
							break;
		case 5 :	PORTC |= (1<<DRIVER_OUTPUT_5);
							break;
		case 6 :	PORTC |= (1<<DRIVER_OUTPUT_6);
							break;
		case 7 :	PORTC |= (1<<DRIVER_OUTPUT_7);
							break;
		case 8 :	PORTC |= (1<<DRIVER_OUTPUT_8);
							break;
		case 9 :	PORTA |= (1<<DRIVER_OUTPUT_9);
							break;
		case 10 :	PORTA |= (1<<DRIVER_OUTPUT_10);
							break;
		case 11 :	PORTA |= (1<<DRIVER_OUTPUT_11);
							break;
		case 12 :	PORTA |= (1<<DRIVER_OUTPUT_12);
							break;
		default: 
							break;
	}
}

/*! \brief Deactivate a driver output
* This function is used to deactivate an output on the relay outputs
* It controls both the sink and source output at the same time.
* \param index The index of which output to activate (1-12)
*/
void deactivate_output(unsigned char index) {
	driver_output_state &= ~(1<<(index-1));
	driver_output_type[index-1] = 0;
	
	switch (index) {
		case 1 :	PORTC &= ~(1<<DRIVER_OUTPUT_1);
							break;
		case 2 :	PORTC &= ~(1<<DRIVER_OUTPUT_2);
							break;
		case 3 :	PORTC &= ~(1<<DRIVER_OUTPUT_3);
							break;
		case 4 :	PORTC &= ~(1<<DRIVER_OUTPUT_4);
							break;
		case 5 :	PORTC &= ~(1<<DRIVER_OUTPUT_5);
							break;
		case 6 :	PORTC &= ~(1<<DRIVER_OUTPUT_6);
							break;
		case 7 :	PORTC &= ~(1<<DRIVER_OUTPUT_7);
							break;
		case 8 :	PORTC &= ~(1<<DRIVER_OUTPUT_8);
							break;
		case 9 :	PORTA &= ~(1<<DRIVER_OUTPUT_9);
							break;
		case 10 :	PORTA &= ~(1<<DRIVER_OUTPUT_10);
							break;
		case 11 :	PORTA &= ~(1<<DRIVER_OUTPUT_11);
							break;
		case 12 :	PORTA &= ~(1<<DRIVER_OUTPUT_12);
							break;
		default: 
							break;
	}
}

void parse_internal_comm_message(UC_MESSAGE message) {
	char temp=0;

	switch(message.cmd) {
		case INT_COMM_REDIRECT_DATA:
			computer_interface_send(message.data[0], message.data[1], (void *)message.data[2]);
			break;
		case BUS_CMD_DRIVER_ACTIVATE_ANT_OUTPUT :
			for (unsigned char i=0;i<message.length;i++)
				activate_output(message.data[i], BUS_CMD_DRIVER_ACTIVATE_ANT_OUTPUT);
			break;
		case BUS_CMD_DRIVER_DEACTIVATE_ANT_OUTPUT:
			for (unsigned char i=0;i<message.length;i++)
				deactivate_output(message.data[i]);
			break;
		case BUS_CMD_DRIVER_ACTIVATE_BAND_OUTPUT: 
			for (unsigned char i=0;i<message.length;i++)
				activate_output(message	.data[i], BUS_CMD_DRIVER_ACTIVATE_BAND_OUTPUT);
			break;
		case BUS_CMD_DRIVER_ACTIVATE_RX_ANT_OUTPUT:
			for (unsigned char i=0;i<message.length;i++)
				activate_output(message.data[i], BUS_CMD_DRIVER_ACTIVATE_RX_ANT_OUTPUT);
			break;
		case BUS_CMD_DRIVER_DEACTIVATE_ALL_OUTPUTS:
			for (unsigned char i=1;i<=12;i++)
				deactivate_output(i);
			break;
		case BUS_CMD_DRIVER_DEACTIVATE_ALL_ANT_OUTPUTS:
			for (unsigned char i=1;i<=12;i++)
				if (driver_output_type[i-1] == BUS_CMD_DRIVER_ACTIVATE_ANT_OUTPUT)
					deactivate_output(i);
			break;
		case BUS_CMD_DRIVER_DEACTIVATE_RX_ANT_OUTPUT:
			for (unsigned char i=1;i<=12;i++)
				if (driver_output_type[i-1] == BUS_CMD_DRIVER_ACTIVATE_RX_ANT_OUTPUT)
					deactivate_output(i);
			break;
		case BUS_CMD_DRIVER_DEACTIVATE_ALL_BAND_OUTPUTS:
			for (unsigned char i=1;i<=12;i++)
				if (driver_output_type[i-1] == BUS_CMD_DRIVER_ACTIVATE_BAND_OUTPUT)
					deactivate_output(i);
			break;		
		case INT_COMM_AUX_CHANGE_OUTPUT_PIN:
			switch(message.data[0]) {
				case AUX_X11_PIN3: 
					if (message.data[1] == 1)
						PORTA |= (1<<0);
					else
						PORTA &= ~(1<<0);
					
					break;
				case AUX_X11_PIN8: 
					if (message.data[1] == 1)
						PORTA |= (1<<1);
					else
						PORTA &= ~(1<<1);
					
					break;
				case AUX_X11_PIN4:
					if (message.data[1] == 1)
						PORTA |= (1<<2);
					else
						PORTA &= ~(1<<2);
					
					break;
				case AUX_X11_PIN5:
					if (message.data[1] == 1)
						PORTF |= (1<<5);
					else
						PORTF &= ~(1<<5);
					
					break;
				case AUX_X11_PIN9:
					if (message.data[1] == 1)
						PORTF |= (1<<4);
					else
						PORTF &= ~(1<<4);
					break;
			}
			
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
			temp |= (PINF & (1<<1)) << 3;
			
			internal_comm_add_tx_message(INT_COMM_GET_BAND_BCD_STATUS, 1, &temp);
			break;
		case INT_COMM_PULL_THE_PLUG:
			printf("PULLING THE PLUG\n");
			//Will drop the voltage to the input relay
			//
			PORTB &= ~(1<<7);
			break;
		default:
			break;
	}
}

void ps2_keyboard_send(unsigned char cmd) {
	ps2.transmit = 1;
	ps2.bit_count = 0;
	ps2.tx_data = cmd;
	ps2.parity = ~parity_even_bit(cmd);
		
	DDRA |= (1<<3);
	PS2_DATA_HIGH;
	DDRE |= (1<<6);
	PS2_CLK_LOW;
	tiny_delay();
	tiny_delay();
	tiny_delay();
	PS2_DATA_LOW;
	tiny_delay();
	DDRE &= ~(1<<6);
	PS2_CLK_HIGH;
	ps2.bit_count = 0;
}

void ps2_process_key(unsigned char key_code) {
	//TODO: Figure out why this does not work with just sending in one variable
	unsigned char why[2];
	why[0] = key_code;
	internal_comm_add_tx_message(INT_COMM_PS2_KEYPRESSED,1,why);
}

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
	
	//19.2kbaud
	usart1_init(47);
	fdevopen((void*)usart1_transmit, (void*)usart1_receive_loopback);
	
	//115.2kbaud - internal comm
	usart0_init(7);
	
	printf("Motherboard started\n");
	
	sei();

	//Init the values in the ps2 struct
	ps2.started = 0;
	ps2.bit_count = 0;
	ps2.data = 0;
	ps2.data_ready = 0;
	
	delay_ms(200);
	
	PORTA |= (1<<3);
	
	ps2_keyboard_send(0xF4);
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
/*		if ((btn_on_off_last_state == 0) && (PINB & (1<<6))) {
			while ((PINB & (1<<6)));
			delay_s(1);
			internal_comm_add_tx_message(INT_COMM_TURN_DEVICE_OFF,0,0);
			btn_on_off_last_state = 1;
		}
	*/
	}
}

/*!Output compare 0 interrupt - "called" with 1ms intervals*/
ISR(SIG_OUTPUT_COMPARE0) {
	counter_time_start++;
	
	if (counter_time_start > 5000) {
		btn_on_off_last_state = 0;
	}
	
	if (counter_ps2 > 250) {
		ps2.started = 0;
		ps2.bit_count = 0;
		ps2.data = 0;
		
		counter_ps2 = 0;
	}
	
	counter_ps2++;
}

/*! Output overflow 0 interrupt */
ISR(SIG_OVERFLOW0) {
	
}

ISR(SIG_INTERRUPT6) {
	if (!ps2.transmit) {
		if ((PINE & (1<<6)) == 0) {
			if (ps2.started == 0) {
				if ((PINA & (1<<3)) == 0) {
					ps2.started = 1;
					ps2.bit_count = 0;
					ps2.data = 0;
					
					counter_ps2 = 0;
					
					return;
				}
			}
			else if (ps2.bit_count < 8) {
				if (PINA & (1<<3)) {
					ps2.data |= (1<<ps2.bit_count);
				}
				ps2.bit_count++;
				return;
			}
			else if (ps2.bit_count == 8) { //Parity bit, not implemented
				ps2.bit_count++;
				
				return;
			}
			else {
				ps2.started = 0;
				
				if (ps2.prev_cmd == 0xF0) {
					ps2_process_key(ps2.data);
				}
				
				ps2.prev_cmd = ps2.data;
				
				return;
			}
		}
	}
	else {	//Transmission of data
		if (PINE & (1<<6)) {
			if (ps2.bit_count == 0) {
				PS2_DATA_LOW;
			} else if (ps2.bit_count == 9) {
				if (ps2.parity)
					PS2_DATA_HIGH;
				else
					PS2_DATA_LOW;
			}
			else if (ps2.bit_count == 10) {
				DDRA &= ~(1<<3);
				PS2_DATA_HIGH;
			}
			else {
				temp_count++;
				if (ps2.tx_data & (1<<(ps2.bit_count-1)))
					PS2_DATA_HIGH;
				else
					PS2_DATA_LOW;
			}
			
			ps2.bit_count++;
		}
		else {
			if (ps2.bit_count == 11) {
				ps2.tx_data = 0;
				ps2.transmit = 0;

				if ((PINA & (1<<3)) == 0)
					usart1_transmit('A');
				else
					usart1_transmit('N');
			}
		}
	}
}