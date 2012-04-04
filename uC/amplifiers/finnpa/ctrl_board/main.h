/*! \file 4cx1500b/ctrl_board/main.h
 *  \brief Main file of the 4cx1500b amp control board
 *  \defgroup ctrl_board_group 4CX1500B amplifier control board
 *  \author Mikael Larsmark, SM2WMV
 *  \date 2011-02-05
 *  \code #include "4cx1500b/ctrl_board/main.h" \endcode
 */
//    Copyright (C) 2011  Mikael Larsmark, SM2WMV
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

#ifndef _MAIN_H_
#define _MAIN_H_

#define DEBUG

//! The current firmware revision nr
#define FIRMWARE_REV "0.1b\0"

#define MODE_LOCAL    1
#define MODE_REMOTE   0

#define BAND_SEGMENT_LOW  0
#define BAND_SEGMENT_MID  1
#define BAND_SEGMENT_HIGH 2

//! Macro to enable timer 0 interrupt
#define ENABLE_TIMER0_INT() 	TIMSK0 |= (1<<OCIE0A);
//! Macro to disable timer 0 interrupt
#define DISABLE_TIMER0_INT() 	TIMSK0 &= ~(1<<OCIE0A);

//! Flag to indicate that the bus is allowed to transmit
#define BUS_STATUS_ALLOWED_TO_SEND_BIT	0
//! Flag to indicate that a preamble has been found in the bus
#define BUS_STATUS_PREAMBLE_FOUND_BIT		1

#define DEF_NR_DEVICES  25

#define BUS_ADDR 0x10

#define AD_CONV_INTERVAL	500

//!Run the event first in the event queue
#define FLAG_RUN_EVENT_QUEUE      0
//! Flag which indicates that the device has started properly
#define FLAG_DEVICE_STARTED       1

/*! Amplifier status bits */
//! The status of the mains of the amplifier
#define AMP_STATUS_MAINS      0
//! The status of the operate/standby of the amplifier
#define AMP_STATUS_OPR_STBY   1

/*! The amplifier operational status */
/*! The amplifier is in standby */
#define AMP_OP_STATUS_STBY          0
/*! The amplifier is ready */
#define AMP_OP_STATUS_READY         1
/*! An error has occured, amp in standby */
#define AMP_OP_STATUS_ERROR         2
/*! The amplifier is tuning */
#define AMP_OP_STATUS_TUNING        3

void event_add_message(void (*func), unsigned int offset, unsigned char id);
unsigned int get_ad_curr_val(unsigned char ch);
void send_ping(void);

typedef struct {
  /*! 1 = Local or 0 = remote mode */
  unsigned char mode;
  /*! The current band */
  unsigned char curr_band;
  /*! The current segment */
  unsigned char curr_segment;
  /*! The parent address of the openASC unit controlling the amp */
  unsigned parent_addr;
  /*! Amplifier status flags */
  unsigned char amp_flags;
  /*! The amplifier operational status */
  unsigned char amp_op_status;
  /*! The amplifier PTT status */
  unsigned char ptt_status;
} main_struct_status;

typedef struct {
  unsigned int tune_cap_pos[6][3];
  unsigned int load_cap_pos[6][3];
} main_struct_settings;

#endif
