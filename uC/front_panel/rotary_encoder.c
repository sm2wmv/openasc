/*! \file front_panel/rotary_encoder.c
 *  \brief Rotary encoder functions
 *  \ingroup front_panel_group
 *  \author Mikael Larsmark, SM2WMV
 *  \date 2017-11-08
 *  \code #include "front_panel/rotary_encoder.c" \endcode
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

//Parts of this coden is taken from Ben Buxton, https://github.com/buxtronix/arduino/tree/master/libraries/Rotary

#include <stdio.h>
#include <stdlib.h>
#include <avr/io.h>
#include <string.h>

#include "board.h"
#include "rotary_encoder.h"
#include "main.h"

#define R_START 0x0

// Values returned by 'process'
// No complete step yet.
#define ENCODER_DIR_NONE 0x0
// Clockwise step.
#define ENCODER_DIR_CW 0x10
// Anti-clockwise step.
#define ENCODER_DIR_CCW 0x20

static unsigned char encoder_state = R_START;

#define HALF_STEP

#ifdef HALF_STEP
// Use the half-step state table (emits a code at 00 and 11)
#define R_CCW_BEGIN 0x1
#define R_CW_BEGIN 0x2
#define R_START_M 0x3
#define R_CW_BEGIN_M 0x4
#define R_CCW_BEGIN_M 0x5
const unsigned char ttable[6][4] = {
  // R_START (00)
  {R_START_M,            R_CW_BEGIN,     R_CCW_BEGIN,  R_START},
  // R_CCW_BEGIN
  {R_START_M | ENCODER_DIR_CCW, R_START,        R_CCW_BEGIN,  R_START},
  // R_CW_BEGIN
  {R_START_M | ENCODER_DIR_CW,  R_CW_BEGIN,     R_START,      R_START},
  // R_START_M (11)
  {R_START_M,            R_CCW_BEGIN_M,  R_CW_BEGIN_M, R_START},
  // R_CW_BEGIN_M
  {R_START_M,            R_START_M,      R_CW_BEGIN_M, R_START | ENCODER_DIR_CW},
  // R_CCW_BEGIN_M
  {R_START_M,            R_CCW_BEGIN_M,  R_START_M,    R_START | ENCODER_DIR_CCW},
};
#else
// Use the full-step state table (emits a code at 00 only)
#define R_CW_FINAL 0x1
#define R_CW_BEGIN 0x2
#define R_CW_NEXT 0x3
#define R_CCW_BEGIN 0x4
#define R_CCW_FINAL 0x5
#define R_CCW_NEXT 0x6

const unsigned char ttable[7][4] = {
  // R_START
  {R_START,    R_CW_BEGIN,  R_CCW_BEGIN, R_START},
  // R_CW_FINAL
  {R_CW_NEXT,  R_START,     R_CW_FINAL,  R_START | ENCODER_DIR_CW},
  // R_CW_BEGIN
  {R_CW_NEXT,  R_CW_BEGIN,  R_START,     R_START},
  // R_CW_NEXT
  {R_CW_NEXT,  R_CW_BEGIN,  R_CW_FINAL,  R_START},
  // R_CCW_BEGIN
  {R_CCW_NEXT, R_START,     R_CCW_BEGIN, R_START},
  // R_CCW_FINAL
  {R_CCW_NEXT, R_CCW_FINAL, R_START,     R_START | ENCODER_DIR_CCW},
  // R_CCW_NEXT
  {R_CCW_NEXT, R_CCW_FINAL, R_CCW_BEGIN, R_START},
};
#endif

/*! \brief Poll the rotary encoder 
 *  \return Returns 0 if nothing happened, -1 if rotary CCW and 1 if CW */
int rotary_encoder_poll(void) {
	int retval = 0;
  unsigned char pinstate = 0;
  
// Grab state of input pins.
  if ((PINE & (1<<PULSE_SENSOR_BIT1)) != 0)
    pinstate = 1;
    
  if ((PINE & (1<<PULSE_SENSOR_BIT2)) != 0)
    pinstate |= 2;  

  // Determine new state from the pins and state table.
  encoder_state = ttable[encoder_state & 0xf][pinstate];
  
  // Return emit bits, ie the generated event.
  if ((encoder_state & 0x30) == ENCODER_DIR_CW)
    retval = 1;
  else if ((encoder_state & 0x30) == ENCODER_DIR_CCW)
    retval = -1;
    
	return(retval);
}