/*! \file motherboard/main.c 
 *  \ingroup  motherboard_group
 *  \brief Main file of the motherboard
 *  \author Mikael Larsmark, SM2WMV
 *  \date 2010-01-25
 *  \code #include "motherboard/main.c" \endcode
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
#include "ps2kbd.h"

#include "../delay.h"
#include "../internal_comm.h"
#include "../internal_comm_commands.h"
#include "../wmv_bus/bus_commands.h"
#include "../comm_interface.h"
#include "../remote_commands.h"

#include "remote_ctrl.h"

//! Macro to put PS2 CLK output LOW
#define PS2_CLK_LOW		PORTE &= ~(1<<6)
//! Macro to put PS2 CLK output HIGH
#define PS2_CLK_HIGH	PORTE |= (1<<6)
//! Macro to put PS2 DATA output LOW
#define PS2_DATA_LOW	PORTA &= ~(1<<3)
//! Macro to put PS2 DATA output HIGH
#define PS2_DATA_HIGH	PORTA |= (1<<3)

//#define PS2_DEBUG 1

static unsigned char resend_count = 0;

driver_status_struct driver_status;

//! Counter used for the PS/2 decoding
unsigned char temp_count = 0;

//! PS2 send counter
unsigned int ps2_send_counter = 0;

//! The driver output state
unsigned int driver_output_state = 0;
//! The type of driver output
unsigned int driver_output_type[12];

//! Variable used to keep track of the last state of the ON/OFF button so we can see when it has been pressed and released
unsigned char btn_on_off_last_state = 1;

/*! Counter which keeps track of how long time ago it was since we started the box. This is used at startup so that 
		we ignore button actions on the ON/OFF button for a certain time at startup */
unsigned int counter_time_start=0;

unsigned long long global_bcd_count = 0;

//! Counter which keeps track of the PS/2 decoding
unsigned int counter_ps2=0;

//! PS/2 struct
PS2_STRUCT ps2;

//! \brief Tiny delay function
void __inline__ tiny_delay(void) {
	for (volatile unsigned char i=0;i<50;i++)
		for (volatile unsigned char j=0;j<255;j++)
			__asm__ __volatile__("nop");
}

/*! \brief Activate a driver output
* This function is used to activate an output on the relay driver output in the openASC
* It controls both the sink and source output at the same time.
* \param from_addr Dummy byte to make it compatible with the driver units
* \param index The index of which output to activate (1-12)
* \param type The command that did activate this output
*/
void activate_output(unsigned char from_addr, unsigned char index, unsigned char type) {
	driver_status.driver_output_state |= (1<<(index-1));
	driver_status.driver_output_owner[index-1] = from_addr;

	driver_status.driver_output_type[index-1] = type;

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
* \param from_addr Dummy byte to make it compatible with the driver units
* \param index The index of which output to activate (1-12)
*/
void deactivate_output(unsigned char from_addr, unsigned char index) {
	driver_status.driver_output_state &= ~(1<<(index-1));
	driver_status.driver_output_owner[index-1] = 0;
	driver_status.driver_output_type[index-1] = 0;

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

/*! \brief Parse an internal communication message 
 *  \param message The message that we wish to parse */
void parse_internal_comm_message(struct_comm_interface_msg message) {
	char temp=0;
	unsigned char from_addr = 0;
	
	#ifdef INT_COMM_DEBUG	
		printf("P: 0x%02X\n",message.cmd);	
		
		for (int i=0;i<message.length;i++)
			printf("[%i]: %i\n",i,message.data[i]);
	#endif
		
  //0xA0 <-> 0xAF are reseverd for remote control commands
  if ((message.cmd >= 0xA0) && (message.cmd <= 0xAF)) {
    remote_ctrl_parse_message(message);
  }
  else {
    switch(message.cmd) {
      case INT_COMM_REDIRECT_DATA:
        break;
      case BUS_CMD_DRIVER_ACTIVATE_ANT_OUTPUT:
        for (unsigned char i=0;i<message.length;i++)
          activate_output(from_addr,message.data[i], BUS_CMD_DRIVER_ACTIVATE_ANT_OUTPUT);
        break;
      case BUS_CMD_DRIVER_DEACTIVATE_ANT_OUTPUT:
        for (unsigned char i=0;i<message.length;i++)
          deactivate_output(from_addr,message.data[i]);
        break;
      case BUS_CMD_DRIVER_ACTIVATE_BAND_OUTPUT:
        for (unsigned char i=0;i<message.length;i++)
          activate_output(from_addr,message.data[i], BUS_CMD_DRIVER_ACTIVATE_BAND_OUTPUT);
        break;
      case BUS_CMD_DRIVER_ACTIVATE_RX_ANT_OUTPUT:
        for (unsigned char i=0;i<message.length;i++)
          activate_output(from_addr,message.data[i], BUS_CMD_DRIVER_ACTIVATE_RX_ANT_OUTPUT);
        break;
      case BUS_CMD_DRIVER_ACTIVATE_RX_BAND_OUTPUT:
        for (unsigned char i=0;i<message.length;i++)
          activate_output(from_addr,message.data[i], BUS_CMD_DRIVER_ACTIVATE_RX_BAND_OUTPUT);
        break;
      case BUS_CMD_DRIVER_GET_STATUS:
    
        break;	
      case BUS_CMD_DRIVER_DEACTIVATE_ALL_OUTPUTS:
        for (unsigned char i=1;i<=20;i++)
          if (driver_status.driver_output_owner[i-1] == from_addr)
            deactivate_output(from_addr,i);
        break;
      case BUS_CMD_DRIVER_DEACTIVATE_ALL_ANT_OUTPUTS:
        for (unsigned char i=1;i<=20;i++)
          if (driver_status.driver_output_type[i-1] == BUS_CMD_DRIVER_ACTIVATE_ANT_OUTPUT)
            deactivate_output(from_addr,i);
        break;
      case BUS_CMD_DRIVER_DEACTIVATE_ALL_RX_ANTENNA_OUTPUTS:
        for (unsigned char i=1;i<=20;i++)
          if (driver_status.driver_output_type[i-1] == BUS_CMD_DRIVER_ACTIVATE_RX_ANT_OUTPUT)
            deactivate_output(from_addr,i);
        break;
      case BUS_CMD_DRIVER_DEACTIVATE_RX_ANT_OUTPUT:
        for (unsigned char i=0;i<message.length;i++)
          deactivate_output(from_addr,message.data[i]);
        break;
      case BUS_CMD_DRIVER_DEACTIVATE_ALL_RX_BAND_OUTPUTS:
        for (unsigned char i=1;i<=20;i++)
          if (driver_status.driver_output_type[i-1] == BUS_CMD_DRIVER_ACTIVATE_RX_BAND_OUTPUT)
            deactivate_output(from_addr,i);
        break;
      case BUS_CMD_DRIVER_DEACTIVATE_RX_BAND_OUTPUT:
        for (unsigned char i=0;i<message.length;i++)
          deactivate_output(from_addr,message.data[i]);
        break;
      case BUS_CMD_DRIVER_DEACTIVATE_ALL_BAND_OUTPUTS:
        for (unsigned char i=1;i<=20;i++)
          if (driver_status.driver_output_type[i-1] == BUS_CMD_DRIVER_ACTIVATE_BAND_OUTPUT)
            deactivate_output(from_addr,i);
        break;
      case BUS_CMD_DRIVER_ACTIVATE_SUBMENU_ANT1_OUTPUT:
        for (unsigned char i=0;i<message.length;i++)
          activate_output(from_addr,message.data[i], BUS_CMD_DRIVER_ACTIVATE_SUBMENU_ANT1_OUTPUT);
        break;
      case BUS_CMD_DRIVER_DEACTIVATE_SUBMENU_ANT1_OUTPUT:
        for (unsigned char i=0;i<message.length;i++)
          deactivate_output(from_addr,message.data[i]);
        break;
      case BUS_CMD_DRIVER_DEACTIVATE_ALL_SUBMENU_ANT1_OUTPUTS:
        for (unsigned char i=1;i<=20;i++)
          if (driver_status.driver_output_type[i-1] == BUS_CMD_DRIVER_ACTIVATE_SUBMENU_ANT1_OUTPUT)
            deactivate_output(from_addr,i);
        break;
      case BUS_CMD_DRIVER_ACTIVATE_SUBMENU_ANT2_OUTPUT:
        for (unsigned char i=0;i<message.length;i++)
          activate_output(from_addr,message.data[i], BUS_CMD_DRIVER_ACTIVATE_SUBMENU_ANT2_OUTPUT);
        break;
      case BUS_CMD_DRIVER_DEACTIVATE_SUBMENU_ANT2_OUTPUT:
        for (unsigned char i=0;i<message.length;i++)
          deactivate_output(from_addr,message.data[i]);
        break;
      case BUS_CMD_DRIVER_DEACTIVATE_ALL_SUBMENU_ANT2_OUTPUTS:
        for (unsigned char i=1;i<=20;i++)
          if (driver_status.driver_output_type[i-1] == BUS_CMD_DRIVER_ACTIVATE_SUBMENU_ANT2_OUTPUT)
            deactivate_output(from_addr,i);
        break;
      case BUS_CMD_DRIVER_ACTIVATE_SUBMENU_ANT3_OUTPUT:
        for (unsigned char i=0;i<message.length;i++)
          activate_output(from_addr,message.data[i], BUS_CMD_DRIVER_ACTIVATE_SUBMENU_ANT3_OUTPUT);
        break;
      case BUS_CMD_DRIVER_DEACTIVATE_SUBMENU_ANT3_OUTPUT:
        for (unsigned char i=0;i<message.length;i++)
          deactivate_output(from_addr,message.data[i]);
        break;
      case BUS_CMD_DRIVER_DEACTIVATE_ALL_SUBMENU_ANT3_OUTPUTS:
        for (unsigned char i=1;i<=20;i++)
          if (driver_status.driver_output_type[i-1] == BUS_CMD_DRIVER_ACTIVATE_SUBMENU_ANT3_OUTPUT)
            deactivate_output(from_addr,i);
        break;
      case BUS_CMD_DRIVER_ACTIVATE_SUBMENU_ANT4_OUTPUT:
        for (unsigned char i=0;i<message.length;i++)
          activate_output(from_addr,message.data[i], BUS_CMD_DRIVER_ACTIVATE_SUBMENU_ANT4_OUTPUT);
        break;
      case BUS_CMD_DRIVER_DEACTIVATE_SUBMENU_ANT4_OUTPUT:
        for (unsigned char i=0;i<message.length;i++)
          deactivate_output(from_addr,message.data[i]);
        break;
      case BUS_CMD_DRIVER_DEACTIVATE_ALL_SUBMENU_ANT4_OUTPUTS:
        for (unsigned char i=1;i<=20;i++)
          if (driver_status.driver_output_type[i-1] == BUS_CMD_DRIVER_ACTIVATE_SUBMENU_ANT4_OUTPUT)
            deactivate_output(from_addr,i);
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
        temp |= (PINF & (1<<1)) << 2;
        
        internal_comm_add_tx_message(INT_COMM_GET_BAND_BCD_STATUS, 1, &temp);
        break;
      case INT_COMM_PULL_THE_PLUG:
        //Will drop the voltage to the input relay
        PORTB &= ~(1<<7);
        break;
      case INT_COMM_PC_SEND_TO_ADDR:
        //if (remote_ctrl_get_active_status() == 0)
        //  computer_interface_tx_message(message.length,message.data);
          comm_interface_add_tx_message(message.cmd, message.length, message.data);
        break;
      default:
        break;
    }
  }
}

/*! \brief Send a command to the PS/2 keyboard output/input 
 *  \param cmd The command we wish to send */
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
	DDRE &= ~(1<<6);
	PS2_CLK_HIGH;
	ps2.bit_count = 0;
}

/*! \brief Process a keystroke
 *  \param key_code The key code which was received */
void ps2_process_key(unsigned char key_code) {
	//printf("key_code: %i\n",key_code);
	internal_comm_add_tx_message(INT_COMM_PS2_KEYPRESSED,1,&key_code);
}

void parse_computer_comm_message(struct_comm_interface_msg message) {
  if (message.cmd == COMPUTER_COMM_REMOTE_SET_STATUS) {
    if (message.length > 0) {
      if (message.data[0] == 0)
        remote_ctrl_set_deactive();
      else if (message.data[0] == 1)
        remote_ctrl_set_active();
    }
  }
  else if (message.cmd == COMPUTER_COMM_REMOTE_BUTTON_EVENT) {
    if (message.length > 0)
      internal_comm_add_tx_message(COMPUTER_COMM_REMOTE_BUTTON_EVENT,1,message.data);
  }
  else if (message.cmd == COMPUTER_COMM_REMOTE_CHANGE_BAND) {
    if (message.length > 0)
      internal_comm_add_tx_message(COMPUTER_COMM_REMOTE_CHANGE_BAND,1,message.data);
  }
  else if (message.cmd == COMPUTER_COMM_REMOTE_ROTATE_ANTENNA) {
    if (message.length > 2)
      internal_comm_add_tx_message(COMPUTER_COMM_REMOTE_ROTATE_ANTENNA,3,message.data);
  }
  else if (message.cmd == INT_COMM_PC_SEND_TO_ADDR) {
    if (message.length > 0)
      internal_comm_add_tx_message(INT_COMM_PC_SEND_TO_ADDR, message.length,message.data);
  }
  else if (message.cmd == INT_COMM_PC_CONNECT_TO_ADDR) {
    if (message.length > 0)
      internal_comm_add_tx_message(INT_COMM_PC_CONNECT_TO_ADDR, 1, message.data);
  }
  else if (message.cmd == COMPUTER_COMM_REMOTE_CLEAR_ERRORS) {
    internal_comm_add_tx_message(COMPUTER_COMM_REMOTE_CLEAR_ERRORS,0,0);
  }
  else if (message.cmd == COMPUTER_COMM_SET_SUBMENU_OPTION) {
    if (message.length > 0)
      internal_comm_add_tx_message(COMPUTER_COMM_SET_SUBMENU_OPTION,2,message.data);
  }
}

void main_resend_timeout(void) {
  resend_count++;
  
  if (resend_count > 3) {
    remote_ctrl_set_deactive();
    resend_count = 0;
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
	
  //19.2kbaud - computer comm
  usart1_init(47);
  fdevopen((void*)usart1_transmit, (void*)usart1_receive_loopback);
  
  //Initialize computer communication
  comm_interface_init((void*)parse_computer_comm_message,(void*)usart1_transmit);
  comm_interface_set_resend_ptr((void *)main_resend_timeout);
  
	init_ports();
	init_timer_0();
		
	//57.6kbaud - internal comm
	usart0_init(15);
	
	//printf("Motherboard started\n");
	
	sei();

	//Init the values in the ps2 struct
	/*ps2.started = 0;
	ps2.bit_count = 0;
	ps2.data = 0;
	ps2.data_ready = 0;*/
	
  kbd_init();
  
	delay_ms(200);
	
	//PORTA |= (1<<3);

	//ps2_keyboard_send(0xF4);
	delay_ms(100);
	
  unsigned char c = 0;
  
/*  kbd_send(0xF4);
  kbd_send(0xFF);
  kbd_send(0xED);*/

  unsigned char temp_var = 1;
  
  while(1) {
    //Is remote ctrl mode active? Then check if anything should be updated
    if (remote_ctrl_get_active_status())
      remote_ctrl_update_info();
		
		//Poll the RX queue in the internal comm to see if we have any new messages to be PARSED
		internal_comm_poll_rx_queue();
		
		//Poll the TX queue in the internal comm to see if we have any new messages to be SENT
    internal_comm_poll_tx_queue();
    
    //Poll the RX queue in the computer comm to see if we have any new messages to be PARSED
    comm_interface_poll_rx_queue();
    
    //Poll the TX queue in the computer comm to see if we have any new messages to be SENT
    comm_interface_poll_tx_queue();
		
/*    if (c = kbd_getchar()) {
      printf("CHAR: %c\n\r",c);
    }*/
    
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
	
	if (counter_time_start > 5000) {
		btn_on_off_last_state = 0;
	}
	
/*	if (counter_ps2 > 250) {
		ps2.started = 0;
		ps2.bit_count = 0;
		ps2.data = 0;
		
		counter_ps2 = 0;
	}
	
	computer_comm_1ms_timer();
	
	//So that if the keyboard has been unplugged and plugged in again it will start working
	//after maximum time of one minute
	if (ps2_send_counter > 60000) {
		ps2_keyboard_send(0xF4);

		ps2_send_counter = 0;
	}
	
	ps2_send_counter++;
	counter_ps2++;*/
	
  comm_interface_1ms_tick();
	internal_comm_1ms_timer();
}

/*! \brief Output overflow 0 interrupt */
ISR(SIG_OVERFLOW0) {
	
}

/*ISR(SIG_INTERRUPT6) {
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
			}
		}
	}
}*/