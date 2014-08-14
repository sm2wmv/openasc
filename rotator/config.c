/*! \file     rotator/config.c
 *  \ingroup  rotator_group
 *  \brief    Definition of configuration data to store in the EEPROM
 *  \author   Tobias Blomberg, SM0SVX
 *  \date     2012-12-02
 *
 * This file contains definitions used by the eeprom routines to read and
 * store data.
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

#include <avr/pgmspace.h>

#include "config.h"


/**
 * Default config that is loaded on command or when the CRC checksum mismatch
 * for the configuration data stored in the EEPROM. This variable is external
 * declared in eeprom.c and used by the eeprom routines.
 */
const Config PROGMEM eeprom_default_cfg = {
  rot: {
    {
      ccw_limit: 0,
      ccw_limit_deg: 0,
      cw_limit: 0,
      cw_limit_deg: 0,
      start_wait: 10,
      stop_wait: 30
    },
#if ROTATOR_COUNT > 1
    {
      ccw_limit: 0,
      ccw_limit_deg: 0,
      cw_limit: 0,
      cw_limit_deg: 0,
      start_wait: 10,
      stop_wait: 30
    },
#endif
#if ROTATOR_COUNT > 2
    {
      ccw_limit: 0,
      ccw_limit_deg: 0,
      cw_limit: 0,
      cw_limit_deg: 0,
      start_wait: 10,
      stop_wait: 30
    },
#endif
#if ROTATOR_COUNT > 3
    {
      ccw_limit: 0,
      ccw_limit_deg: 0,
      cw_limit: 0,
      cw_limit_deg: 0,
      start_wait: 10,
      stop_wait: 30
    },
#endif
#if ROTATOR_COUNT > 4
    {
      ccw_limit: 0,
      ccw_limit_deg: 0,
      cw_limit: 0,
      cw_limit_deg: 0,
      start_wait: 10,
      stop_wait: 30
    }
#endif
  }
};
