/*! \file front_panel/sequencer.h
 *  \brief Sequencer
 *  \ingroup front_panel_group
 *  \author Mikael Larsmark, SM2WMV
 *  \date 2010-01-25
 *  \code #include "front_panel/sequencer.h" \endcode
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

#ifndef _SEQUENCER_H_
#define _SEQUENCER_H_

//! Flag bit offset for the footswitch
#define PTT_INPUT_FOOTSWITCH						0
//! Flag bit offset for the radio sense on the upper floor
#define PTT_INPUT_RADIO_SENSE_UP				1
//! Flag bit offset for the radio sense on the lower floor
#define PTT_INPUT_RADIO_SENSE_LO				2
//! Flag bit offset for the COMPUTER RTS signal
#define PTT_INPUT_COMPUTER_RTS					3
//! Flag bit offset for an inverted radio sense signal
#define PTT_INPUT_INVERTED_RADIO_SENSE	4
//! Flag bit offset for an inverted computer rts signal
#define PTT_INPUT_INVERTED_COMPUTER_RTS	5
//! Flag bit offset for the inhibit polarity
#define PTT_INPUT_INHIBIT_POLARITY			6

//! Sequencer enabled for RADIO
#define SEQUENCER_PTT_RADIO_ENABLED		0
//! Sequencer enabled for AMP
#define SEQUENCER_PTT_AMP_ENABLED			1
//! Sequencer enabled for INHIBIT
#define SEQUENCER_PTT_INHIBIT_ENABLED	2

//! \brief The footswitch PTT input is active
#define PTT_ACTIVE_FOOTSWITCH     0
//! \brief The radio sense PTT input is active
#define PTT_ACTIVE_RADIO_SENSE    1
//! \brief The computer PTT input is active
#define PTT_ACTIVE_COMPUTER_RTS   2


/* 
 *	Input
 *	-----
 *	Footswitch
 *	Radio sense lower floor
 *  Radio sense upper floor
 *  Computer RTS
 *  Inverted radio sense
 *  Inverted Computer RTS
 */

//! All the delays are divided with 10 so 100 is really 1000 ms which makes the maximium delay 2550 ms.
typedef struct {
	//! The delay before the radio is PTTed after the input PTT has been activated
	unsigned char radio_pre_delay;	
	//! The delay after the input PTT has been released and the radio PTT is released
	unsigned char radio_post_delay;	
	//! The delay before the amp is PTTed after the input PTT has been activated
	unsigned char amp_pre_delay;
	//! The delay after the input PTT has been released and the amp PTT is released
	unsigned char amp_post_delay;
	//! The delay before the inhibit is activated after the input PTT has been activated
	unsigned char inhibit_pre_delay;
	//! The delay after the input PTT has been released and the inhibit pin is released
	unsigned char inhibit_post_delay;
	//! The delay after theinput PTT has been released and the antennas are switched
	unsigned char antennas_post_delay;
	//! Flags on which sequencer variables that are enabled
	unsigned char active;		//Bit 0 = PTT Radio
													//Bit 1 = PTT Amp
													//Bit 2 = Inhibit enabled
} struct_ptt_sequencer;

//! PTT Sequencer struct
typedef struct {
	//! The size of this structure in bytes
	unsigned int struct_size;
	//! The PTT SEQUENCER for the computer input
	struct_ptt_sequencer computer;		
	//! The PTT SEQUENCER for the footswitch input
	struct_ptt_sequencer footswitch;	
	//! The PTT SEQUENCER for the radio sense input
	struct_ptt_sequencer radio_sense;	
	/*! Bit 0 = Footswitch \n
			Bit 1 = Radio sense lower floor \n
			Bit 2 = Radio sense upper floor \n
			Bit 3 = Computer RTS \n
			Bit 4 = Inverted radio sense \n
			Bit 5 = Inverted Computer RTS  \n
			Bit 6 = Inhibit polarity (0=active low, 1=active high) \n
	*/
	unsigned char ptt_input;
} struct_ptt;

unsigned char sequencer_get_ptt_active(void);

void sequencer_load_eeprom(void);

void sequencer_footsw_pressed(void);
void sequencer_footsw_released(void);

void sequencer_computer_rts_activated(void);
void sequencer_computer_rts_deactivated(void);

void sequencer_radio_sense_activated(void);
void sequencer_radio_sense_deactivated(void);

unsigned char sequencer_get_rts_polarity(void);
unsigned char sequencer_get_sense_polarity(void);

unsigned char sequencer_ptt_active(void);

unsigned char sequencer_get_radio_sense(void);

void sequencer_lock_all(void);

#endif