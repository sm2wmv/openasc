/*! \file wmv_bus/bus_commands.h 
 *  \ingroup bus_group
 *  \brief Global commands for the WMV communication bus.
 *  \author Mikael Larsmark, SM2WMV
 *  \date 2010-01-25
 *  \code #include "wmv_bus/bus_commands.h" \endcode
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

#ifndef _BUS_COMMANDS_H_
#define _BUS_COMMANDS_H_

/*! Send an acknowledge */
#define BUS_CMD_ACK	0xFA
/*! Send an NOT acknowledge */
#define BUS_CMD_NACK	0xFB

/*! Transmit the SYNC signal. The SYNC signal contains one variable
 *  which describes the number of devices connected to the bus. */
#define BUS_CMD_SYNC	0x01	
/*! Sends a ping which all devices can use to see what's connected to the bus */
#define BUS_CMD_PING	0x02	

/*! Activate TX/RX mode */
#define BUS_CMD_DRIVER_ACTIVATE_TXRX_MODE							0x14
/*! Deactivate TX/RX mode */
#define BUS_CMD_DRIVER_DEACTIVATE_TXRX_MODE						0x15
/*! Activate a driver combo, type = TX ANT */
#define BUS_CMD_DRIVER_ACTIVATE_TX_ANT_COMBO					0x16
/*! Deactivate a driver combo, type = TX ANT */
#define BUS_CMD_DRIVER_DEACTIVATE_TX_ANT_COMBO				0x17
/*! Activate a driver combo, type = RX ANT combo */
#define BUS_CMD_DRIVER_ACTIVATE_RX_ANT_COMBO					0x18
/*! Deactivate a driver combo, type = RX ANT combo */
#define BUS_CMD_DRIVER_DEACTIVATE_RX_ANT_COMBO				0x19
/*! Activate a driver output, type = ANT */
#define BUS_CMD_DRIVER_ACTIVATE_ANT_OUTPUT						0x20
/*! Deactivate a driver output, type = ANT */
#define BUS_CMD_DRIVER_DEACTIVATE_ANT_OUTPUT					0x21
/*! Activate a driver output, type = BAND */
#define BUS_CMD_DRIVER_ACTIVATE_BAND_OUTPUT						0x22
/*! Deactivate a driver output, type = BAND */
#define BUS_CMD_DRIVER_DEACTIVATE_BAND_OUTPUT					0x23
/*! Activate a driver output, type = RX Antenna */
#define BUS_CMD_DRIVER_ACTIVATE_RX_ANT_OUTPUT					0x24
/*! Deactivate a driver output, type = RX Antenna */
#define BUS_CMD_DRIVER_DEACTIVATE_RX_ANT_OUTPUT				0x25
/*! Deactivate RX BAND output */
#define BUS_CMD_DRIVER_DEACTIVATE_RX_BAND_OUTPUT			0x26
/*! Deactivate ALL RX BAND outputs */
#define BUS_CMD_DRIVER_DEACTIVATE_ALL_RX_BAND_OUTPUTS	0x27
/*! Activate RX BAND output */
#define BUS_CMD_DRIVER_ACTIVATE_RX_BAND_OUTPUT				0x28

/*! Get the driver status */
#define BUS_CMD_DRIVER_GET_STATUS										0x30
/*! Retrieve the temperature */
#define BUS_CMD_GET_TEMPERATURE											0x31
/*! Deactivate all the outputs enabled by this device */
#define BUS_CMD_DRIVER_DEACTIVATE_ALL_OUTPUTS				0x32
/*! Deactivate all the ant outputs enabled by this device */
#define BUS_CMD_DRIVER_DEACTIVATE_ALL_ANT_OUTPUTS		0x33
/*! Deactivate all the band outputs enabled by this device */
#define BUS_CMD_DRIVER_DEACTIVATE_ALL_BAND_OUTPUTS	0x34
/*! Deactivate all the RX ANTENNA outputs */
#define BUS_CMD_DRIVER_DEACTIVATE_ALL_RX_ANTENNA_OUTPUTS	0x35

/*! Set the target rotation direction and start rotation */
#define BUS_CMD_ROTATOR_SET_ANGLE		0x20
/*! Get the current direction */
#define BUS_CMD_ROTATOR_GET_ANGLE		0x21
/*! Get the current direction */
#define BUS_CMD_ROTATOR_GET_STATUS	0x22
/*! Rotate ClockWise */
#define BUS_CMD_ROTATOR_ROTATE_CW		0x23
/*! Rotate CounterClockWise */
#define BUS_CMD_ROTATOR_ROTATE_CCW	0x24
/*! Stop the rotation of the rotator */
#define BUS_CMD_ROTATOR_STOP				0x25	
/*! Transparent command which just redirects the data to the serial port */
#define BUS_CMD_TRANSPARENT					0x29

/*! PowerMeter information */
#define BUS_CMD_POWERMETER_STATUS 		0xA0
/*! PowerMeter calibration command */
#define BUS_CMD_POWERMETER_CALIBRATE	0xA1

#endif
