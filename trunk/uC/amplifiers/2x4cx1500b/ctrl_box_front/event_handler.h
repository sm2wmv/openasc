/*! \file 4cx1500b/ctrl_box_front/event_handler.h
 *  \brief Event handler of various things
 *  \ingroup amp_front_panel_group

 *  \author Mikael Larsmark, SM2WMV
 *  \date 2010-01-25
 *  \code #include "4cx1500b/ctrl_box_front/event_handler.h" \endcode
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

#ifndef _EVENT_HANDLER_H_
#define _EVENT_HANDLER_H_

/* Include the bus headers */
#include "../../wmv_bus/bus.h"
#include "../../wmv_bus/bus_rx_queue.h"
#include "../../wmv_bus/bus_tx_queue.h"
#include "../../wmv_bus/bus_commands.h"

#include "../../internal_comm.h"
#include "../../internal_comm_commands.h"

void event_check_pings(void);

void event_internal_comm_parse_message(UC_MESSAGE message);
void event_pulse_sensor_up(void);
void event_pulse_sensor_down(void);

void event_update_display(void);

void event_poll_buttons(void);
void event_bus_parse_message(void);

#endif