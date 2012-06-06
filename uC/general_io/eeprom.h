/*! \file     general_io/eeprom.h
 *  \ingroup  general_io_group
 *  \brief    EEPROM config storage routines
 *  \author   Tobias Blomberg, SN0SVX
 *  \date     2012-06-06
 *
 * This file contains functions to handle reading and writing configuration data
 * to/from the AVR internal EEPROM. You should define the file "config.h"
 * locally in your project. This file should contain a typedef for the Config
 * type and a DEFAULT_CONFIG macro that declares default initialization data
 * for the Config type.
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

#ifndef _EEPROM_H_
#define _EEPROM_H_

#include "config.h"


//! Config variable container
extern Config cfg;


/**
 * \brief Read configuration from EEPROM into the global cfg variable
 */
void eeprom_read_config(void);

/**
 * \brief Write configuration data to the EEPROM from the global cfg variable
 */
void eeprom_write_config(void);

/**
 * \brief Reset all config variables to their default values
 *
 * Call this function to reset all config variables to their default values
 * as specified in the DEFAULT_CONFIG macro. This function will not write the
 * data to the EEPROM, just update the data in RAM. If you want to write it
 * to the EEPROM, just call eeprom_write_config after calling this function.
 */
void eeprom_set_default_config(void);


#endif /* _EEPROM_H_ */
