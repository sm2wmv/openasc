/*! \file     general_io/sk3w_power_amp_ctrl/bus_handler.h
 *  \ingroup  general_io_group
 *  \brief    Handle bus communications for this application
 *  \author   Tobias Blomberg, SM0SVX
 *  \date     2012-05-23
 * 
 * Contains functions for handling application specific bus communications.
 */

#ifndef _BUS_HANDLER_H_
#define _BUS_HANDLER_H_


/**
 * \brief Initialize the bus handler
 */
void bus_handler_init(void);

/**
 * \brief Call from the periodic tick timer with 1ms interval
 */
void bus_handler_tick(void);

/**
 * \brief Handle the core of the bus communications
 * 
 * This function handle the core of the bus communications. It is normally
 * called from the bus_handler_poll function but under some circumstances,
 * like when in an assert condition, this function can be called directly
 * for just sustaining the base functionality.
 */
void bus_handler_poll_core(void);

/**
 * \brief Handle bus communications
 * 
 * This function should be called from the main loop or similar to "drive"
 * the bus. It will handle received mesages and perform the correct action.
 */
void bus_handler_poll(void);

/**
 * \brief Send an ASCII message on the bus
 * \param to_addr The address to send the message to
 * \param fmt     A formatted string with arguments, if any. Just like printf.
 */
void send_ascii_data(unsigned char to_addr, const char *fmt, ...);

#endif
