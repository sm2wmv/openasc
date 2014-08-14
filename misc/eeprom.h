/*! \file     general_io/eeprom.h
 *  \ingroup  general_io_group
 *  \brief    EEPROM config storage routines
 *  \author   Tobias Blomberg, SM0SVX
 *  \date     2012-06-06
 *
 * This file contains functions to handle reading and writing configuration data
 * to/from the AVR internal EEPROM. You should define the file "config.h"
 * locally in your project. This file should contain a typedef for the Config
 * type. For example:
 *
 *   typedef struct {
 *     uint16_t   var1;
 *     int16_t    var2;
 *     const char var3[16];
 *   } Config;
 *
 * The extern variable eeprom_default_cfg must also be defined. The suggested
 * place to do that is in a config.c file. It should look something like this:
 *
 *   const Config PROGMEM eeprom_default_cfg = {
 *     var1: 10,
 *     var2: 20,
 *     var3: "Hello"
 *   } 
 *
 * There is also support for reading and writing arbirary data structures by
 * creating an own context and using the *_data functions.
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

#include <avr/pgmspace.h>

#include "config.h"


//! The EEPROM writing cntext
typedef struct eeprom_context {
  uint16_t eep_addr;            /*!< The EEPROM address to write to next */
  uint16_t eep_crc_addr;        /*!< The EEPROM address of the CRC */
  const uint8_t *ram_ptr;       /*!< The pointer to data RAM to read from */
  size_t data_len;              /*!< The size of the data to write */

  struct eeprom_context *next;  /*!< Next context */
} EepromContext;


//! Config variable container
extern Config cfg;
//! Default config values stored in flash memory
extern const Config PROGMEM eeprom_default_cfg;


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

/**
 * \brief Read custom data from the EEPROM
 * \param ctx The EEPROM context
 * \param data_ptr The pointer to RAM to write data to
 * \param data_len The length of the data block to read
 * \param eep_data_ptr Pointer to EEPROM data
 * \param eep_crc_ptr Pointer to EEPROM CRC
 * \param default_data_ptr Pointer to default data. Must be in PROGMEM.
 *
 * Define an EepromContext for each data area you want to store in the EEPROM.
 * No initialization is needed.
 * This function will read data from the EEPROM area pointed out by the
 * eep_data_ptr parameter. The variable it points to must be defined using the
 * EEMEM attribute. A CRC checksum, pointed out by the eep_crc_ptr, will also
 * be read from the EEPROM. A CRC will also be calculated for the data that has
 * been read. The two checksums will then be compared. If they differ, data
 * will be loaded from the area pointed to by the default_data_ptr parameter.
 * This parameter must be defined in PROGMEM (flash).
 */
void eeprom_read_data(const EepromContext *ctx,
                      void *data_ptr, const size_t data_len,
                      const void * const eep_data_ptr,
                      const void * const eep_crc_ptr,
                      const void * const default_data_ptr);

/**
 * \brief Load default data into RAM
 * \param data_ptr Pointer to the RAM area to write data to
 * \param default_data_ptr Pointer to default data. Must be in PROGMEM.
 * \param data_len The length of the data block.
 *
 * Load default data pointed to by the default_data_ptr parameter, which must
 * be defined in PROGMEM (flash), into the data area pointed to by the
 * data_pointer parameter.
 */
void eeprom_set_default_data(void *data_ptr, const void *default_data_ptr,
                             size_t data_len);

/**
 * \brief Write custom data to the EEPROM
 * \param ctx The EEPROM context
 * \param eep_data_ptr Pointer to the EEPROM data area to write to
 * \param eep_crc_ptr Pointer to the EEPROM area where the CRC should be stored
 * \param data_ptr Pointer to RAM where to read data from
 * \param data_len The length of the data to write
 * 
 * Define an EepromContext for each data area you want to store in the EEPROM.
 * No initialization is needed.
 * This function will write data into the EEPROM area pointed out by the
 * eep_data_ptr parameter. A CRC checksum for the data block will be written
 * to the EEPROM area pointed to by the eep_crc_ptr parameter.
 * The two EEPROM variables must be defined using the EEMEM attribute.
 */
void eeprom_write_data(EepromContext *ctx,
                       void *eep_data_ptr, void *eep_crc_ptr,
                       const void * const data_ptr, size_t data_len);

#endif /* _EEPROM_H_ */
