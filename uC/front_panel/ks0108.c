/*! \file ks0108.c \brief Graphic LCD driver for HD61202/KS0108 displays. */
//*****************************************************************************
//
// File Name	: 'ks0108.c'
// Title		: Graphic LCD driver for HD61202/KS0108 displays
// Author		: Pascal Stang - Copyright (C) 2001-2003 - Modified by Mikael Larsmark
// Date			: 10/19/2002
// Revised		: 24/11/2007
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
	#include <avr/interrupt.h>
#endif

#include "../global.h"
#include "ks0108.h"
#include "glcd.h"

// global variables
GrLcdStateType GrLcdState;

/*************************************************************/
/********************** LOCAL FUNCTIONS **********************/
/*************************************************************/

void glcd_init_hw(void)
{
	// initialize I/O ports
	// if I/O interface is in use
#ifdef GLCD_PORT_INTERFACE

	// initialize LCD control lines levels
	cbi(GLCD_CTRL_PORT, GLCD_CTRL_RS);
	cbi(GLCD_CTRL_PORT, GLCD_CTRL_RW);
	cbi(GLCD_CTRL_PORT, GLCD_CTRL_E);
	cbi(GLCD_CTRL_PORT, GLCD_CTRL_CS0);
	cbi(GLCD_CTRL_PORT, GLCD_CTRL_CS1);
	cbi(GLCD_CTRL_PORT, GLCD_CTRL_CS2);
	cbi(GLCD_CTRL_PORT, GLCD_CTRL_CS3);
	cbi(GLCD_CTRL_PORT, GLCD_CTRL_RESET);
	// initialize LCD control port to output
	sbi(GLCD_CTRL_DDR, GLCD_CTRL_RS);
	sbi(GLCD_CTRL_DDR, GLCD_CTRL_RW);
	sbi(GLCD_CTRL_DDR, GLCD_CTRL_E);
	sbi(GLCD_CTRL_DDR, GLCD_CTRL_CS0);
	sbi(GLCD_CTRL_DDR, GLCD_CTRL_CS1);
	sbi(GLCD_CTRL_DDR, GLCD_CTRL_CS2);
	sbi(GLCD_CTRL_DDR, GLCD_CTRL_CS3);
	sbi(GLCD_CTRL_DDR, GLCD_CTRL_RESET);
	// initialize LCD data
	outb(GLCD_DATA_PORT,0x00);
	// initialize LCD data port to output
	GLCD_DATA_DDR = 0xFF;
#endif
}

void glcd_controller_select(u08 controller)
{
#ifdef GLCD_PORT_INTERFACE
	// unselect all controllers
	cbi(GLCD_CTRL_PORT, GLCD_CTRL_CS0);
	cbi(GLCD_CTRL_PORT, GLCD_CTRL_CS1);
	cbi(GLCD_CTRL_PORT, GLCD_CTRL_CS2);
	cbi(GLCD_CTRL_PORT, GLCD_CTRL_CS3);

	// select requested controller
	switch(controller)
	{
	case 0:	sbi(GLCD_CTRL_PORT, GLCD_CTRL_CS0); break;
	case 1:	sbi(GLCD_CTRL_PORT, GLCD_CTRL_CS1); break;
	case 2:	sbi(GLCD_CTRL_PORT, GLCD_CTRL_CS2); break;
	case 3:	sbi(GLCD_CTRL_PORT, GLCD_CTRL_CS3); break;
	default: break;
	}
#endif
}

void glcd_busy_wait(u08 controller)
{
#ifdef GLCD_PORT_INTERFACE
//	ENTER_CRIT;
	// wait until LCD busy bit goes to zero
	// select the controller chip
	glcd_controller_select(controller);
	// do a read from control register
	outb(GLCD_DATA_PORT,0xFF);
	cbi(GLCD_CTRL_PORT, GLCD_CTRL_RS);
	GLCD_DATA_DDR = 0x00;
	sbi(GLCD_CTRL_PORT, GLCD_CTRL_RW);
	sbi(GLCD_CTRL_PORT, GLCD_CTRL_E);
	__asm__ __volatile__("nop"); __asm__ __volatile__("nop");
	while(inb(GLCD_DATA_PIN) & GLCD_STATUS_BUSY)
	{
		cbi(GLCD_CTRL_PORT, GLCD_CTRL_E);
		__asm__ __volatile__ ("nop"); __asm__ __volatile__ ("nop");
		__asm__ __volatile__ ("nop"); __asm__ __volatile__ ("nop");
		sbi(GLCD_CTRL_PORT, GLCD_CTRL_E);
		__asm__ __volatile__ ("nop"); __asm__ __volatile__ ("nop");
		__asm__ __volatile__ ("nop"); __asm__ __volatile__ ("nop");
	}
	cbi(GLCD_CTRL_PORT, GLCD_CTRL_E);
	cbi(GLCD_CTRL_PORT, GLCD_CTRL_RW);
	GLCD_DATA_DDR = 0xFF;
//	EXIT_CRIT;
#else
	// sbi(MCUCR, SRW);			// enable RAM waitstate
	// wait until LCD busy bit goes to zero
	while(*(volatile unsigned char *)
		(GLCD_CONTROLLER0_CTRL_ADDR + GLCD_CONTROLLER_ADDR_OFFSET*controller) & GLCD_STATUS_BUSY);
	// cbi(MCUCR, SRW);			// disable RAM waitstate
#endif
}

void glcd_control_write(u08 controller, u08 data)
{
#ifdef GLCD_PORT_INTERFACE
	glcd_busy_wait(controller);	// wait until LCD not busy
	cbi(GLCD_CTRL_PORT, GLCD_CTRL_RS);
	cbi(GLCD_CTRL_PORT, GLCD_CTRL_RW);
	sbi(GLCD_CTRL_PORT, GLCD_CTRL_E);
	GLCD_DATA_DDR = 0xFF;
	outb(GLCD_DATA_PORT,data);
	__asm__ __volatile__ ("nop"); __asm__ __volatile__ ("nop");
	__asm__ __volatile__ ("nop"); __asm__ __volatile__ ("nop");
	__asm__ __volatile__ ("nop"); __asm__ __volatile__ ("nop");
	__asm__ __volatile__ ("nop"); __asm__ __volatile__ ("nop");
	cbi(GLCD_CTRL_PORT, GLCD_CTRL_E);
#else
	//sbi(MCUCR, SRW);				// enable RAM waitstate
	glcd_busy_wait(controller);		// wait until LCD not busy
	*(volatile unsigned char *) (GLCD_CONTROLLER0_CTRL_ADDR + GLCD_CONTROLLER_ADDR_OFFSET*controller) = data;
	//cbi(MCUCR, SRW);				// disable RAM waitstate
#endif
}

u08 glcd_control_read(u08 controller)
{
	register u08 data;
#ifdef GLCD_PORT_INTERFACE
	glcd_busy_wait(controller);		// wait until LCD not busy
	cbi(GLCD_CTRL_PORT, GLCD_CTRL_RS);
	GLCD_DATA_DDR = 0x00;
	sbi(GLCD_CTRL_PORT, GLCD_CTRL_RW);
	sbi(GLCD_CTRL_PORT, GLCD_CTRL_E);
	__asm__ __volatile__ ("nop"); __asm__ __volatile__ ("nop");
	__asm__ __volatile__ ("nop"); __asm__ __volatile__ ("nop");
	__asm__ __volatile__ ("nop"); __asm__ __volatile__ ("nop");
	__asm__ __volatile__ ("nop"); __asm__ __volatile__ ("nop");
	data = inb(GLCD_DATA_PIN);
	cbi(GLCD_CTRL_PORT, GLCD_CTRL_E);
	cbi(GLCD_CTRL_PORT, GLCD_CTRL_RW);
	GLCD_DATA_DDR = 0xFF;
#else
	//sbi(MCUCR, SRW);				// enable RAM waitstate
	glcd_busy_wait(controller);		// wait until LCD not busy
	data = *(volatile unsigned char *) (GLCD_CONTROLLER0_CTRL_ADDR + GLCD_CONTROLLER_ADDR_OFFSET*controller);
	//cbi(MCUCR, SRW);				// disable RAM waitstate
#endif
	return data;
}

void glcd_data_write(u08 data)
{
	register u08 controller = (GrLcdState.lcdXAddr/GLCD_CONTROLLER_XPIXELS);
#ifdef GLCD_PORT_INTERFACE
	glcd_busy_wait(controller);		// wait until LCD not busy
	sbi(GLCD_CTRL_PORT, GLCD_CTRL_RS);
	cbi(GLCD_CTRL_PORT, GLCD_CTRL_RW);
	sbi(GLCD_CTRL_PORT, GLCD_CTRL_E);
	GLCD_DATA_DDR = 0xFF;
	outb(GLCD_DATA_PORT,data);
	__asm__ __volatile__ ("nop"); __asm__ __volatile__ ("nop");
	__asm__ __volatile__ ("nop"); __asm__ __volatile__ ("nop");
	__asm__ __volatile__ ("nop"); __asm__ __volatile__ ("nop");
	__asm__ __volatile__ ("nop"); __asm__ __volatile__ ("nop");
	cbi(GLCD_CTRL_PORT, GLCD_CTRL_E);
#else
	//sbi(MCUCR, SRW);				// enable RAM waitstate
	glcd_busy_wait(controller);		// wait until LCD not busy
	*(volatile unsigned char *) (GLCD_CONTROLLER0_CTRL_ADDR + GLCD_CONTROLLER_ADDR_OFFSET*controller) = data;
	//cbi(MCUCR, SRW);				// disable RAM waitstate
#endif
	// increment our local address counter
	GrLcdState.ctrlr[controller].xAddr++;
	GrLcdState.lcdXAddr++;
	if(GrLcdState.lcdXAddr >= GLCD_XPIXELS)
	{
		GrLcdState.lcdYAddr++;
		glcd_set_y_address(GrLcdState.lcdYAddr);
		glcd_set_x_address(0);
	}
}

u08 glcd_data_read(void)
{
	register u08 data;
	register u08 controller = (GrLcdState.lcdXAddr/GLCD_CONTROLLER_XPIXELS);
#ifdef GLCD_PORT_INTERFACE
	glcd_busy_wait(controller);		// wait until LCD not busy
	sbi(GLCD_CTRL_PORT, GLCD_CTRL_RS);
	GLCD_DATA_DDR = 0x00;
	sbi(GLCD_CTRL_PORT, GLCD_CTRL_RW);
	sbi(GLCD_CTRL_PORT, GLCD_CTRL_E);
	__asm__ __volatile__ ("nop"); __asm__ __volatile__ ("nop");
	__asm__ __volatile__ ("nop"); __asm__ __volatile__ ("nop");
	__asm__ __volatile__ ("nop"); __asm__ __volatile__ ("nop");
	__asm__ __volatile__ ("nop"); __asm__ __volatile__ ("nop");
	data = inb(GLCD_DATA_PIN);
	cbi(GLCD_CTRL_PORT, GLCD_CTRL_E);
	cbi(GLCD_CTRL_PORT, GLCD_CTRL_RW);
#else
	//sbi(MCUCR, SRW);				// enable RAM waitstate
	glcd_busy_wait(controller);		// wait until LCD not busy
	data = *(volatile unsigned char *) (GLCD_CONTROLLER0_CTRL_ADDR + GLCD_CONTROLLER_ADDR_OFFSET*controller);
	//cbi(MCUCR, SRW);				// disable RAM waitstate
#endif
	// increment our local address counter
	GrLcdState.ctrlr[controller].xAddr++;
	GrLcdState.lcdXAddr++;
	if(GrLcdState.lcdXAddr >= GLCD_XPIXELS)
	{
		GrLcdState.lcdYAddr++;
		glcd_set_y_address(GrLcdState.lcdYAddr);
		glcd_set_x_address(0);
	}
	return data;
}

void glcd_reset(u08 reset_state)
{
	// reset lcd if argument is true
	// run lcd if argument is false
#ifdef GLCD_PORT_INTERFACE
	if(reset_state)
		cbi(GLCD_CTRL_PORT, GLCD_CTRL_RESET);
	else
		sbi(GLCD_CTRL_PORT, GLCD_CTRL_RESET);
#endif
}

u08 glcd_get_x_address()
{
	return (GrLcdState.lcdXAddr);
}

u08 glcd_get_y_address()
{
	return (GrLcdState.lcdYAddr);
}


void glcd_set_x_address(u08 xAddr)
{
	u08 i;
	// record address change locally
	GrLcdState.lcdXAddr = xAddr;
	
	// clear y (col) address on all controllers
	for(i=0; i<GLCD_NUM_CONTROLLERS; i++)
	{
		glcd_control_write(i, GLCD_SET_Y_ADDR | 0x00);
		GrLcdState.ctrlr[i].xAddr = 0;
	}

	// set y (col) address on destination controller
	glcd_control_write((GrLcdState.lcdXAddr/GLCD_CONTROLLER_XPIXELS),
		GLCD_SET_Y_ADDR | (GrLcdState.lcdXAddr & 0x3F));
}

void glcd_set_y_address(u08 yAddr)
{
	u08 i;
	// record address change locally
	GrLcdState.lcdYAddr = yAddr;
	// set page address for all controllers
	for(i=0; i<GLCD_NUM_CONTROLLERS; i++)
	{
		glcd_control_write(i, GLCD_SET_PAGE | yAddr);
	}
}


/*************************************************************/
/********************* PUBLIC FUNCTIONS **********************/
/*************************************************************/

void glcd_init()
{
	u08 i;
	
	// initialize hardware
	glcd_init_hw();
	// bring lcd out of reset
	glcd_reset(FALSE);
	// Turn on LCD
	for(i=0; i<GLCD_NUM_CONTROLLERS; i++)
	{
		glcd_control_write(i, GLCD_ON_CTRL | GLCD_ON_DISPLAY);
	}
	// clear lcd
	glcd_clear_screen();
	// initialize positions
	glcd_home();
}

void glcd_home(void)
{
	u08 i;
	// initialize addresses/positions
	glcd_start_line(0);
	glcd_set_address(0,0);
	// initialize local data structures
	for(i=0; i<GLCD_NUM_CONTROLLERS; i++)
	{
		GrLcdState.ctrlr[i].xAddr = 0;
		GrLcdState.ctrlr[i].yAddr = 0;
	}
}

void glcd_clear_screen(void)
{
	glcd_clear();
}

void glcd_start_line(u08 start)
{
	glcd_control_write(0, GLCD_START_LINE | start);
	glcd_control_write(1, GLCD_START_LINE | start);
}

void glcd_set_address(u08 x, u08 yLine)
{
	// set addresses
	glcd_set_y_address(yLine);
	glcd_set_x_address(x);
}

void glcd_goto_char(u08 line, u08 col)
{
	glcd_set_address(col*6, line);
}

void glcd_delay(u16 p)			// 1-8us      ...2-13us     ...5-31us
{								// 10-60us    ...50-290us
	unsigned int i;				// 100-580us  ...500-2,9ms
	unsigned char j; 			// 1000-5,8ms ...5000-29ms
								// 10000-56ms ...30000-170ms
								// 50000-295ms...60000-345ms
	//for (i = 0; i < p; i++) for (j = 0; j < 10; j++) __asm__ __volatile__ ("nop");
	for (i = 0; i < p; i++) for (j = 0; j < 10; j++);
}


// Higher level functionality has been moved to the API-layer glcd.c/glcd.h
