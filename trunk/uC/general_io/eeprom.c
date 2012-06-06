/*! \file     general_io/eeprom.c
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

#include <avr/io.h>
#include <avr/interrupt.h>
#include <avr/eeprom.h>
#include <avr/pgmspace.h>
#include <util/crc16.h>

#include "eeprom.h"


//! Initial value for the CRC calculations
#define CRC_INIT        0xffff;


//! The state for the EEPROM writing process
typedef enum {
  WRITE_STATE_IDLE,     //! No writing active
  WRITE_STATE_DATA,     //! Writing data to the EEPROM
  WRITE_STATE_CRC       //! Writing the CRC to the EEPROM
} WriteState;


//! EEPROM config variable container
static Config EEMEM eep_cfg;

//! EEPROM CRC
static uint16_t EEMEM eep_crc;

//! Config variable container
Config cfg;

//! The CRC to write
static uint16_t crc;

//! The state of the EEPROM writer
static WriteState write_state = WRITE_STATE_IDLE;

//! Pointer into RAM to read data from when writing to the EEPROM
static uint8_t *write_ram_ptr;

//! EEPROM address to write to
static uint16_t write_eep_addr;

//! Default config values stored in flash memory
static const Config PROGMEM default_cfg = DEFAULT_CONFIG;


void eeprom_read_config(void) {
    /* Wait for a pending config write operation to complete */
  while (write_state != WRITE_STATE_IDLE) ;

  eeprom_read_block(&cfg, &eep_cfg, sizeof(cfg));
  uint16_t calculated_crc = CRC_INIT;
  uint8_t *ptr = (uint8_t *)&cfg;
  for (int i=0; i<sizeof(cfg); ++i) {
    calculated_crc = _crc_ccitt_update(calculated_crc, *ptr++);
  }
  uint16_t crc = eeprom_read_word(&eep_crc);
  if (crc != calculated_crc) {
    eeprom_set_default_config();
  }
}

void eeprom_write_config(void) {
    /* Disable the EEPROM Ready Interrupt */
  EECR &= ~(1 << EERIE);

    /* Set up the pointer to the config struct in RAM */
  write_ram_ptr = (uint8_t *)&cfg;

    /* Set up the address to the position in EEPROM where to start writing */
  write_eep_addr = (uint16_t)&eep_cfg;

    /* Caclulate CRC for config block */
  crc = CRC_INIT;
  uint8_t *ptr = (uint8_t *)&cfg;
  for (int i=0; i<sizeof(cfg); ++i) {
    crc = _crc_ccitt_update(crc, *ptr++);
  }
  
    /* Set the write state to DATA */
  write_state = WRITE_STATE_DATA;
  
    /* Enable the EEPROM Ready Interrupt */
  EECR |= (1 << EERIE);
}

void eeprom_set_default_config(void) {
  uint8_t *pgmptr = (uint8_t *)&default_cfg;
  uint8_t *cfgptr = (uint8_t *)&cfg;
  for (int i = 0; i < sizeof(cfg); ++i) {
    *cfgptr++ = pgm_read_byte(pgmptr++);
  }
}

/**
 * \brief Interrupt handler for the EEPROM ready interrupt
 */
ISR(SIG_EEPROM_READY) {
    /* Set up EEPROM address */
  EEAR = write_eep_addr++;

    /* Read back data in EEPROM and only write if the new data differs */
  EECR |= (1 << EERE);
  uint8_t data = *write_ram_ptr++;
  if (data != EEDR) {
    EEDR = data;
    EECR |= (1 << EEMWE);
    EECR |= (1 << EEWE);
  }

  switch (write_state) {
    case WRITE_STATE_DATA:
      if (write_ram_ptr >= ((uint8_t *)&cfg + sizeof(cfg))) {
        write_ram_ptr = (uint8_t *)&crc;
        write_eep_addr = (uint16_t)&eep_crc;
        write_state = WRITE_STATE_CRC;
      }
      break;
    case WRITE_STATE_CRC:
      if (write_ram_ptr >= ((uint8_t *)&crc + sizeof(crc))) {
        write_state = WRITE_STATE_IDLE;
        EECR &= ~(1 << EERIE);
      }
      break;
    default:
      write_state = WRITE_STATE_IDLE;
      /* Fall through */
    case WRITE_STATE_IDLE:
      EECR &= ~(1 << EERIE);
      break;
  }
}
