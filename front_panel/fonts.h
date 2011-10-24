//    Copyright (C) 2007  Mikael Larsmark, SM3WMV
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
#ifndef _FONTS_H
#define _FONTS_H

#include <avr/pgmspace.h>


/* select desired fonts. (Simply comment out those not needed) */
//#define EN_FIVE_DOT
#define EN_SIX_DOT
#define EN_SEVEN_DOT
#define EN_NINE_DOT
//#define EN_TEN_DOT
#define EN_FIFTEEN_DOT
#define EN_EIGHTEEN_DOT
#define EN_FONT_MT1

/* define number labels for the font selections */
typedef enum
{
#ifdef EN_FIVE_DOT
	FONT_FIVE_DOT,
#endif

#ifdef EN_SIX_DOT
	FONT_SIX_DOT,
#endif

#ifdef EN_SEVEN_DOT
	FONT_SEVEN_DOT,
#endif

#ifdef EN_NINE_DOT
	FONT_NINE_DOT,
#endif

#ifdef EN_TEN_DOT
	FONT_TEN_DOT,
#endif

#ifdef EN_FIFTEEN_DOT
	FONT_FIFTEEN_DOT,
#endif

#ifdef EN_EIGHTEEN_DOT
	FONT_EIGHTEEN_DOT,
#endif

#ifdef EN_FONT_MT1
	FONT_MT1,
#endif

	FONT_COUNT
} FONT_BASE;

struct FONT_DEF 
{
   uint8_t store_width;            /* glyph storage width in bytes */
   uint8_t glyph_height;  		 /* glyph height for storage */
   const prog_char *glyph_table;      /* font table start address in memory */
   uint8_t fixed_width;            /* fixed width of glyphs. If zero */
                                         /* then use the width table. */
   const prog_char *width_table; 	 /* variable width table start adress */
   uint8_t glyph_beg;			 	 /* start ascii offset in table */
   uint8_t glyph_end;				 /* end ascii offset in table */
   uint8_t glyph_def;				 /*  for undefined glyph  */
};

/* font definition tables for the fonts */
extern const struct FONT_DEF fonts[FONT_COUNT];

/* glyph bitmap and width tables for the fonts */ 
#ifdef EN_FIVE_DOT
  extern const uint8_t __attribute__ ((progmem)) five_dot_glyph_table[];
  extern const uint8_t five_dot_width_table[];
#endif

#ifdef EN_SIX_DOT
  extern const uint8_t __attribute__ ((progmem)) six_dot_glyph_table[];
  extern const uint8_t six_dot_width_table[];
#endif

#ifdef EN_SEVEN_DOT
#define DEG_CHAR ('~'+1)
  extern const uint8_t __attribute__ ((progmem)) seven_dot_glyph_table[];
  extern const uint8_t seven_dot_width_table[];
#endif

#ifdef EN_NINE_DOT
  extern const uint8_t  __attribute__ ((progmem)) nine_dot_glyph_table[];
#endif

#ifdef EN_TEN_DOT
  extern const uint8_t  __attribute__ ((progmem)) ten_dot_glyph_table[];
#endif

#ifdef EN_FIFTEEN_DOT
  extern const uint8_t __attribute__ ((progmem)) fifteen_dot_glyph_table[];
  extern const uint8_t  fifteen_dot_width_table[];
#endif

#ifdef EN_EIGHTEEN_DOT
  extern const uint8_t __attribute__ ((progmem)) eighteen_dot_glyph_table[];
  extern const uint8_t  eighteen_dot_width_table[];
#endif

#ifdef EN_FONT_MT1
  extern const uint8_t __attribute__ ((progmem)) font_mt1_glyph_table[];
  extern const uint8_t  font_mt1_width_table[];
#endif
	
#endif