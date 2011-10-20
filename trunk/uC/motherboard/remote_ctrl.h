/*! \file motherboard/remote_ctrl.h
 *  \ingroup motherboard_group
 *  \brief Remote control mode
 *  \author Mikael Larsmark, SM2WMV
 *  \date 2011-09-21
 *  \code #include "computer_interface.h" \endcode */
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

#ifndef _REMOTE_CTRL_H_
#define _REMOTE_CTRL_H_

#include "../global.h"
#include "../internal_comm.h"

typedef struct {
  //! The current band
  unsigned char current_band;
  //! Which antennas are currently selected, bit-wise presentation with bit 0 -> TX ant1, bit 7 -> RX ant4
  unsigned char curr_ant_selected;
  //! The current rx antenna selected, 0 = None, 10 = rx ant nr 10
  unsigned char curr_rx_ant_selected;
  //! Which modes are active, bit 0 -> RX ANT, bit 1 -> TX/RX modes
  unsigned char current_modes;
  //! The RX antenna names
  char rx_antenna_name[10][RX_ANTENNA_NAME_LENGTH];
  //! The antenna names
  char antenna_name[4][ANTENNA_TEXT_SIZE];
  //! The sub menu array name
  char sub_menu_array_name[4][SUB_MENU_ARRAY_NAME_SIZE];
  //! The sub menu stack name
  char sub_menu_stack_name[4][SUB_MENU_STACK_NAME_SIZE];
  //! The current directions of the beams
  unsigned int antenna_curr_direction[4];
  //! Flags
  unsigned int flags;
  //! Error status
  unsigned int error;
  //! Sub menu type
  unsigned char sub_menu_type[4];
  //! The antenna flags
  unsigned char antenna_flags[4];
  //! Antenna rotator flags
  unsigned char antenna_rotator_flags[4];
} struct_band_data;

void remote_ctrl_set_active(void);
void remote_ctrl_set_deactive(void);
unsigned char remote_ctrl_get_active_status(void);

void remote_ctrl_parse_message(UC_MESSAGE message);

void remote_ctrl_update_info(void);

#endif
