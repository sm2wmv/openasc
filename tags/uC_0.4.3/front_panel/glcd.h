/*! \file glcd.h \brief Graphic LCD API functions. */
//*****************************************************************************
//
// File Name	: 'glcd.h'
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
///	\ingroup front_panel_group
/// \code #include "glcd.h" \endcode
/// \par Overview
///		This library (or API) allows you to draw dots, lines, boxes, circles,
///	and text on most monochrome graphic LCDs.� An easily expandable font file
/// (5x7-pixel characters) is provided for all basic ASCII characters
/// (0x20-0x7F hex, 32-127 decimal).� An expandable graphic font file is
/// provided for defining specialty characters or custom icons.� Because this
/// library is designed to work with many different kinds of LCDs, it needs a
/// graphic LCD driver such as ks0108.c to enable it to talk to the LCD.
///
/// \note For full text-output functionality, you may wish to use the rprintf
/// functions along with this driver.
//
// This code is distributed under the GNU Public License
//		which can be found at http://www.gnu.org/licenses/gpl.txt
//
//*****************************************************************************

#ifndef GLCD_H
#define GLCD_H

#ifndef WIN32
// AVR specific includes
	#include <avr/io.h>
#endif

#include "../global.h"

#define GLCD_LEFT				0
#define GLCD_TOP				0
#define GLCD_RIGHT			128
#define GLCD_BOTTOM			64

#define GLCD_Y_BYTES 8
#define GLCD_X_BYTES 128

#define GLCD_MAXPAGE 		 8
#define GLCD_MAXADDRESS		 64

#define glcd_update_all() glcd_update(GLCD_TOP, GLCD_BOTTOM);

#define GLCD_MODE_CLEAR     0
#define GLCD_MODE_SET       1
#define GLCD_MODE_XOR       2

#define LINE1		0
#define LINE2		1
#define LINE3		2
#define LINE4		3
#define LINE5		4
#define LINE6		5
#define LINE7		6
#define LINE8		7

#define ON			1
#define OFF			0

// API-level interface commands
// ***** Public Functions *****
//! set a dot on the display (x is horiz 0:127, y is vert 0:63)
void glcd_set_dot(unsigned char x, unsigned char y, unsigned char mode);

//! draw line
void glcd_line(unsigned char x1,  unsigned char x2, unsigned char y);

//! draw rectangle (coords????)
void glcd_rectangle(unsigned char x, unsigned char y, unsigned char a, unsigned char b);

//! draw circle of radius at xcenter,ycenter
void glcd_circle(unsigned char xcenter, unsigned char ycenter, unsigned char radius);

void glcd_print_picture(void);

void glcd_invert_area(unsigned char x1, unsigned char x2, unsigned char y1, unsigned char y2);

void glcd_update(unsigned int top, unsigned int bottom);
void glcd_text(unsigned char left, unsigned char top, unsigned char font, char *str, unsigned char length);
void glcd_invert(void);
void glcd_clear(void);
void glcd_update_area(unsigned char x1, unsigned char x2, unsigned char y1, unsigned char y2);
void glcd_clear_area(unsigned char x1, unsigned char x2, unsigned char y1, unsigned char y2);

void glcd_set_byte(unsigned char x, unsigned char y, unsigned char curr_byte);

#endif
