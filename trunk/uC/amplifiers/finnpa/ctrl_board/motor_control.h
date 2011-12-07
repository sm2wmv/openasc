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

//! The delay between each tick of a stepper motor
#define MOTOR_CONTROL_STEP_DELAY  3

#define MOTOR_DIR_NONE  0
#define MOTOR_DIR_DOWN  1
#define MOTOR_DIR_UP    2

#define MOTOR1_LIMIT_DOWN	100
#define MOTOR2_LIMIT_DOWN	100
#define MOTOR3_LIMIT_DOWN	100

#define MOTOR1_LIMIT_UP 900
#define MOTOR2_LIMIT_UP 900
#define MOTOR3_LIMIT_UP 900

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
void motor_control_set_phase(unsigned char motor_index, unsigned char phase);

void motor_control_step_motor1(void);

#endif