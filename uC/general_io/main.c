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
#include <avr/io.h>
#include <avr/interrupt.h>

#include "main.h"
#include "board.h"
#include "init.h"
#include "../i2c.h"
#include "../delay.h"

/* Include the bus headers */
#include "../wmv_bus/bus.h"
#include "../wmv_bus/bus_ping.h"
#include "../wmv_bus/bus_rx_queue.h"
#include "../wmv_bus/bus_tx_queue.h"
#include "../wmv_bus/bus_commands.h"

#define ADC_CHANNELS		3
#define ADC_INTERVAL		33

//! Contains info of the driver type
unsigned char device_id;

//! Counter to keep track of the numbers of ticks from timer0
unsigned int counter_compare0 = 0;
//! Counter to keep track of the time elapsed since the last sync message was sent
unsigned int counter_sync=0;
//! Counter to keep track of when to send a ping out on the bus
unsigned int counter_ping_interval=0;
//! Counter to keep track of when to poll the ADC
unsigned int counter_adc_interval = 0;
//! Selected ADC channel
static uint8_t adc_ch = 0;

static int current_heading[ADC_CHANNELS] = {0, 0, 0};
static int target_heading[ADC_CHANNELS] = {-1, -1, -1};
static int8_t rotate_dir[ADC_CHANNELS] = {0, 0, 0};


static void init_adc(void) {
    /* ADC right adjust result.   */
    /* AREF as reference voltage */
  ADMUX = 0;
  
    /* ADC interrupt enable */
    /* ADC prescaler division factor 128 */
  ADCSRA = _BV(ADIE) | _BV(ADPS2) | _BV(ADPS1) | _BV(ADPS0);
}


/*! \brief Parse a message and exectute the proper commands
* This function is used to parse a message that was receieved on the bus that is located
* in the RX queue. */
void bus_parse_message(void) {
	BUS_MESSAGE bus_message = rx_queue_get();

	if (bus_message.cmd == BUS_CMD_ACK)
		bus_message_acked(bus_message.from_addr);
	else if (bus_message.cmd == BUS_CMD_NACK)
		bus_message_nacked(bus_message.from_addr, bus_message.data[0]);
	else if (bus_message.cmd == BUS_CMD_ROTATOR_SET_ANGLE) {
		unsigned char subaddr = bus_message.data[0];
		if (subaddr <= 2) {
			unsigned int new_dir;
			new_dir = bus_message.data[1] << 8;
			new_dir |= bus_message.data[2];
			if (new_dir > 1023) {
				new_dir = 1023;
			}
			if (new_dir > current_heading[subaddr]) {
				target_heading[subaddr] = new_dir;
				rotate_dir[subaddr] = 1;
			}
			else if (new_dir < current_heading[subaddr]) {
				target_heading[subaddr] = new_dir;
				rotate_dir[subaddr] = -1;
			}
			else {
				target_heading[subaddr] = -1;
				rotate_dir[subaddr] = 0;
			}
		}
	}
	else {
	
	}
	
	//Drop the message
	rx_queue_drop();
}

/*! \brief Read the external DIP-switch.
* This function is used to read the external offset address on the General I/O card
*	\return The address of the external DIP-switch */
unsigned char read_ext_addr(void) {
	return(~(PINE >> 2) & 0x0F);
}

/*! Main function of the General I/O */
int main(void)
{
	cli();
	
	delay_ms(250);

	init_ports();

	delay_ms(250);

	/* Initialize various hardware resources */

	/* Read the external address of the device */
	bus_set_address(BUS_BASE_ADDR+read_ext_addr());

	/* This delay is simply so that if you have the devices connected to the same power supply
	all units should not send their status messages at the same time. Therefor we insert a delay
	that is based on the external address of the device */
	for (unsigned char i=0;i<bus_get_address();i++)
		delay_ms(DEFAULT_STARTUP_DELAY);

	//Initialize the communication bus	
	bus_init();
	
	if ((BUS_BASE_ADDR+read_ext_addr()) == 0x01)
		bus_set_is_master(1,DEF_NR_DEVICES);
	else
		bus_set_is_master(0,0);

	//Timer used for the communication bus. The interrupt is caught in bus.c
	init_timer_2();	
	
	//Timer with interrupts each ms
	init_timer_0();
	
	init_adc();
	
	device_id = DEVICE_ID_GENERAL_IO;
	
	sei();

	unsigned char device_count = bus_get_device_count();

	while(1) {
		if (!rx_queue_is_empty()) {
			bus_parse_message();
		}

		if (!tx_queue_is_empty())
			bus_check_tx_status();
			
		//If this device should act as master it should send out a SYNC command
		//and also the number of devices (for the time slots) that are active on the bus
		if (bus_is_master()) {
			if (counter_sync >= BUS_MASTER_SYNC_INTERVAL) {
				bus_add_tx_message(bus_get_address(), BUS_BROADCAST_ADDR, 0, BUS_CMD_SYNC, 1, &device_count);
				counter_sync = 0;
			}
		}

		if (bus_allowed_to_send()) {
			//Check if a ping message should be sent out on the bus
			if (counter_ping_interval >= BUS_DEVICE_STATUS_MESSAGE_INTERVAL) {
				//Check if the device is a POS or NEG driver module
				bus_add_tx_message(bus_get_address(), BUS_BROADCAST_ADDR, 0, BUS_CMD_PING, 1, &device_id);
				
				unsigned char data[6];
				data[0] = DEVICE_ID_ROTATOR_UNIT;
				int i;
				for (i=0; i<3; ++i) {
					int target = current_heading[i];
					if (target_heading[i] != -1) {
						target = target_heading[i];
					}
					
					data[1] = i;															/* Sub address */
					data[2] = (current_heading[i] >> 8);	  /* Hi current heading */
					data[3] = (current_heading[i] & 0xff);  /* Lo current heading */
					data[4] = (target >> 8);								/* Hi target heading */
					data[5] = (target & 0xff);							/* Lo target heading */
					bus_add_tx_message(bus_get_address(), BUS_BROADCAST_ADDR, 0,
								BUS_CMD_ROTATOR_STATUS_UPDATE, sizeof(data), data);
				}
				
				counter_ping_interval = 0;
			}
		}
		
		if (counter_adc_interval >= ADC_INTERVAL) {
			counter_adc_interval = 0;
			
				/* Check if the reqested heading has been reached */
			if (target_heading[adc_ch] >= 0) {
				if (((rotate_dir[adc_ch] < 0) && (current_heading[adc_ch] <= target_heading[adc_ch])) ||
						((rotate_dir[adc_ch] > 0) && (current_heading[adc_ch] >= target_heading[adc_ch]))) {
					target_heading[adc_ch] = -1;
					rotate_dir[adc_ch] = 0;
				}
			}
			
				/* Set the relays according to the rotate_dir variable */
			uint8_t portc = PORTC;
			if (rotate_dir[adc_ch] < 0) {
				portc &= ~_BV(4-adc_ch*2);
				portc |= _BV(5-adc_ch*2);
			}
			else if (rotate_dir[adc_ch] > 0) {
				portc &= ~_BV(5-adc_ch*2);
				portc |= _BV(4-adc_ch*2);
			}
			else {
				portc &= ~(_BV(4-adc_ch*2) | _BV(5-adc_ch*2));
			}
			PORTC = portc;
	
				/* Switch to next ADC channel and start a conversion. */
			if (++adc_ch == ADC_CHANNELS) {
				adc_ch = 0;
			}
			ADMUX = (ADMUX & 0xf0) | adc_ch;
			ADCSRA |= (_BV(ADEN) | _BV(ADSC));
		}
	}
	
	return 0;
}


/*! \brief Output compare 0 interrupt - "called" with 1ms intervals*/
ISR(SIG_OUTPUT_COMPARE0) {
	counter_sync++;
	counter_ping_interval++;
	counter_compare0++;
	counter_adc_interval++;
	
	bus_ping_tick();
}



/*! \brief Interrupt service routine for ADC conversion finished */
SIGNAL(SIG_ADC) {
    /* Read the ADC conversion result */
  uint16_t val = ADCL;
	val |= ADCH << 8;
	current_heading[adc_ch] = val;
	
    /* Turn off ADC clock (saves power) */
  ADCSRA &= ~_BV(ADEN);
}

