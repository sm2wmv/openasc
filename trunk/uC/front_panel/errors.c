/*! \file front_panel/errors.c '
 *  \brief Error handler
 *  \ingroup front_panel_group
 *  \author Mikael Larsmark, SM2WMV
 *  \date 2010-12-04
 *  \code #include "front_panel/errors.c" \endcode
 */
//    Copyright (C) 2010  Mikael Larsmark, SM2WMV
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

#include <stdio.h>
#include <stdlib.h>
#include <avr/io.h>
#include <avr/interrupt.h>
#include <string.h>

#include "errors.h"
#include "main.h"
#include "menu.h"
#include "led_control.h"
#include "display_handler.h"
#include "remote_control.h"

//! Flag that the PTT should be locked if this error occur
#define ERROR_FLAG_LOCK_PTT					0
#define ERROR_FLAG_SHOW_ERROR_MENU	1

unsigned char lock_status = 0;

typedef struct {
	unsigned char type;
	unsigned char state;
	unsigned char flags;
	unsigned int data;
} struct_errors;

//! List of the error types
struct_errors error_list[NR_OF_ERRORS] = {
	{ERROR_TYPE_BUS_RESEND,0,(1<<ERROR_FLAG_LOCK_PTT) | (1<<ERROR_FLAG_SHOW_ERROR_MENU),0},
	{ERROR_TYPE_BUS_SYNC,0,(1<<ERROR_FLAG_LOCK_PTT) | (1<<ERROR_FLAG_SHOW_ERROR_MENU),0},
	{ERROR_TYPE_BUS_TX_QUEUE_FULL,0,(1<<ERROR_FLAG_LOCK_PTT) | (1<<ERROR_FLAG_SHOW_ERROR_MENU),0},
	{ERROR_TYPE_BUS_RX_QUEUE_FULL,0,(1<<ERROR_FLAG_LOCK_PTT) | (1<<ERROR_FLAG_SHOW_ERROR_MENU),0},
	{ERROR_TYPE_INT_COMM_RESEND,0,(1<<ERROR_FLAG_LOCK_PTT) | (1<<ERROR_FLAG_SHOW_ERROR_MENU),0},
	{ERROR_TYPE_ANT_PING_TIMEOUT,0,(1<<ERROR_FLAG_LOCK_PTT) | (1<<ERROR_FLAG_SHOW_ERROR_MENU),0},
	{ERROR_TYPE_BAND_PING_TIMEOUT,0,(1<<ERROR_FLAG_LOCK_PTT) | (1<<ERROR_FLAG_SHOW_ERROR_MENU),0},
	{ERROR_TYPE_HIGH_VSWR,0,(1<<ERROR_FLAG_LOCK_PTT) | (1<<ERROR_FLAG_SHOW_ERROR_MENU),0},
  {ERROR_TYPE_BAND_IN_USE,0,(1<<ERROR_FLAG_LOCK_PTT) | (1<<ERROR_FLAG_SHOW_ERROR_MENU),0},
  {ERROR_TYPE_INT_COMM_TX_FULL,0,(1<<ERROR_FLAG_LOCK_PTT) | (1<<ERROR_FLAG_SHOW_ERROR_MENU),0},
  {ERROR_TYPE_INT_COMM_RX_FULL,0,(1<<ERROR_FLAG_LOCK_PTT) | (1<<ERROR_FLAG_SHOW_ERROR_MENU),0},
  {ERROR_TYPE_EVENT_QUEUE_FULL,0,(1<<ERROR_FLAG_LOCK_PTT) | (1<<ERROR_FLAG_SHOW_ERROR_MENU),0},
  {ERROR_TYPE_HIGH_REF_POWER,0,(1<<ERROR_FLAG_LOCK_PTT) | (1<<ERROR_FLAG_SHOW_ERROR_MENU),0},
  {ERROR_TYPE_PA_ERROR,0,(1<<ERROR_FLAG_LOCK_PTT) | (1<<ERROR_FLAG_SHOW_ERROR_MENU),0},
  {ERROR_TYPE_ROTATOR_ERROR,0,(1<<ERROR_FLAG_SHOW_ERROR_MENU),0}
};


void error_handler_set(unsigned char error_type, unsigned char state, unsigned int data) {
	if (state != error_list[error_type].state) { 
    error_list[error_type].state = state;
    error_list[error_type].data = data;
    
    if (error_list[error_type].flags & (1<<ERROR_FLAG_LOCK_PTT)) {
      lock_status = 1;
      
      main_process_lock(1);
      
      main_update_ptt_status();
    }
    
    led_set_error(LED_STATE_ON);
    
    if (error_list[error_type].flags & (1<<ERROR_FLAG_SHOW_ERROR_MENU)) {
      clear_screensaver_timer();

      menu_reset();
      menu_set_pos(MENU_POS_SHOW_ERRORS);
      status.function_status |= (1<<FUNC_STATUS_MENU_ACTIVE);
      
      display_handler_new_view(DISPLAY_HANDLER_VIEW_MENU);
      
      led_set_menu(LED_STATE_ON);
    }
    
    if (remote_control_get_remote_mode())
      remote_control_set_update_band_info();
  }
}

unsigned char error_handler_ptt_check(void) {
	unsigned char temp_status = 0;
	
	for (unsigned char i=0;i<NR_OF_ERRORS;i++)
		if ((error_list[i].state == 1) && (error_list[i].flags & (1<<ERROR_FLAG_LOCK_PTT)))
			temp_status = 1;

	return(temp_status);
}

unsigned char error_handler_clear_all(void) {
	unsigned char errors_cleared = 0;
	
	for (unsigned char i=0;i<NR_OF_ERRORS;i++)
		if (error_list[i].state != 0) {
			errors_cleared++;
			error_list[i].state = 0;
		}
	
	lock_status = 0;
	main_update_ptt_status();
	
	return(errors_cleared);
}

void error_handler_clear(unsigned char error_type) {
  error_list[error_type].state = 0;
	lock_status = error_handler_ptt_check();
  if (lock_status == 0) {
    main_update_ptt_status();
  }
  if (error_handler_get_errors() == 0) {
    led_set_error(LED_STATE_OFF);
  }
}

unsigned char error_handler_is_ptt_locked(void) {
	return(lock_status);
}

unsigned char error_handler_get_state(unsigned char error_type) {
	return(error_list[error_type].state);
}

unsigned int error_handler_get_errors(void) {
  unsigned int retval = 0;
  
  for (unsigned char i=0;i<NR_OF_ERRORS;i++)
    if (error_list[i].state)
      retval |= (1<<i);
    
  return(retval);
}