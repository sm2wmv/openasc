/*! \file     general_io/sk3w_rotator_control/main.c
 *  \ingroup  general_io_group
 *  \brief    Main file of the SK3W rotator control logic
 *  \author   Tobias Blomberg, SM0SVX
 *  \date     2010-05-18
 */
//    Copyright (C) 2009  Mikael Larsmark, SM2WMV
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

#include "qpn_port.h"
#include "rotator.h"
#include "bsp.h"
#include "bus_handler.h"


/*! Main function of the General I/O base SK3W PA control */
int main(void) {
  QF_INT_DISABLE();
  bsp_init();
  bus_handler_init();
  rotator_init();
  QF_INT_ENABLE();

  QF_run();                     /* transfer control to QF-nano */

  return 0;
}
