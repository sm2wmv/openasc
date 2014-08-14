/*! \file     general_io/sk3w_power_amp_ctrl/pa.h
 *  \ingroup  general_io_group
 *  \brief    PA state machine base
 *  \author   Tobias Blomberg, SM0SVX
 *  \date     2012-05-23
 * 
 * Implements the base for the PA state machine. The actual state
 * handling is done in the auto generated file pasm.c, which is included at
 * the bottom of this file.
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

#ifndef _PA_H_
#define _PA_H_


//! PA controller is unused indicator
#define PA_CTRLR_UNUSED      255


/**
 * \brief Initialize the PA state machine
 */
void pa_init(void);

/**
 * \brief	Set the warmup timeout time
 * \param	new_timeout	The new timeout value in seconds
 */
void pa_set_warmup_timeout(uint16_t new_timeout);

/**
 * \brief   Read the "warmup" timeout time
 * \returns Returns the value set up for the "warmup" timeout in seconds
 */
uint16_t pa_warmup_timeout(void);

/**
 * \brief	Set the unused timeout time
 * \param	new_timeout	The new timeout value in seconds
 */
void pa_set_unused_timeout(uint16_t new_timeout);

/**
 * \brief   Read the "unused" timeout time
 * \returns Returns the value set up for the "unused" timeout in seconds
 */
uint16_t pa_unused_timeout(void);

/**
 * \brief	Set the cooldown timeout time
 * \param	new_timeout	The new timeout value in seconds
 */
void pa_set_cooldown_timeout(uint16_t new_timeout);

/**
 * \brief   Read the "cooldown" timeout time
 * \returns Returns the value set up for the "cooldown" timeout in seconds
 */
uint16_t pa_cooldown_timeout(void);

/**
 * \brief Set all configuration variables to their default values
 */
void pa_set_default_config(void);

/**
 * \brief Set which controller has allocated which band
 * \param band  The band (e.g. BAND_160M)
 * \param ctrlr The controller
 */
void pa_set_controller(uint8_t band, uint8_t ctrlr);

/**
 * \brief Clear a previously set controller, making the band unused
 * \param band  The band (e.g. BAND_160M)
 */
void pa_clear_controller(uint8_t band);

/**
 * \brief   Get the controller that is associated with a certain band
 * \param   band  The band (e.g. BAND_160M)
 * \returns Returns the associated controller or PA_CTRLR_UNUSED if unused
 */
uint8_t pa_controller(uint8_t band);

/**
 * \brief   Toggle the main power on the PA
 * \param   band  The band (e.g. BAND_160M)
 * \returns Returns 0 on success or -1 if an error occurred
 */
int8_t pa_toggle_mains(uint8_t band);

/**
 * \brief   Set mains to on on given band
 * \param   band  The band (e.g. BAND_160M)
 * \returns Returns 0 on success or -1 if an error occurred
 */
int8_t pa_mains_on(uint8_t band);

/**
 * \brief   Set mains to off on given band
 * \param   band  The band (e.g. BAND_160M)
 * \returns Returns 0 on success or -1 if an error occurred
 */
int8_t pa_mains_off(uint8_t band);

/**
 * \brief   Set mains to on on all bands
 * \returns Returns 0 on success or -1 if an error occurred
 */
int8_t pa_mains_all_on(void);

/**
 * \brief   Set mains to off on all bands
 * \returns Returns 0 on success or -1 if an error occurred
 */
int8_t pa_mains_all_off(void);

/**
 * \brief Tell the state machine that the tx_active pin have changed its state
 * \param band  The band (e.g. BAND_160M)
 * \param on    Set to 1 if tx_active is on or 0 otherwise
 */
void pa_set_tx_active(uint8_t band, int8_t on);

/**
 * \brief   Read the operational staus for this PA
 * \param   band  The band (e.g. BAND_160M)
 * \returns Returns one of the AMP_OP_STATUS_* defines
 */
uint8_t pa_op_status(uint8_t band);


/**
 * \brief Called when the operational status changes
 * \param   band  The band (e.g. BAND_160M)
 * 
 * This function should be implemented outside of the state machine to get
 * a notification of when the operational status changes. It is called by
 * the state machine as soon as the operational status changes.
 */
extern void pa_op_status_changed(uint8_t band);

/**
 * \brief Called when the controller in use changes
 * \param   band  The band (e.g. BAND_160M)
 * 
 * This function should be implemented outside of the state machine to get
 * a notification of when the controller in use changes. It is called by
 * the state machine as soon as the controller changes.
 */
extern void pa_ctrlr_changed(uint8_t band);

#endif /* _PA_H_ */
