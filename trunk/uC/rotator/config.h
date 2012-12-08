/*! \file     rotator/config.h
 *  \ingroup  rotator_group
 *  \brief    Definition of configuration data to store in the EEPROM
 *  \author   Tobias Blomberg, SM0SVX
 *  \date     2012-06-06
 *
 * This file contains definitions used by the eeprom routines to read and
 * store data. You normally do not include this file directly. Include the
 * eeprom.h file instead.
 */
//    Copyright (C) 2012  Mikael Larsmark, SM2WMV
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

#ifndef _CONFIG_H_
#define _CONFIG_H_

#include <stdint.h>

#include <bsp.h>


/**
 * The config variables for each rotator
 */
typedef struct {
  int16_t ccw_limit;            /*!< CCW limit as a raw heading value */
  int16_t ccw_limit_deg;        /*!< CCW limit in degrees */
  int16_t cw_limit;             /*!< CW limit as a raw heading value */
  int16_t cw_limit_deg;         /*!< CW limit in degrees */
  uint16_t start_wait;          /*!< Break release to start rotate delay */
  uint16_t stop_wait;           /*!< Stop rotate to break apply delay */
} RotatorConfig;


/**
 * Configuration variables that are persisted in the EEPROM
 */
typedef struct {
  RotatorConfig rot[ROTATOR_COUNT];         /*!< Config for each rotator */
} Config;


#endif /* _CONFIG_H_ */
