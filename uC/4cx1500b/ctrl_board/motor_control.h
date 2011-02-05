/*! \file 4cx1500b/ctrl_board/motor_control.h
 *  \brief Motor control code
 *  \ingroup ctrl_board_group
 *  \author Mikael Larsmark, SM2WMV
 *  \date 2011-02-05
 *  \code #include "4cx1500b/ctrl_board/motor_control.h" \endcode
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

#ifndef _MOTOR_CONTROL_H_
#define _MOTOR_CONTROL_H_

/*
 * Driver 1
 * --------
 * PH PL3
 * D0 PL6 
 * D1 PL4
 * D2 PL2

 * Driver 2
 * --------
 * PH PD4
 * D0 PL5 
 * D1 PL7
 * D2 PD5

 * Driver 3
 * --------
 * PH PB6
 * D0 PG3 
 * D1 PB7
 * D2 PB5
 
 * Driver 4
 * --------
 * PH PL0
 * D0 PH7 
 * D1 PG4
 * D2 PL1
 
 * Driver 5
 * --------
 * PH PH2
 * D0 PH5 
 * D1 PH3
 * D2 PE7 
  
 * Driver 6
 * --------
 * PH PB0
 * D0 PH4 
 * D1 PH6
 * D2 PB4
*/

//! The delay between each tick of a stepper motor
#define MOTOR_CONTROL_STEP_DELAY  50

#define MOTOR_DIR_NONE  0
#define MOTOR_DIR_DOWN  1
#define MOTOR_DIR_UP    2

typedef struct {
  //! The current position of the stepper motor
  unsigned int current_pos;
  //! The new position of the stepper motor
  unsigned int target_pos;
  //! Current ms tick
  unsigned int current_tick;  
  //! Next ms tick
  unsigned int next_tick;  
  //! Which phase the motor is in
  unsigned char current_phase;
  //! Which direction the motor should head
  unsigned char current_dir;
} struct_stepper_motor;

void motor_control_process(void);
void motor_control_tick(void);
void motor_control_goto(unsigned char motor_index, unsigned int pos);

#endif