/*! \file     general_io/sk3w_power_amp_ctrl/config.h
 *  \ingroup  general_io_group
 *  \brief    Definition of configuration data to store in the EEPROM
 *  \author   Tobias Blomberg, SN0SVX
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


/**
 * Default config that is loaded on command or when the CRC checksum mismatch
 * for the configuration data stored in the EEPROM.
 */
#define DEFAULT_CONFIG { \
  rot: { \
    { \
      ccw_limit: 0, \
      ccw_limit_deg: 0, \
      cw_limit: 0, \
      cw_limit_deg: 0 \
    }, \
    { \
      ccw_limit: 0, \
      ccw_limit_deg: 0, \
      cw_limit: 0, \
      cw_limit_deg: 0 \
    }, \
    { \
      ccw_limit: 0, \
      ccw_limit_deg: 0, \
      cw_limit: 0, \
      cw_limit_deg: 0 \
    }, \
    { \
      ccw_limit: 0, \
      ccw_limit_deg: 0, \
      cw_limit: 0, \
      cw_limit_deg: 0 \
    }, \
    { \
      ccw_limit: 0, \
      ccw_limit_deg: 0, \
      cw_limit: 0, \
      cw_limit_deg: 0 \
    } \
  } \
}


/**
 * The config variables for each rotator
 */
typedef struct {
  int16_t ccw_limit;            /*!< CCW limit as a raw heading value */
  int16_t ccw_limit_deg;        /*!< CCW limit in degrees */
  int16_t cw_limit;             /*!< CW limit as a raw heading value */
  int16_t cw_limit_deg;         /*!< CW limit in degrees */
} RotatorConfig;


/**
 * Configuration variables that are persisted in the EEPROM
 */
typedef struct {
  RotatorConfig rot[5];         /*!< Config for each rotator */
} Config;


#endif /* _CONFIG_H_ */
