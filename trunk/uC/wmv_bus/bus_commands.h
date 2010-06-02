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

//Commands which are from 0xC0 to 0xEF are reserved for internal communication commands

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
#define BUS_CMD_DRIVER_ACTIVATE_TXRX_MODE							0x10
/*! Deactivate TX/RX mode */
#define BUS_CMD_DRIVER_DEACTIVATE_TXRX_MODE						0x11
/*! Activate a driver combo, type = TX ANT */
#define BUS_CMD_DRIVER_ACTIVATE_TX_ANT_COMBO					0x12
/*! Deactivate a driver combo, type = TX ANT */
#define BUS_CMD_DRIVER_DEACTIVATE_TX_ANT_COMBO				0x13
/*! Activate a driver combo, type = RX ANT combo */
#define BUS_CMD_DRIVER_ACTIVATE_RX_ANT_COMBO					0x14
/*! Deactivate a driver combo, type = RX ANT combo */
#define BUS_CMD_DRIVER_DEACTIVATE_RX_ANT_COMBO				0x15
/*! Activate a driver output, type = ANT */
#define BUS_CMD_DRIVER_ACTIVATE_ANT_OUTPUT						0x16
/*! Deactivate a driver output, type = ANT */
#define BUS_CMD_DRIVER_DEACTIVATE_ANT_OUTPUT					0x17
/*! Activate a driver output, type = BAND */
#define BUS_CMD_DRIVER_ACTIVATE_BAND_OUTPUT						0x18
/*! Deactivate a driver output, type = BAND */
#define BUS_CMD_DRIVER_DEACTIVATE_BAND_OUTPUT					0x19
/*! Activate a driver output, type = RX Antenna */
#define BUS_CMD_DRIVER_ACTIVATE_RX_ANT_OUTPUT					0x1A
/*! Deactivate a driver output, type = RX Antenna */
#define BUS_CMD_DRIVER_DEACTIVATE_RX_ANT_OUTPUT				0x1B
/*! Deactivate RX BAND output */
#define BUS_CMD_DRIVER_DEACTIVATE_RX_BAND_OUTPUT			0x1C
/*! Deactivate ALL RX BAND outputs */
#define BUS_CMD_DRIVER_DEACTIVATE_ALL_RX_BAND_OUTPUTS	0x1D
/*! Activate RX BAND output */
#define BUS_CMD_DRIVER_ACTIVATE_RX_BAND_OUTPUT				0x1E

/*! Get the driver status */
#define BUS_CMD_DRIVER_GET_STATUS										0x1F
/*! Retrieve the temperature */
#define BUS_CMD_GET_TEMPERATURE											0x20
/*! Deactivate all the outputs enabled by this device */
#define BUS_CMD_DRIVER_DEACTIVATE_ALL_OUTPUTS				0x21
/*! Deactivate all the ant outputs enabled by this device */
#define BUS_CMD_DRIVER_DEACTIVATE_ALL_ANT_OUTPUTS		0x22
/*! Deactivate all the band outputs enabled by this device */
#define BUS_CMD_DRIVER_DEACTIVATE_ALL_BAND_OUTPUTS	0x23
/*! Deactivate all the RX ANTENNA outputs */
#define BUS_CMD_DRIVER_DEACTIVATE_ALL_RX_ANTENNA_OUTPUTS	0x24
/*! Set the PTT settings, which PTT input that corresponds to which device */
#define BUS_CMD_SET_PTT_SETTINGS										0x25
/*! Activate sub menu output */
#define BUS_CMD_DRIVER_ACTIVATE_SUBMENU_ANT1_OUTPUT					0x26
/*! Deactivate sub menu output */
#define BUS_CMD_DRIVER_DEACTIVATE_SUBMENU_ANT1_OUTPUT				0x27
/*! Deactivate all sub menu outputs */
#define BUS_CMD_DRIVER_DEACTIVATE_ALL_SUBMENU_ANT1_OUTPUTS	0x28
/*! Activate sub menu output */
#define BUS_CMD_DRIVER_ACTIVATE_SUBMENU_ANT2_OUTPUT					0x29
/*! Deactivate sub menu output */
#define BUS_CMD_DRIVER_DEACTIVATE_SUBMENU_ANT2_OUTPUT				0x2A
/*! Deactivate all sub menu outputs */
#define BUS_CMD_DRIVER_DEACTIVATE_ALL_SUBMENU_ANT2_OUTPUTS	0x2B
/*! Activate sub menu output */
#define BUS_CMD_DRIVER_ACTIVATE_SUBMENU_ANT3_OUTPUT					0x2C
/*! Deactivate sub menu output */
#define BUS_CMD_DRIVER_DEACTIVATE_SUBMENU_ANT3_OUTPUT				0x2D
/*! Deactivate all sub menu outputs */
#define BUS_CMD_DRIVER_DEACTIVATE_ALL_SUBMENU_ANT3_OUTPUTS	0x2E
/*! Activate sub menu output */
#define BUS_CMD_DRIVER_ACTIVATE_SUBMENU_ANT4_OUTPUT					0x2F
/*! Deactivate sub menu output */
#define BUS_CMD_DRIVER_DEACTIVATE_SUBMENU_ANT4_OUTPUT				0x30
/*! Deactivate all sub menu outputs */
#define BUS_CMD_DRIVER_DEACTIVATE_ALL_SUBMENU_ANT4_OUTPUTS	0x31

/*! Set the target rotation direction and start rotation */
#define BUS_CMD_ROTATOR_SET_ANGLE		0x60
/*! Get the current direction */
#define BUS_CMD_ROTATOR_GET_STATUS	0x61
/*! Rotate ClockWise */
#define BUS_CMD_ROTATOR_ROTATE_CW		0x62
/*! Rotate CounterClockWise */
#define BUS_CMD_ROTATOR_ROTATE_CCW	0x63
/*! Stop the rotation of the rotator */
#define BUS_CMD_ROTATOR_STOP				0x64	
/*! Transparent command which just redirects the data to the serial port */
#define BUS_CMD_ROTATOR_TRANSPARENT	0x65
/*! Rotator unit status message */
#define BUS_CMD_ROTATOR_STATUS_UPDATE	0x66

/*! PowerMeter information */
#define BUS_CMD_POWERMETER_STATUS 		0x70
/*! PowerMeter calibration command */
#define BUS_CMD_POWERMETER_CALIBRATE	0x71

#endif
