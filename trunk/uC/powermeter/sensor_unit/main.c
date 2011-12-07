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

unsigned int curr_icp = 270, last_icp = 0, icp_mutex = 0;

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

unsigned int last_pwr_change_interval = 5000;

#define ICP_ARRAY_SIZE			3

unsigned int icp_array[ICP_ARRAY_SIZE];
unsigned char icp_array_pos = 0;

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
		if (bus_message.length > 1)
			bus_ping_new_stamp(bus_message.from_addr, bus_message.data[0], bus_message.length-1, (unsigned char *)(bus_message.data+1));
		else
			bus_ping_new_stamp(bus_message.from_addr, bus_message.data[0], 0, 0);		
	}
	else {
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

void init_calib_values(void) {
	current_coupler.fwd_scale_value[0] = PICKUP_SCALE_FWD_VALUE_160M; //160
	current_coupler.fwd_scale_value[1] = PICKUP_SCALE_FWD_VALUE_80M; //80
	current_coupler.fwd_scale_value[2] = PICKUP_SCALE_FWD_VALUE_40M; //40
	current_coupler.fwd_scale_value[3] = PICKUP_SCALE_FWD_VALUE_20M; //20
	current_coupler.fwd_scale_value[4] = PICKUP_SCALE_FWD_VALUE_15M; //15
	current_coupler.fwd_scale_value[5] = PICKUP_SCALE_FWD_VALUE_10M; //10
	
	current_coupler.fwd_scale_constant[0] = PICKUP_SCALE_FWD_CONSTANT_160M; //160
	current_coupler.fwd_scale_constant[1] = PICKUP_SCALE_FWD_CONSTANT_80M; //80
	current_coupler.fwd_scale_constant[2] = PICKUP_SCALE_FWD_CONSTANT_40M; //40
	current_coupler.fwd_scale_constant[3] = PICKUP_SCALE_FWD_CONSTANT_20M; //20
	current_coupler.fwd_scale_constant[4] = PICKUP_SCALE_FWD_CONSTANT_15M; //15
	current_coupler.fwd_scale_constant[5] = PICKUP_SCALE_FWD_CONSTANT_10M; //10

	current_coupler.ref_scale_value[0] = PICKUP_SCALE_REF_VALUE_160M; //160
	current_coupler.ref_scale_value[1] = PICKUP_SCALE_REF_VALUE_80M; //80
	current_coupler.ref_scale_value[2] = PICKUP_SCALE_REF_VALUE_40M; //40
	current_coupler.ref_scale_value[3] = PICKUP_SCALE_REF_VALUE_20M; //20
	current_coupler.ref_scale_value[4] = PICKUP_SCALE_REF_VALUE_15M; //15
	current_coupler.ref_scale_value[5] = PICKUP_SCALE_REF_VALUE_10M; //10
	
	current_coupler.ref_scale_constant[0] = PICKUP_SCALE_REF_CONSTANT_160M; //160
	current_coupler.ref_scale_constant[1] = PICKUP_SCALE_REF_CONSTANT_80M; //80
	current_coupler.ref_scale_constant[2] = PICKUP_SCALE_REF_CONSTANT_40M; //40
	current_coupler.ref_scale_constant[3] = PICKUP_SCALE_REF_CONSTANT_20M; //20
	current_coupler.ref_scale_constant[4] = PICKUP_SCALE_REF_CONSTANT_15M; //15
	current_coupler.ref_scale_constant[5] = PICKUP_SCALE_REF_CONSTANT_10M; //10
}

unsigned char get_band(unsigned int freq) {
	//Default band configuration would be for 20m
	unsigned char band = status.curr_band;
	
	if ((freq >= 1000) && (freq < 2500))
		band = 0;
	else if ((freq >= 2500) && (freq < 5000))
		band = 1;
	else if ((freq >= 5000) && (freq < 12000))
		band = 2;
	else if ((freq >= 12000) && (freq < 20000))
		band = 3;
	else if ((freq >= 20000) && (freq < 23000))
		band = 4;
	else if (freq >= 23000)
		band = 5;
	
	return(band);
}

unsigned int get_freq(void) {
	unsigned int freq = 0;
	unsigned char state = 1;

	icp_mutex = 1;
	for (unsigned char i=0;i<ICP_ARRAY_SIZE-1;i++) {
		if ((icp_array[i] > (icp_array[i+1]+2)) || (icp_array[i] < (icp_array[i+1]-2))) {
			
			state = 0;
			break;
		}
	}

	if (state == 1) {
		freq = (14746/icp_array[0]) * 256;
	}

	icp_mutex = 0;
	
	return(freq);
}

int main(void) {
	cli();
	
	init_ports();
	
	delay_ms(100);
	
	/* Read the external address of the device */
	bus_set_address(BUS_BASE_ADDR+read_ext_addr());

	/* This delay is simply so that if you have the devices connected to the same power supply
	all units should not send their status messages at the same time. Therefor we insert a delay
	that is based on the external address of the device */
	delay_ms(bus_get_address());
	
	//Default band 20m
	for (unsigned char i=0;i<ICP_ARRAY_SIZE;i++)
		icp_array[i] = 270;
	
	a2dInit();
	
		//Initialize the communication bus	
	bus_init();

	if (bus_get_address() == 0x01)
		bus_set_is_master(1,DEF_NR_DEVICES);
	else
		bus_set_is_master(0,0);
	
	init_timer_1();
	
	//Timer used for the communication bus. The interrupt is caught in bus.c
	init_timer_2();
	
	//Timer with interrupts each ms
	init_timer_0();
	
	a2dSetPrescaler(ADC_PRESCALE_DIV64);
	a2dSetReference(ADC_REFERENCE_256V);	//Set the 2.56V internal reference
	
	//Read the current configuration
	current_coupler.pickup_type = read_ext_configuration();
	
	//Load the calibration values
	init_calib_values();
	
	unsigned char data[7];
	unsigned int temp_vswr;
	
	//status.curr_fwd_power = 10;
	
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
		
		#ifndef CAL_MODE
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
		#endif
		
		if (poll_curr_power == 1) {
			read_state();
			status.curr_freq = get_freq();
			status.curr_band = get_band(status.curr_freq);
		
			input_calculate_power();

			if (status.curr_fwd_power >= NO_FWD_PWR_LIMIT) {
				last_pwr_change_interval = 0;
				
				if (pwr_meter_sleep == 1) {
					update_status	= 1;	//Make sure we send out power info at once, otherwise it might take upto 1s
					pwr_meter_sleep = 0;
				}
			}
			else
				last_pwr_change_interval++;
	
			poll_curr_power = 0;
		}
		
		/* If the update_status flag is set, we should sample the current power values
		   and send them over the bus as a broadcast message */
		if (update_status == 1) {
			#ifndef CAL_MODE
				//Calculate the VSWR, this is not needed during each power update, only
				//just before we send the data
				input_calculate_vswr();
			
				data[0] = current_coupler.pickup_type;
				data[1] = ((unsigned int)status.curr_fwd_power >> 8) & 0xFF;
				data[2] = ((unsigned int)status.curr_fwd_power & 0xFF);
				data[3] = ((unsigned int)status.curr_ref_power >> 8) & 0xFF;
				data[4] = ((unsigned int)status.curr_ref_power & 0xFF);
					
				temp_vswr = (unsigned int)(status.curr_vswr * 100);
				
				data[5] = (temp_vswr >> 8) & 0xFF;
				data[6] = (temp_vswr & 0xFF);
				
				bus_add_tx_message(bus_get_address(), BUS_BROADCAST_ADDR, 0, BUS_CMD_POWERMETER_STATUS, 7, data);
			#endif
			
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

ISR(TIMER1_CAPT_vect) {
	if (icp_mutex == 0) {
		curr_icp = ICR1 - last_icp;
		last_icp = ICR1;
		
		if (icp_array_pos < ICP_ARRAY_SIZE) {
			icp_array[icp_array_pos] = curr_icp;
			icp_array_pos++;
		}
		else {
			icp_array[0] = curr_icp;
			icp_array_pos = 1;
		}
	}
}