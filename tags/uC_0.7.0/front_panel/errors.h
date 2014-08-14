/*! \file front_panel/errors.h 
 *  \brief List of error codes
 *  \ingroup front_panel_group
 *  \author Mikael Larsmark, SM2WMV
 *  \date 2010-12-04
 *  \code #include "front_panel/errors.h" \endcode
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

#ifndef _ERRORS_H_
#define _ERRORS_H_

//! Define which tells us how many different error types that currently exist
#define NR_OF_ERRORS	15

//! Error that the bus had to resend a message more times than the max limit
#define ERROR_TYPE_BUS_RESEND						0
//! Error that no sync was recieved within the default time frame
#define ERROR_TYPE_BUS_SYNC							1
//! The TX queue of the bus has gotten full
#define ERROR_TYPE_BUS_TX_QUEUE_FULL		2
//! The RX queue of the bus has gotten full
#define ERROR_TYPE_BUS_RX_QUEUE_FULL		3
//! Internal communication resend fail
#define ERROR_TYPE_INT_COMM_RESEND			4
//! Ping timeout error, which means that a device, which is used by THIS openASC box has stopped to respond
//! However not all type of cards generate these errors. This is mainly an error for the driver cards which are
//! very critical for the whole system
#define ERROR_TYPE_ANT_PING_TIMEOUT			5
//! Ping timeout error, which means that a device, which is used by THIS openASC box has stopped to respond
//! However not all type of cards generate these errors. This is mainly an error for the driver cards which are
//! very critical for the whole system
#define ERROR_TYPE_BAND_PING_TIMEOUT		6
//! VSWR error
#define ERROR_TYPE_HIGH_VSWR						7
//! Band is already in use
#define ERROR_TYPE_BAND_IN_USE          8
//! Internal communication TX queue full   
#define ERROR_TYPE_INT_COMM_TX_FULL     9
//! Internal communication RX queue full   
#define ERROR_TYPE_INT_COMM_RX_FULL     10
//! Event queue is full
#define ERROR_TYPE_EVENT_QUEUE_FULL     11
//! HIGH REFLECTED POWER
#define ERROR_TYPE_HIGH_REF_POWER       12
//! PA Error
#define ERROR_TYPE_PA_ERROR             13
//! Rotator Stuck or turning in the wrong direction etc
#define ERROR_TYPE_ROTATOR_ERROR        14


/*! \brief Set the error flag of a certain error type
 *  \param error_type The type of error that has occured, defines can be found in errors.h
 *  \param state State of the error 
 *  \param data Some additional information of the error */
void error_handler_set(unsigned char error_type, unsigned char state, unsigned int data);

/*! \brief Clear all the errors
 *  \return The number of errors that were cleared */
unsigned char error_handler_clear_all(void);

/*! \brief Clear an error of a certain type
 *  \param error_type The type of error that has occured, defines can be found in errors.h */
void error_handler_clear(unsigned char error_type);

/*! \brief Clear an error of a certain type
 *  \return 1 if the ptt is locked, 0 if not */
unsigned char error_handler_is_ptt_locked(void);

unsigned char error_handler_get_state(unsigned char error_type);

unsigned int error_handler_get_errors(void);

#endif