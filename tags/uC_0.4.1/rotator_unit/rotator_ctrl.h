/*! \file rotator_unit/board.h
 *  \brief Defines for the rotator board
 *  \author Mikael Larsmark, SM2WMV
 *  \date 2009-04-22
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

#ifndef _ROTATOR_CTRL_H_
#define _ROTATOR_CTRL_H_

void rotator_release_break(void);
void rotator_activate_break(void);

void rotator_rotate_cw(void);
void rotator_rotate_ccw(void);
void rotator_stop(void);


void rotator_rotate_to(int heading);


#endif