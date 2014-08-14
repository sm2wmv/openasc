/*! \file delay.c \brief Delay routine
 * \author Mikael Larsmark, SM2WMV
 * \date 2008-04-06
 */
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

#include "delay.h"

/*! \brief Millisecond delay
 * \param ms the delay in milliseconds
 */
void delay_ms(unsigned char ms)
{
   unsigned short delay_count = F_CPU / 4000;
#ifdef __GNUC__
    unsigned short cnt;
    __asm__ __volatile__ ("\n"
                  "L_dl1%=:\n\t"
                  "mov %A0, %A2\n\t"
                  "mov %B0, %B2\n"
                  "L_dl2%=:\n\t"
                  "sbiw %A0, 1\n\t"
                  "brne L_dl2%=\n\t"
                  "dec %1\n\t" "brne L_dl1%=\n\t":"=&w" (cnt)
                  :"r"(ms), "r"((unsigned short) (delay_count))
        );
#else
    unsigned short delay_cnt = F_CPU/6000;
    //unsigned short delay_cnt = 2400;   //*KU* for 14.745600 MHz Clock
    unsigned short delay_cnt_buffer;

    while (ms--) {
        delay_cnt_buffer = delay_cnt;
        while (delay_cnt_buffer--);
    }
#endif
}

/*! \brief Seconds delay
 * \param s delay in seconds
 */
void delay_s(unsigned char s)
{
	for (unsigned char i=0;i<s;i++)
	{
		delay_ms(250);
		delay_ms(250);
		delay_ms(250);
		delay_ms(250);
	}
}