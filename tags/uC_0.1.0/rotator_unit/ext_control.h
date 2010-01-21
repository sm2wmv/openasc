/*! \file ext_control.h \brief External control of the rotator unit
 * \author Mikael Larsmark, SM2WMV
 * \date 2009-04-22
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

#ifndef _EXT_CONTROL_H_
#define _EXT_CONTROL_H_

void ext_ctrl_relay1_activate(void);
void ext_ctrl_relay2_activate(void);
void ext_ctrl_relay3_activate(void);
void ext_ctrl_relay4_activate(void);

void ext_ctrl_relay1_deactivate(void);
void ext_ctrl_relay2_deactivate(void);
void ext_ctrl_relay3_deactivate(void);
void ext_ctrl_relay4_deactivate(void);

void ext_ctrl_fet1_activate(void);
void ext_ctrl_fet1_deactivate(void);

void ext_ctrl_fet2_activate(void);
void ext_ctrl_fet2_deactivate(void);

//AUX I/Os
void ext_ctrl_set_io_dir(unsigned char bit, unsigned char dir);
void ext_ctrl_set_io(unsigned char bit, unsigned char state);
unsigned char ext_ctrl_read_io(unsigned char bit);

#endif

