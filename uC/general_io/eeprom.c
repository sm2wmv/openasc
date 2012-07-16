/*! \file     general_io/eeprom.c
 *  \ingroup  general_io_group
 *  \brief    EEPROM config storage routines
 *  \author   Tobias Blomberg, SM0SVX
 *  \date     2012-06-06
 *
 * This file contains functions to handle reading and writing configuration data
 * to/from the AVR internal EEPROM. You should define the file "config.h"
 * locally in your project. This file should contain a typedef for the Config
 * type and a DEFAULT_CONFIG macro that declares default initialization data
 * for the Config type.
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

/******************************************************************************
 *
 * Standard library includes
 *
 *****************************************************************************/

#include <avr/io.h>
#include <avr/interrupt.h>
#include <avr/eeprom.h>
#include <avr/pgmspace.h>
#include <util/crc16.h>
#include <string.h>


/******************************************************************************
 *
 * Local includes
 *
 *****************************************************************************/

#include "eeprom.h"


/******************************************************************************
 *
 * Macros and type definitions
 *
 *****************************************************************************/

//! Initial value for the CRC calculations
#define CRC_INIT        0xffff;


//! The state for the EEPROM writing process
typedef enum {
  WRITE_STATE_IDLE,     //! No writing active
  WRITE_STATE_DATA,     //! Writing data to the EEPROM
  WRITE_STATE_CRC       //! Writing the CRC to the EEPROM
} WriteState;


/******************************************************************************
 *
 * Global variable declarations
 *
 *****************************************************************************/

//! EEPROM config variable container
static Config EEMEM eep_cfg;

//! EEPROM CRC
static uint16_t EEMEM eep_cfg_crc;

//! Config variable container
Config cfg;

//! The CRC to write
static uint16_t crc;

//! The state of the EEPROM writer
static WriteState write_state = WRITE_STATE_IDLE;

//! Pointer into RAM to read data from when writing to the EEPROM
static const uint8_t *write_ram_ptr;

//! EEPROM context for the config reader/writer
static EepromContext cfg_ctx;

//! The active EEPROM context being written at the moment
static EepromContext *active = NULL;

//! Default config values stored in flash memory
static const Config PROGMEM default_cfg = DEFAULT_CONFIG;


/******************************************************************************
 *
 * Public functions
 *
 *****************************************************************************/

void eeprom_read_config(void) {
  eeprom_read_data(&cfg_ctx,
                   &cfg, sizeof(cfg),
                   &eep_cfg, &eep_cfg_crc,
                   &default_cfg);
}


void eeprom_read_data(const EepromContext *ctx,
                      void *data_ptr, const size_t data_len,
                      const void * const eep_data_ptr,
                      const void * const eep_crc_ptr,
                      const void * const default_data_ptr) {
    /* Save the state of the EEPROM Ready Interrupt and then disable it */
  uint8_t prev_eerie = EECR & _BV(EERIE);
  EECR &= ~_BV(EERIE);

    /* If the current context is being written, we just copy the write buffer
     * to the read buffer. Otherwise we read the data from the EEPROM. */
  if (active == ctx) {
    memcpy(data_ptr, ctx->ram_ptr, data_len);
  }
  else {
    eeprom_read_block(data_ptr, eep_data_ptr, data_len);
    uint16_t calculated_crc = CRC_INIT;
    uint8_t *ptr = (uint8_t *)data_ptr;
    for (int i=0; i<data_len; ++i) {
      calculated_crc = _crc_ccitt_update(calculated_crc, *ptr++);
    }
    uint16_t crc = eeprom_read_word(eep_crc_ptr);
    if (crc != calculated_crc) {
      eeprom_set_default_data(data_ptr, default_data_ptr, data_len);
    }
  }

    /* Restore the previsously saved state for the EEPROM Ready Interrupt */
  EECR |= prev_eerie;
}


void eeprom_write_config(void) {
  eeprom_write_data(&cfg_ctx, &eep_cfg, &eep_cfg_crc, &cfg, sizeof(cfg));
}


void eeprom_write_data(EepromContext *ctx,
                       void *eep_data_ptr, void *eep_crc_ptr,
                       const void * const data_ptr, size_t data_len) {
    /* Disable the EEPROM Ready Interrupt */
  EECR &= ~_BV(EERIE);

    /* Set up the context */
  ctx->eep_addr = (uint16_t)eep_data_ptr;
  ctx->eep_crc_addr = (uint16_t)eep_crc_ptr;
  ctx->ram_ptr = (const uint8_t *)data_ptr;
  ctx->data_len = data_len;

  if (active == NULL) {
      /* If no writer is active, just go */
    active = ctx;
    ctx->next = NULL;
    write_ram_ptr = ctx->ram_ptr;
    write_state = WRITE_STATE_DATA;
  }
  else if (ctx == active) {
      /*  Restart if the active writer is the same context */
    write_ram_ptr = ctx->ram_ptr;
    write_state = WRITE_STATE_DATA;
  }
  else {
      /* There are at least one other active job. Put our job in the queue */
    EepromContext *prev = active;
    EepromContext *it = active->next;
    while (it != NULL) {
      prev = it;
      it = it->next;
    }
    prev->next = ctx;
    ctx->next = NULL;
  }

    /* Caclulate CRC for the data block */
  crc = CRC_INIT;
  const uint8_t *ptr = (const uint8_t *)data_ptr;
  for (int i=0; i<data_len; ++i) {
    crc = _crc_ccitt_update(crc, *ptr++);
  }

    /* Enable the EEPROM Ready Interrupt */
  EECR |= (1 << EERIE);
}


void eeprom_set_default_config(void) {
  eeprom_set_default_data(&cfg, &default_cfg, sizeof(cfg));
}


void eeprom_set_default_data(void *data_ptr, const void *default_data_ptr,
                             size_t data_len) {
  for (int i = 0; i < data_len; ++i) {
    *(uint8_t*)data_ptr++ = pgm_read_byte((uint8_t*)default_data_ptr++);
  }
}


/******************************************************************************
 *
 * Interrupt Service Routines
 *
 *****************************************************************************/

/**
 * \brief Interrupt handler for the EEPROM ready interrupt
 */
ISR(SIG_EEPROM_READY) {
  if (active == NULL) {
    EECR &= ~_BV(EERIE);
    return;
  }
  
    /* Set up EEPROM address */
  EEAR = active->eep_addr++;

    /* Read back data in EEPROM and only write if the new data differs */
  EECR |= _BV(EERE);
  uint8_t data = *write_ram_ptr++;
  if (data != EEDR) {
    EEDR = data;
    EECR |= _BV(EEMWE);
    EECR |= _BV(EEWE);
  }

  switch (write_state) {
    case WRITE_STATE_DATA:
      if (--active->data_len == 0) {
        write_ram_ptr = (uint8_t *)&crc;
        active->eep_addr = active->eep_crc_addr;
        active->data_len = sizeof(crc);
        write_state = WRITE_STATE_CRC;
      }
      break;
    case WRITE_STATE_CRC:
      if (--active->data_len == 0) {
        active = active->next;
        if (active != NULL) {
          write_ram_ptr = active->ram_ptr;
          write_state = WRITE_STATE_DATA;
        }
        else {
          write_state = WRITE_STATE_IDLE;
          EECR &= ~_BV(EERIE);
        }
      }
      break;
    default:
      write_state = WRITE_STATE_IDLE;
      /* Fall through */
    case WRITE_STATE_IDLE:
      active = NULL;
      EECR &= ~_BV(EERIE);
      break;
  }
}
