/*! \file front_panel/errors.h 
 *  \brief List of error codes
 *  \ingroup front_panel_group
 *  \author Mikael Larsmark, SM2WMV
 *  \date 2010-01-25
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

#define NR_OF_ERRORS	2

//! Error that the bus had to resend a message more times than the max limit
#define ERROR_TYPE_BUS_RESEND				0
//! Error that no sync was recieved within the default time frame
#define ERROR_TYPE_BUS_SYNC					1

#endif