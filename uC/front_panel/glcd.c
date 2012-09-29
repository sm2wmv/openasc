/*! \file glcd.c 
 *  \brief Graphic LCD API functions. */
//*****************************************************************************
//
// File Name	: 'glcd.c'
// Title		: Graphic LCD API functions
// Author		: Pascal Stang - Copyright (C) 2002 - Modified by Mikael Larsmark, SM3WMV
// Date			: 5/30/2002
// Revised		: 2007-11-24
// Version		: 0.5
// Target MCU	: Atmel AVR
// Editor Tabs	: 4
//
// NOTE: This code is currently below version 1.0, and therefore is considered
// to be lacking in some functionality or documentation, or may not be fully
// tested.  Nonetheless, you can expect most functions to work.
//
// This code is distributed under the GNU Public License
//		which can be found at http://www.gnu.org/licenses/gpl.txt
//
//*****************************************************************************

#ifndef WIN32
// AVR specific includes
	#include <avr/io.h>
	#include <avr/pgmspace.h>
#endif

#include "glcd.h"
#include "fonts.h"

// include hardware support
#include "ks0108.h"

// include fonts
#include "pictures.h"
#include "ethernet.h"
#include <string.h> 
#include <stdio.h>

extern unsigned char rxed_data;

static unsigned char glcd_buffer[GLCD_Y_BYTES][GLCD_X_BYTES];
static const unsigned char glcd_mask_array[8] = {0x01,0x02,0x04,0x08,0x10,0x20,0x40,0x80};

void glcd_update_area(unsigned char x1, unsigned char x2, unsigned char y1, unsigned char y2)
{
	glcd_set_address(x1,y1>>3);
	
	y1 = y1>>3;
	y2 = y2>>3;
	
	for (unsigned char y=y1;y<y2;y++)
		for (unsigned char x=x1;x<x2;x++)
			glcd_data_write(glcd_buffer[y][x]);
}

void glcd_update(unsigned int top, unsigned int bottom)
{
	glcd_set_address(0,0);

	unsigned char yt, yb;
	
	yt = top >> 3;				/* setup bytes of range */
	yb = bottom >> 3;
	
	for (unsigned char y=yt;y<yb;y++)
		for (unsigned char x=0;x<GLCD_X_BYTES;x++)
			glcd_data_write(glcd_buffer[y][x]);
    
    
  if (ethernet_is_active()) {
    ethernet_send_display_data(0,&glcd_buffer,1024);
    
    #ifdef ETHERNET_DEBUG_ENABLED
      printf("Send DISP data\r\n");
    #endif
  }
}

void glcd_glyph(unsigned char left, unsigned char top, unsigned char width, unsigned char height, const prog_char *glyph, unsigned char store_width)
{
	unsigned char bit_pos;
	unsigned char byte_offset;
	unsigned char y_bits;
	unsigned char remaining_bits;
	unsigned char mask;
	unsigned char char_mask;
	unsigned char x;
	const prog_char *glyph_scan;
	unsigned char glyph_offset;

	bit_pos = top & 0x07;		/* get the bit offset into a byte */

	glyph_offset = 0;			/* start at left side of the glyph rasters */
	char_mask = 0x80;			/* initial character glyph mask */

	for (x = left; x < (left + width); x++)
	{
		byte_offset = top >> 3;        	/* get the byte offset into y direction */
		y_bits = height;				/* get length in y direction to write */
		remaining_bits = 8 - bit_pos;	/* number of bits left in byte */
		mask = glcd_mask_array[bit_pos];	/* get mask for this bit */
		glyph_scan = glyph + glyph_offset;	 /* point to base of the glyph */

		/* boundary checking here to account for the possibility of  */
		/* write past the bottom of the screen.                        */
		while((y_bits) && (byte_offset < GLCD_Y_BYTES)) /* while there are bits still to write */
		{
		/* check if the character pixel is set or not */
			if(pgm_read_byte(glyph_scan) & char_mask)
					glcd_buffer[byte_offset][x] |= mask;	/* set image pixel */
			else
					glcd_buffer[byte_offset][x] &= ~mask;	/* clear the image pixel */

			if(glcd_mask_array[0] & 0x80)
				mask >>= 1;
			else
				mask <<= 1;
		
			y_bits--;
			remaining_bits--;
			
			if(remaining_bits == 0)
			{
				/* just crossed over a byte boundry, reset byte counts */
				remaining_bits = 8;
				byte_offset++;
				mask = glcd_mask_array[0];
			}

		/* bump the glyph scan to next raster */
			glyph_scan += store_width;
		}

	/* shift over to next glyph bit */
	char_mask >>= 1;
	if(char_mask == 0)				/* reset for next byte in raster */
	{
		char_mask = 0x80;
		glyph_offset++;
		}
	}
}

void glcd_set_byte(unsigned char x, unsigned char y, unsigned char curr_byte) {
	glcd_buffer[y][x] = curr_byte;
}

void glcd_text(unsigned char left, unsigned char top, unsigned char font, char *str, unsigned char length)
{
	unsigned char x = left;
	unsigned char glyph;
	unsigned char width;
	unsigned char height;
	unsigned char store_width;
	const prog_char *glyph_ptr;

	for (unsigned char count=0;count<length;count++) {
		glyph = (unsigned char)*str;

		/* check to make sure the symbol is a legal one */
		/* if not then just replace it with the default character */
		if((glyph < fonts[font].glyph_beg) || (glyph > fonts[font].glyph_end))
		{
			glyph = fonts[font].glyph_def;
		}

			/* make zero based index into the font data arrays */
		glyph -= fonts[font].glyph_beg;
		width = fonts[font].fixed_width;	/* check if it is a fixed width */
		if(width == 0)
			width=fonts[font].width_table[glyph];	/* get the variable width instead */

		height = fonts[font].glyph_height;
		store_width = fonts[font].store_width;
		
		glyph_ptr = fonts[font].glyph_table + ((unsigned int)glyph * (unsigned int)store_width * (unsigned int)height);

		/* range check / limit things here */
		if(x > GLCD_RIGHT)
			x = GLCD_RIGHT;
		if((x + width) > GLCD_RIGHT+1)
			width = GLCD_RIGHT - x + 1;
		if(top > GLCD_BOTTOM)
			top = GLCD_BOTTOM;
		if((top + height) > GLCD_BOTTOM+1)
			height = GLCD_BOTTOM - top + 1;

		glcd_glyph(x,top,width,height,glyph_ptr,store_width);  /* plug symbol into buffer */

		x += width;							/* move right for next character */
		str++;								/* point to next character in string */
	}
}

void glcd_clear_area(unsigned char x1, unsigned char x2, unsigned char y1, unsigned char y2)
{
	y1 = y1>>3;
	y2 = y2>>3;
	
	for (unsigned char y=y1;y<y2;y++)
		for (unsigned char x=x1;x<x2;x++)
			glcd_buffer[y][x] = 0x00;
}

void glcd_set_dot(unsigned char x, unsigned char y, unsigned char mode) {
	unsigned char bitnum, bitmask, yByte;
	
	if ( (x>=GLCD_RIGHT) || (y>=GLCD_BOTTOM) ) return;
	
	yByte   = y >> 3; // / 8
	bitnum  = y & 0x07;
	bitmask = glcd_mask_array[bitnum];
	switch (mode) {
		case GLCD_MODE_SET:
			glcd_buffer[yByte][x] |= bitmask;
			break;
		case GLCD_MODE_CLEAR:
			glcd_buffer[yByte][x] &= ~bitmask;
			break;
		case GLCD_MODE_XOR:
			glcd_buffer[yByte][x] ^= bitmask;
			break;
		default: break;
	}
}

void glcd_invert_area(unsigned char x1, unsigned char x2, unsigned char y1, unsigned char y2)
{
	for (unsigned char x=x1;x<=x2;x++)
		for (unsigned char y=y1;y<=y2;y++)
			glcd_set_dot(x,y,GLCD_MODE_XOR);
}

// draw line
void glcd_line(unsigned char x1, unsigned char x2, unsigned char y)
{
	for (unsigned char pos = x1;pos<=x2;pos++)
		glcd_set_dot(pos,y,GLCD_MODE_SET);
}

// draw rectangle
void glcd_rectangle(unsigned char x, unsigned char y, unsigned char a, unsigned char b)
{
  unsigned char j;

  for (j = 0; j < a; j++) {
		glcd_set_dot(x, y + j,GLCD_MODE_SET);
		glcd_set_dot(x + b - 1, y + j,GLCD_MODE_SET);
	}
	
  for (j = 0; j < b; j++)	{
		glcd_set_dot(x + j, y,GLCD_MODE_SET);
		glcd_set_dot(x + j, y + a - 1,GLCD_MODE_SET);
	}
}

// draw circle
void glcd_circle(unsigned char xcenter, unsigned char ycenter, unsigned char radius)
{
  int tswitch, y, x = 0;
  unsigned char d;

  d = ycenter - xcenter;
  y = radius;
  tswitch = 3 - 2 * radius;
  while (x <= y) {
    glcd_set_dot(xcenter + x, ycenter + y,GLCD_MODE_SET);     glcd_set_dot(xcenter + x, ycenter - y,GLCD_MODE_SET);
    glcd_set_dot(xcenter - x, ycenter + y,GLCD_MODE_SET);     glcd_set_dot(xcenter - x, ycenter - y,GLCD_MODE_SET);
    glcd_set_dot(ycenter + y - d, ycenter + x,GLCD_MODE_SET); glcd_set_dot(ycenter + y - d, ycenter - x,GLCD_MODE_SET);
    glcd_set_dot(ycenter - y - d, ycenter + x,GLCD_MODE_SET); glcd_set_dot(ycenter - y - d, ycenter - x,GLCD_MODE_SET);

    if (tswitch < 0) tswitch += (4 * x + 6);
    else {
      tswitch += (4 * (x - y) + 10);
      y--;
    }
    x++;
  }
}

void glcd_invert()
{
	for (unsigned char y=0;y<GLCD_Y_BYTES;y++)
		for (unsigned char x=0;x<GLCD_X_BYTES;x++)
			glcd_buffer[y][x] = ~glcd_buffer[y][x];
}

void glcd_clear(void)
{
	memset(&glcd_buffer,0x00,GLCD_Y_BYTES*GLCD_X_BYTES);
}

void glcd_print_picture(void)
{
	unsigned int i=0;

	for (unsigned char y=0;y<GLCD_Y_BYTES;y++)
		for (unsigned char x=0;x<GLCD_X_BYTES;x++)
		{
			glcd_buffer[y][x] = pgm_read_byte(&openASC_logo[i]);
			i++;
		}
			
	glcd_invert();
	glcd_update_all();
}