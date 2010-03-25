#include <stdio.h>
#include <stdlib.h>
#include <avr/io.h>
#include <avr/interrupt.h>

#include "main.h"
#include "board.h"
#include "init.h"
#include "a2d.h"
#include "input.h"
#include "../../delay.h"

/* Include the bus headers */
#include "../../wmv_bus/bus.h"
#include "../../wmv_bus/bus_rx_queue.h"
#include "../../wmv_bus/bus_tx_queue.h"
#include "../../wmv_bus/bus_commands.h"


//! Counter to keep track of the numbers of ticks from timer0
unsigned int counter_compare0 = 0;
//! Counter to keep track of when to send a ping out on the bus
unsigned int counter_ping_interval=0;

unsigned char update_status = 0;

/*! \brief Parse a message and exectute the proper commands
 * This function is used to parse a message that was receieved on the bus that is located
 * in the RX queue. */
void bus_parse_message(void) {
	BUS_MESSAGE bus_message = rx_queue_get();

	if (bus_message.cmd == BUS_CMD_ACK)
		bus_message_acked();
	else if (bus_message.cmd == BUS_CMD_NACK)
		bus_message_nacked();
	else if (bus_message.cmd == BUS_CMD_PING) {
		
	}
	else {
		if (bus_message.cmd == BUS_CMD_POWERMETER_CALIBRATE) {
			//TODO: The calibration command
		}
		else if (bus_message.cmd == BUS_CMD_SYNC) {
		
		}
	}
	
	//Drop the message
	rx_queue_drop();
}

/*! \brief Read the external DIP-switch.
 * This function is used to read the external offset address on the powermeter pickup
 *	\return The address of the external DIP-switch */
unsigned char read_ext_addr(void) {
	return(~(PINC >> 2) & 0x0F);
}

void read_state() {
	//Read the forward A/D value
	status.curr_fwd_ad_value = a2dConvert10bit(ADC_CH_ADC0);
		//Read the reflected A/D value
	status.curr_ref_ad_value = a2dConvert10bit(ADC_CH_ADC1);
		
	input_calculate_power();
	status.curr_vswr = input_calculate_vswr();
}

int main(void) {
	cli();
	
	init_ports();
	
	delay_ms(100);
	
	//Read the current configuration
	current_coupler.pickup_type = (~(PIND >>3) & 0x07);
	
	/* Read the external address of the device */
	bus_set_address(BUS_BASE_ADDR+read_ext_addr());

	/* This delay is simply so that if you have the devices connected to the same power supply
	all units should not send their status messages at the same time. Therefor we insert a delay
	that is based on the external address of the device */
	delay_ms(7 * bus_get_address());
	
	a2dInit();
	
		//Initialize the communication bus	
	bus_init();
	
	//The device should not be master
	bus_set_is_master(0,0);
	
	//Timer used for the communication bus. The interrupt is caught in bus.c
	init_timer_2();	
	
	//Timer with interrupts each ms
	init_timer_0();	
	
	a2dSetPrescaler(ADC_PRESCALE_DIV16);
	a2dSetReference(ADC_REFERENCE_AREF);
	
	current_coupler.fwd_scale_value[0] = 3782;
	current_coupler.ref_scale_value[0] = 3782;
	
	sei();
	
	while(1) {
		if (!rx_queue_is_empty())
			bus_parse_message();

		if (!tx_queue_is_empty())
			bus_check_tx_status();
		
		/* If the update_status flag is set, we should sample the current power values
		   and send them over the bus as a broadcast message */
		if (update_status == 1) {
			read_state();
			
			unsigned char data[7];
			data[0] = current_coupler.pickup_type;
			data[1] = (status.curr_fwd_power >> 8) & 0xFF;
			data[2] = (status.curr_fwd_power & 0xFF);
			data[3] = (status.curr_ref_power >> 8) & 0xFF;
			data[4] = (status.curr_ref_power & 0xFF);
			
			unsigned int temp_vswr = (unsigned int)(status.curr_vswr * 100);
					
			data[5] = (temp_vswr >> 8) & 0xFF;
			data[6] = (temp_vswr & 0xFF);
			
			bus_add_tx_message(bus_get_address(), BUS_BROADCAST_ADDR, 0, BUS_CMD_POWERMETER_STATUS, 7, data[0]);
			
			update_status = 0;
		}
	}
}

/*!Output compare 0 interrupt - "called" with 1ms intervals*/
ISR(SIG_OUTPUT_COMPARE0) {
	if (bus_allowed_to_send()) {
		//Check if a ping message should be sent out on the bus
		if (counter_ping_interval >= BUS_DEVICE_STATUS_MESSAGE_INTERVAL) {
			bus_add_tx_message(bus_get_address(), BUS_BROADCAST_ADDR, 0, BUS_CMD_PING, 0, 0);
			counter_ping_interval = 0;
		}
	}
	
	//Will happen each 10 ms
	if ((counter_compare0 % 10) == 0) {
		update_status = 1;
	}

	counter_ping_interval++;
	counter_compare0++;
}