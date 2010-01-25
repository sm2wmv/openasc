/*! \file front_panel/radio_interface.h
 *  \brief Radio interface, such as PTT AMP, PTT Radio, CAT etc
 *  \ingroup front_panel_group
 *  \author Mikael Larsmark, SM2WMV
 *  \date 2010-01-25
 *  \code #include "front_panel/radio_interface.h " \endcode
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

#ifndef _RADIO_INTERFACE_H_
#define _RADIO_INTERFACE_H_

//! Flag to indicate that the frequency has changed
#define RADIO_FLAG_FREQ_CHANGED		0

//! Kenwood radio connected to the box
#define RADIO_MODEL_KENWOOD		0
//! ICOM radio connected to the box
#define RADIO_MODEL_ICOM			1
//! FT1000D radio connected to the box
#define RADIO_MODEL_FT1000		2
//! FT1000MP radio connected to the box
#define RADIO_MODEL_FT1000MP	3
//! FT1000MKV radio connected to the box
#define RADIO_MODEL_FT1000MKV	4
//! FT2000 radio connected to the box
#define RADIO_MODEL_FT2000		5

//! MANUAL mode which means no way to interface the radio
#define RADIO_INTERFACE_MANUAL		0
//! Serial interface that connects to the radio, POLLING
#define RADIO_INTERFACE_CAT_POLL	1
//! Serial interface that connects to the radio, MONITORING
#define RADIO_INTERFACE_CAT_MON		2
//! BCD interface that connects the radio
#define RADIO_INTERFACE_BCD				3

//! This bit is set if the radio PTT should be sensed from the upper floor
#define RADIO_SENSE_UPPER_FLOOR	1	
//! This bit is set if the radio PTT should be sensed from the lower floor
#define RADIO_SENSE_LOWER_FLOOR	2	
//! This bit is set if the PTT sense input should be inverted, which means that PTT is active if it's low
#define RADIO_SENSE_INVERTED		3

//! PTT Activate
#define RADIO_PTT_ACTIVE		1
//! PTT Deactivate
#define RADIO_PTT_DEACTIVE	2

//! Radio serial RX buffer length
#define RADIO_SERIAL_RX_BUFFER_LENGTH	50

//! Flag to indicate the radio PTT is active
#define RADIO_FLAG_RADIO_PTT	0
//! Flag to indicate the amp PTT is active
#define RADIO_FLAG_AMP_PTT		1
//! Flag to indicate the box openASC has enabled a transmission (TX ACTIVE output)
#define RADIO_FLAG_TX_ACTIVE	2

//! Serial baudrate 1200 baud
#define RADIO_SERIAL_BAUDRATE_1200	0
//! Serial baudrate 2400 baud
#define RADIO_SERIAL_BAUDRATE_2400	1
//! Serial baudrate 4800 baud
#define RADIO_SERIAL_BAUDRATE_4800	2
//! Serial baudrate 9600 baud
#define RADIO_SERIAL_BAUDRATE_9600	3
//! Serial baudrate 14400 baud
#define RADIO_SERIAL_BAUDRATE_14400	4
//! Serial baudrate 19200 baud
#define RADIO_SERIAL_BAUDRATE_19200	5
//! Serial baudrate 28800 baud
#define RADIO_SERIAL_BAUDRATE_28800	6
//! Serial baudrate 38400 baud
#define RADIO_SERIAL_BAUDRATE_38400	7
//! Serial baudrate 57600 baud
#define RADIO_SERIAL_BAUDRATE_57600	8

//! Radio settings struct
typedef struct {
	//! The size of this structure
	unsigned int struct_size;
	//! Which model of the radio
	unsigned char radio_model;
	//! Which kind of interface to detect band
	unsigned char interface_type;
	//! Baudrate of the radio, used in serial mode
	unsigned char baudrate;
	//! Number of stop bits, used in serial mode
	unsigned char stopbits;
	//! If it's an ICOM, what is the CI-V address to the radio
	unsigned char civ_addr;
	//! What kind of PTT mode, inhibit, static etc
	unsigned char ptt_mode;
	/*! From which input should we monitor the radio PTT?
	 *  Bit 0 = Radio sense lower floor
	 *	Bit 1 = Radio sense upper floor
	 *	Bit 2 = Inverted sense pin	(if this is set the box will sense PTT as LOW)
	 */
	unsigned char ptt_input;
	/*! The interval to poll the band information from the radio, this should be
      set in 10th ms, so for example 100 means 1000ms. */
	unsigned char poll_interval;
} struct_radio_settings;

//! The radio status struct
typedef struct {
	//! The radios current frequency
	unsigned int current_freq;
	//! The radios current band
	unsigned char current_band;
} struct_radio_status;
	
void radio_process_tasks(void);
	
void radio_interface_init(void);
	
unsigned int radio_get_current_freq ( void );
unsigned char radio_get_current_band(void);
void radio_ptt(unsigned char status);

unsigned char radio_get_band_portion(void);

unsigned char radio_poll_status(void);

void radio_ptt_active(void);
void radio_ptt_deactive(void);

void radio_amp_ptt_active(void);
void radio_amp_ptt_deactive(void);

void radio_inhibit_low(void);
void radio_inhibit_high(void);

unsigned char radio_freq_to_band(unsigned int freq);

/* Functions to change settings in the radio setting structure */
void radio_interface_set_model(unsigned char model);
void radio_interface_set_interface(unsigned char interface_type);
void radio_interface_set_baudrate(unsigned char baudrate);
void radio_interface_set_stopbits(unsigned char stopbits);
void radio_interface_set_civ_addr(unsigned char civ);
void radio_interface_set_ptt_mode(unsigned char ptt_mode);
void radio_interface_set_ptt_input(unsigned char ptt_input);
void radio_interface_set_poll_interval(unsigned char poll_interval);

/* Functions to retrieve settings in the radio setting structure */
unsigned char radio_interface_get_model(void);
unsigned char radio_interface_get_interface(void);
unsigned char radio_interface_get_baudrate(void);
unsigned char radio_interface_get_stopbits(void);
unsigned char radio_interface_get_civ_addr(void);
unsigned char radio_interface_get_ptt_mode(void);
unsigned char radio_interface_get_ptt_input(void);
unsigned char radio_interface_get_poll_interval(void);

void radio_interface_save_eeprom(void);
void radio_interface_load_eeprom(void);

void radio_tx_active(void);
void radio_tx_deactive(void);

unsigned char radio_get_ptt_status(void);

#endif
