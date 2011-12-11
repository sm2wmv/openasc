/* PS/2 keyboard routines by Jurre Hanema, version 1.0
 * ps2kbd.c
 *
 * Released under "THE BEER-WARE LICENSE" (Revision 42):
 * <kipmans@gmail.com> wrote this file. As long as you retain this notice you can 
 * do whatever you want with this stuff. If we meet some day, and you think this 
 * stuff is worth it, you can buy me a beer in return.
 */

#include <avr/io.h>
#include <avr/interrupt.h>
#include <avr/pgmspace.h>
#include <util/delay.h>

#include "ps2kbd.h"

#define LOGIC_XOR(a, b)	(((a) && !(b)) || ((b) && !(a)))


volatile uint8_t	bit_n = 1;
volatile uint8_t	n_bits = 0;
volatile uint8_t	buffer = 0;
volatile uint8_t	queue[KBD_BUFSIZE];
volatile uint8_t	queue_idx = 0;
volatile uint16_t	status = 0;

// Lookup table for 'normal' scancodes

const unsigned char lut_normal_keys[] PROGMEM = {
	0x1c, 'a',
	0x32, 'b',
	0x21, 'c',
	0x23, 'd',
	0x24, 'e',
	0x2b, 'f',
	0x34, 'g',
	0x33, 'h',
	0x43, 'i',
	0x3b, 'j',
	0x42, 'k',
	0x4b, 'l',
	0x3a, 'm',
	0x31, 'n',
	0x44, 'o',
	0x4d, 'p',
	0x15, 'q',
	0x2d, 'r',
	0x1b, 's',
	0x2c, 't',
	0x3c, 'u',
	0x2a, 'v',
	0x1d, 'w',
	0x22, 'x',
	0x35, 'y',
	0x1a, 'z',
	0x29, ' ',
	0x45, '0',
	0x16, '1',
	0x1e, '2',
	0x26, '3',
	0x25, '4',
	0x2e, '5',
	0x36, '6',
	0x3d, '7',
	0x3e, '8',
	0x46, '9',
/*	0xee, 'E',
	0xfa, 'A',
	0xfe, 'R',
	0x00, 'F',
	0xff, 'F',*/
	0, 0, 0
};


// Lookup table for 'normal' scancodes when SHIFT is held down
// If no entry is found in this table, the uppercase equivalent of the result
// from lut_normal_keys will be used.

const unsigned char lut_normal_keys_shift[] PROGMEM = {
	0x45, ')',
	0x16, '!',
	0x1e, '@',
	0x26, '#',
	0x25, '$',
	0x2e, '%',
	0x36, '^',
	0x3d, '&',
	0x3e, '*',
	0x46, '(',
	0, 0
};


// Lookup table for 'normal' scancodes when NUMLOCK is on
// Can, for example, be used to change the behaviour of the keypad keys if desired

const unsigned char lut_normal_keys_numlock[] PROGMEM = {
	0, 0
};


// Lookup table foor 'extended' scancodes
// Scancode without e0

const unsigned char lut_extended_keys[] PROGMEM = {
	/* For example, up, left, down and right arrows */
	0x75, 'u',
	0x6b, 'l',
	0x72, 'd',
	0x74, 'r',
	0, 0
};


// Begin actual implementation

void kbd_init(void)
{	
	// Set interrupts
	
	//KBD_SET_INT();
	//KBD_EN_INT();
	
	// Enable pullup on clock
	KBD_CLOCK_PORT |= _BV(KBD_CLOCK_BIT);
}


uint8_t kbd_queue_scancode(volatile uint8_t p)
{
	if(queue_idx < KBD_BUFSIZE)
	{
		queue[queue_idx] = p;
		queue_idx++;
	} else
		return 0;

	return 1;
}


uint8_t kbd_get_scancode(void)
{
	uint8_t		i;
	uint8_t		tmp;

	if(queue_idx > 0)
	{
		tmp = queue[queue_idx - 1]; 
		
		for(i = 0; i < KBD_BUFSIZE; i++)
			queue[i] = queue[i + 1];
		
		queue_idx--;
		
		return tmp;
	} else
		return 0;
}


void kbd_send(uint8_t data)
{
	if(status & KBD_SEND) _delay_ms(5);	// This behaviour isn't the most desirable,
	if(status & KBD_SEND) return;		// but it's the easiest and proved to be reliable.
	
	// Initiate request-to-send, the actual sending of the data
	// is handled in the ISR.
	
	KBD_CLOCK_PORT &= ~_BV(KBD_CLOCK_BIT);
	KBD_CLOCK_DDR |= _BV(KBD_CLOCK_BIT);
	_delay_us(120);
	KBD_DATA_DDR |= _BV(KBD_DATA_BIT);
	KBD_CLOCK_DDR &= ~_BV(KBD_CLOCK_BIT);
	KBD_CLOCK_PORT |= _BV(KBD_CLOCK_BIT);
	
	bit_n = 1;
	status |= KBD_SEND;
	n_bits = 0;
	buffer = data;
}


void kbd_update_leds(void)
{
	uint8_t	val = 0;

	if(status & KBD_CAPS) val |= 0x04;
	if(status & KBD_NUMLOCK) val |= 0x02;
	if(status & KBD_SCROLL) val |= 0x01;
	
	kbd_send(0xed);
	kbd_send(val);
}


unsigned char kbd_do_lookup(const unsigned char *lut, uint8_t sc)
{
	uint8_t	i;
	
	for(i = 0; pgm_read_byte(&lut[i]); i += 2)
		if(sc == pgm_read_byte(&lut[i]))
			return pgm_read_byte(&lut[i + 1]);
	return 0;
}


unsigned char kbd_getchar(void)
{
	uint8_t		sc = 0;
	unsigned char	c;
	
	while((sc = kbd_get_scancode()))
	{
		if(sc == 0xe0)
			status |= KBD_EX;
		else if(sc == 0xf0)
			status |= KBD_BREAK;
		else
		{
			if(status & KBD_BREAK)
			{
				status &= ~KBD_BREAK;
				status &= ~KBD_EX;
				
				if(sc == 0x12 || sc == 0x59)	// Shift
					status &= ~KBD_SHIFT;
				else if(sc == 0x14)		// Ctrl
					status &= ~KBD_CTRL;
				else if(sc == 0x11)		// Alt
					status &= ~KBD_ALT;
				else if(sc == 0x77 || sc == 0x58 || sc == 0x7e)	// Caps lock, num lock or scroll lock
					status &= ~KBD_LOCKED;
			} else if(status & KBD_EX)
			{
				status &= ~KBD_EX;
				
				if(sc == 0x14)			// R ctrl
					status |= KBD_CTRL;
				else if(sc == 0x11)		// R alt
					status |= KBD_ALT;
				else if((c = kbd_do_lookup(lut_extended_keys, sc)))
					return c;				
			} else
			{
				if(sc == 0x12 || sc == 0x59)	// Shift
					status |= KBD_SHIFT;
				else if(sc == 0x14)		// L ctrl
					status |= KBD_CTRL;
				else if(sc == 0x11)		// L alt
					status |= KBD_ALT;
				else if(sc == 0x77 && !(status & KBD_LOCKED))		// Num lock
				{
					status = (status & KBD_NUMLOCK) ? status & ~KBD_NUMLOCK : status | KBD_NUMLOCK;
					status |= KBD_LOCKED;
					kbd_update_leds();
				} else if(sc == 0x58 && !(status & KBD_LOCKED))		// Caps lock
				{
					status = (status & KBD_CAPS) ? status & ~KBD_CAPS : status | KBD_CAPS;
					status |= KBD_LOCKED;
					kbd_update_leds();
				} else if(sc == 0x7e && !(status & KBD_LOCKED))		// Scroll lock
				{
					status = (status & KBD_SCROLL) ? status & ~KBD_SCROLL : status | KBD_SCROLL;
					status |= KBD_LOCKED;
					kbd_update_leds();
				} else
				{
					if((status & KBD_SHIFT) && (c = kbd_do_lookup(lut_normal_keys_shift, sc)))
						return c;
					else if((status & KBD_NUMLOCK) && (c = kbd_do_lookup(lut_normal_keys_numlock, sc)))
						return c;
					else if((c = kbd_do_lookup(lut_normal_keys, sc)))
						return (LOGIC_XOR(status & KBD_SHIFT, status & KBD_CAPS) && (c >= 'a' && c <= 'z')) ? c - 32 : c;
				}
			}
		}
	}
	
	return 0;
}


uint16_t kbd_get_status(void)
{
	return status;
}


ISR(SIG_INTERRUPT6)
{
	if(status & KBD_SEND)
	{
		// Send data
		
		if(bit_n == 9)				// Parity bit
		{
			if(n_bits & 0x01)
				KBD_DATA_DDR |= _BV(KBD_DATA_BIT);
			else
				KBD_DATA_DDR &= ~_BV(KBD_DATA_BIT);
		} else if(bit_n == 10)			// Stop bit
			KBD_DATA_DDR &= ~_BV(KBD_DATA_BIT);
		else if(bit_n == 11)			// ACK bit, set by device
		{
			buffer = 0;
			bit_n = 0;
			status &= ~KBD_SEND;
		} else					// Data bits
		{
			if(buffer & (1 << (bit_n - 1)))
			{
				KBD_DATA_DDR &= ~_BV(KBD_DATA_BIT);
				n_bits++;
			} else
				KBD_DATA_DDR |= _BV(KBD_DATA_BIT);
		}
	} else
	{
		// Receive data
		
		if((bit_n > 0) && (bit_n < 10))		// Ignore start, parity & stop bit
		{
			if(!bit_is_clear(KBD_DATA_PIN, KBD_DATA_BIT))
				buffer |= (1 << (bit_n - 2));
		} else if(bit_n == 11)
		{
			kbd_queue_scancode(buffer);
			buffer = 0;
			bit_n = 0;
		}
	}
	
	bit_n++;
	
//	KBD_SET_INT();
}
