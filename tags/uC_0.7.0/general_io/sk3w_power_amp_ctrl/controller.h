/*! \file     general_io/sk3w_power_amp_ctrl/controller.h
 *  \ingroup  general_io_group
 *  \brief    Handling of controllers (boxes)
 *  \author   Tobias Blomberg, SM0SVX
 *  \date     2012-05-23
 * 
 * Contains functions for handling controllers and actions performed by
 * controllers.
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

#ifndef _CONTROLLER_H_
#define _CONTROLLER_H_


/*! The data associated with each controller */
typedef struct {
  uint8_t addr;
  uint8_t band;
} Controller;


/**
 * \brief Initialize the controller handler
 */
void controller_init(void);

/**
 * \brief   Set the band to use for this controller
 * \param   ctrlr   The controller
 * \param   fm_addr The bus address of the controller device
 * \param   band    The band to use (e.g. BAND_160M)
 * \returns Returns 0 on success or -1 on error
 */
int8_t controller_set_band(uint8_t ctrlr, uint8_t fm_addr, uint8_t band);

/**
 * \brief   Get the band associated with this controller
 * \param   ctrlr   The controller
 * \returns Returns the band in use or BAND_UNDEFINED if not in use
 */
uint8_t controller_band(uint8_t cntrlr);

/**
 * \brief   Get a list mapping controllers to bands
 * \param   band_list Will be assigned a list of Controller objects
 * \returns Returns the number of controllers in the list
 */
uint8_t controller_band_list(const Controller **band_list);

/**
 * \brief   Toggle the main power on the PA associated with this controller
 * \param   ctrlr   The controller
 * \returns Returns 0 on success or -1 on error
 */
int8_t controller_toggle_mains(uint8_t ctrlr);

/**
 * \brief   Set tx_active for the PA associated with the given controller
 * \param   ctrlr   The controller
 * \param   is_active Set to 1 if active or 0 if it's not
 */
void controller_set_tx_active(uint8_t ctrlr, int8_t is_active);


#endif /* _CONTROLLER_H_ */
