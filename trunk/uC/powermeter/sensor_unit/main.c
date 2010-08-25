#include <stdio.h>
#include <stdlib.h>
#include <avr/io.h>
#include <avr/interrupt.h>
#include <math.h>

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
#include "../../wmv_bus/bus_usart.h"

/* All these flags should be put together in one variable to save space, however 
   this is not needed on this device since it will never run out of RAM anyway */ 

//! Counter to keep track of the numbers of ticks from timer0
unsigned int counter_compare0 = 0;
//! Counter to keep track of when to send a ping out on the bus
unsigned int counter_ping_interval=0;
//! Flag which is set when the power information should be sent out on the bus
unsigned int update_status = 0;
//! Flag which is set when the power should be polled
unsigned char poll_curr_power = 0;
//! Counter for the bus SYNC if the device is acting as master
unsigned int counter_sync = 0;
//! Flag which indicates that the device is put into "sleep" mode, only sending out data
//! at a very much lower rate than during its awake time
unsigned char pwr_meter_sleep = 0;

//! The device ID of the power meter
unsigned char device_id;

/*! \brief Parse a message and exectute the proper commands
 * This function is used to parse a message that was receieved on the bus that is located
 * in the RX queue. */
void bus_parse_message(void) {
	BUS_MESSAGE bus_message = rx_queue_get();

	if (bus_message.cmd == BUS_CMD_ACK)
		bus_message_acked(bus_message.from_addr);
	else if (bus_message.cmd == BUS_CMD_NACK)
		bus_message_nacked(bus_message.from_addr, bus_message.data[0]);
	else if (bus_message.cmd == BUS_CMD_PING) {
		
	}
	else {
		if (bus_message.cmd == BUS_CMD_POWERMETER_CALIBRATE) {
			//TODO: The calibration commands
		}
	}
	
	//Drop the message from the RX queue
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
	
	//Calculate the power from the new A/D reads
	input_calculate_power();
	//Save the calculation of the VSWR to the status struct
	status.curr_vswr = input_calculate_vswr();
}

/*! Read the external configuration jumper settings 
 *  \return The value of the three pins */
unsigned char read_ext_configuration(void) {
	return(~(PIND >> 3) & 0x07);
}

unsigned int last_pwr_change_interval = 5000;

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
	delay_ms(bus_get_address());
	
	a2dInit();
	
		//Initialize the communication bus	
	bus_init();
	
	if (bus_get_address() == 0x01)
		bus_set_is_master(1,DEF_NR_DEVICES);
	else
		bus_set_is_master(0,0);
	
	//Timer used for the communication bus. The interrupt is caught in bus.c
	init_timer_2();
	
	//Timer with interrupts each ms
	init_timer_0();
	
	current_coupler.scale_value[0] = 27.29; //160
	current_coupler.scale_value[1] = 27.93; //80
	current_coupler.scale_value[2] = 27.83; //40
	current_coupler.scale_value[3] = 27.83; //30
	current_coupler.scale_value[4] = 27.47; //20
	current_coupler.scale_value[5] = 27.47; //17
	current_coupler.scale_value[6] = 28.07; //15
	current_coupler.scale_value[7] = 28.07; //12
	current_coupler.scale_value[8] = 28.9; //10
	
	current_coupler.scale_constant[0] = 9.88; //160
	current_coupler.scale_constant[1] = 8.73; //80
	current_coupler.scale_constant[2] = 8.16; //40
	current_coupler.scale_constant[3] = 8.16; //30
	current_coupler.scale_constant[4] = 8.6; //20
	current_coupler.scale_constant[5] = 8.6; //17
	current_coupler.scale_constant[6] = 9.9; //15
	current_coupler.scale_constant[7] = 9.9; //12
	current_coupler.scale_constant[8] = 8.9; //10	
	
	a2dSetPrescaler(ADC_PRESCALE_DIV16);
	a2dSetReference(ADC_REFERENCE_256V);	//Set the 2.56V internal reference
	
	current_coupler.pickup_type = read_ext_configuration();
	
	unsigned char data[7];
	unsigned int temp_vswr;
	
	status.curr_fwd_power = 10;
	
	device_id = DEVICE_ID_POWERMETER_PICKUP;
	
	//TEMP
	fdevopen((void*)bus_usart_transmit, (void*)bus_usart_receive_loopback);
	
	sei();
	
	while(1) {
		if (!rx_queue_is_empty()) {
			bus_parse_message();
		}
		
		if (!tx_queue_is_empty()) {
			bus_check_tx_status();
		}
		
		//If this device should act as master it should send out a SYNC command
		//and also the number of devices (for the time slots) that are active on the bus
		if (bus_is_master()) {
			if (counter_sync >= BUS_MASTER_SYNC_INTERVAL) {
				unsigned char temp = bus_get_device_count();
			
				bus_add_tx_message(bus_get_address(), BUS_BROADCAST_ADDR, 0, BUS_CMD_SYNC, 1, &temp);
				counter_sync = 0;
			}
		}
		
		if (bus_allowed_to_send()) {
		//Check if a ping message should be sent out on the bus
			if (counter_ping_interval >= BUS_DEVICE_STATUS_MESSAGE_INTERVAL) {
				bus_add_tx_message(bus_get_address(), BUS_BROADCAST_ADDR, 0, BUS_CMD_PING, 1, &device_id);
				counter_ping_interval = 0;
			}
		}
		
		
		if (poll_curr_power == 1) {
			read_state();
			
			input_calculate_power();
			input_calculate_vswr();
		
			if (status.curr_fwd_power > 5) {
				last_pwr_change_interval = 0;
				
			if (pwr_meter_sleep == 1)
				pwr_meter_sleep = 0;
			}
			else
				last_pwr_change_interval++;
	
			poll_curr_power = 0;
		}
		
		/* If the update_status flag is set, we should sample the current power values
		   and send them over the bus as a broadcast message */
		if (update_status == 1) {
			data[0] = current_coupler.pickup_type;
			data[1] = ((unsigned int)status.curr_fwd_power >> 8) & 0xFF;
			data[2] = ((unsigned int)status.curr_fwd_power & 0xFF);
			data[3] = ((unsigned int)status.curr_ref_power >> 8) & 0xFF;
			data[4] = ((unsigned int)status.curr_ref_power & 0xFF);
				
			temp_vswr = (unsigned int)(status.curr_vswr * 100);
			
			data[5] = (temp_vswr >> 8) & 0xFF;
			data[6] = (temp_vswr & 0xFF);
			
			bus_add_tx_message(bus_get_address(), BUS_BROADCAST_ADDR, 0, BUS_CMD_POWERMETER_STATUS, 7, data);
			
			update_status = 0;
		}
	}
}

/*!Output compare 0 interrupt - "called" with 1ms intervals*/
ISR(SIG_OUTPUT_COMPARE0) {
	if (pwr_meter_sleep == 0) {
		if ((counter_compare0 % POWER_TRANSMIT_INTERVAL1) == 0) {
			if (update_status == 0)
				update_status = 1;
		}
	}
	else {
		if ((counter_compare0 % POWER_TRANSMIT_INTERVAL2) == 0) {
			if (update_status == 0)
				update_status = 1;
		}
	}
	
	if ((counter_compare0 % POWER_POLL_INTERVAL) == 0) {
		poll_curr_power = 1;
	}

	if (last_pwr_change_interval > (POWER_LAST_CHANGE_TIME/POWER_POLL_INTERVAL)) {
		pwr_meter_sleep = 1;
	}

	counter_sync++;
	counter_ping_interval++;
	counter_compare0++;
}