/*! \file front_panel/interrupt_handler.c 
 *  \brief Handles different external interrupts
 *  \ingroup front_panel_group 
 *  \author Mikael Larsmark, SM2WMV
 *  \date 2010-01-25
 *  \code #include "front_panel/interrupt_handler.c" \endcode
 */
//    Copyright (C) 2008  Mikael Larsmark, SM2WMV
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

#include <stdio.h>
#include <stdlib.h>
#include <avr/io.h>
#include <avr/interrupt.h>

#include "main.h"
#include "board.h"


/*! Polls the status of all buttons on the front panel and returns it as an integer.
 *  The bit mask pattern is defined in board.h
 *  
 *  \return Which buttons that are currently pressed, see mask pattern in board.h
 */
int ih_poll_buttons(void) {
	int temp = 0;
	
	if ((PINL & (1<<BUTTON1_TX_BIT)) == 0)
		temp |= (1<<FLAG_BUTTON1_TX_BIT);

	if ((PINL & (1<<BUTTON2_TX_BIT)) == 0)
		temp |= (1<<FLAG_BUTTON2_TX_BIT);

	if ((PINL & (1<<BUTTON3_TX_BIT)) == 0)
		temp |= (1<<FLAG_BUTTON3_TX_BIT);

	if ((PINL & (1<<BUTTON4_TX_BIT)) == 0)
		temp |= (1<<FLAG_BUTTON4_TX_BIT);
	
	if ((PINH & (1<<BUTTON1_RX_BIT)) == 0)
		temp |= (1<<FLAG_BUTTON1_RX_BIT);
	
	if ((PINH & (1<<BUTTON2_RX_BIT)) == 0)
		temp |= (1<<FLAG_BUTTON2_RX_BIT);
	
	if ((PINH & (1<<BUTTON3_RX_BIT)) == 0)
		temp |= (1<<FLAG_BUTTON3_RX_BIT);
	
	if ((PINB & (1<<BUTTON4_RX_BIT)) == 0)
		temp |= (1<<FLAG_BUTTON4_RX_BIT);

	if ((PINC & (1<<BUTTON_ROTATE_BIT)) == 0)
		temp |= (1<<FLAG_BUTTON_ROTATE_BIT);
	
	if ((PINC & (1<<BUTTON_TXRX_BIT)) == 0)
		temp |= (1<<FLAG_BUTTON_TXRX_BIT);
		
	if ((PIND & (1<<BUTTON_RXANT_BIT)) == 0)
		temp |= (1<<FLAG_BUTTON_RXANT_BIT);
		
	if ((PIND & (1<<BUTTON_SUBMENU_BIT)) == 0)
		temp |= (1<<FLAG_BUTTON_SUBMENU_BIT);
		
	if ((PINK & (1<<BUTTON_MENU_BIT)) == 0)
		temp |= (1<<FLAG_BUTTON_MENU_BIT);
		
	if ((PING & (1<<BUTTON_PULSE_BIT)) == 0)
		temp |= (1<<FLAG_BUTTON_PULSE_BIT);

	if ((PING & (1<<BUTTON_AUX1_BIT)) == 0)
		temp |= (1<<FLAG_BUTTON_AUX1_BIT);
	
	if ((PING & (1<<BUTTON_AUX2_BIT)) == 0) {
			temp |= (1<<FLAG_BUTTON_AUX2_BIT);
			
			/*! The following is done because of a hardware bug! The pullups on the uC are too small
			    to actually charge the debounce capacitor in time. The way we solve it is that by making the 
			    pin to an output we can charge the capacitor and then go over to using the pin as 
			    input and reactivate the pullups again. */
			DDRG |= (1<<BUTTON_AUX2_BIT);
			PORTG |= (1<<BUTTON_AUX2_BIT);
			
			DDRG &= ~(1<<BUTTON_AUX2_BIT);
			PORTG |= (1<<BUTTON_AUX2_BIT);
	}
	
	return(temp);
}

/*! Polls the status of all the external inputs. This function does not care if the device is active low
 *  or active high. It will just return the current state so the handeling of leveling needs to be done elsewhere.
 *  The bit mask pattern is defined in board.h
 *  
 *  \return The status of the external devices
 */
unsigned char ih_poll_ext_devices(void) {
	unsigned char temp = 0;
	
	if (PINE & (1<<EXT_FOOTSWITCH_BIT))
		temp |= (1<<STATUS_FOOTSWITCH_BIT);
	
	if (PINE & (1<<EXT_RADIO_SENSE1_BIT))
		temp |= (1<<STATUS_RADIO_SENSE1_BIT);
	
	if (PINJ & (1<<EXT_RADIO_SENSE2_BIT))
		temp |= (1<<STATUS_RADIO_SENSE2_BIT);
	
	if (PINJ & (1<<EXT_USB1_DTR_BIT))
		temp |= (1<<STATUS_USB1_DTR_BIT);

	if (PINJ & (1<<EXT_USB2_DTR_BIT))
		temp |= (1<<STATUS_USB2_DTR_BIT);

	if (PINJ & (1<<EXT_USB2_RTS_BIT))
		temp |= (1<<STATUS_USB2_RTS_BIT);
	
	return(temp);
}