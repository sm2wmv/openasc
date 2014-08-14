/*! \file     general_io/sk3w_power_amp_ctrl/bsp.c
 *  \ingroup  general_io_group
 *  \brief    Board support package for the SK3W PA controller
 *  \author   Tobias Blomberg, SM0SVX
 *  \date     2012-05-23
 * 
 * The board support package is used to isolate hardware access from other
 * parts of the application. This will make it easier to port software to
 * other hardware if necessary.
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

#include <avr/io.h>

#include <delay.h>
#include <i2c.h>
#include <wmv_bus/bus.h>
#include <wmv_bus/bus_commands.h>
#include <wmv_bus/bus_ascii_cmd.h>

#include "qpn_port.h"
#include "bus_handler.h"
#include "bsp.h"



//! Used for timer compare to match 1 ms
#define OCR0_1MS 14
//! The tick interval for the QP framework
#define QF_TICK_INTERVAL    1000
//! The TX interval for the assertion bus messages
#define ASSERT_TX_INTERVAL  5000

/**
 * \brief Copy a bit from one variable to a band bitmap variable
 * \param bm    The bitmap variable to set
 * \param band  The corresponding band bit to set
 * \param var   The variable to read the bit from
 * \param bit   The source bit position
 * 
 * This macro will copy a bit from a variable to a band bitmap variable.
 * This can for example be used to read a bit from a port indicating some
 * condition on a band (e.g. an alarm) and put that bit in a bitmap variable
 * which then is easier to handle.
 * 
 * Example: SET_BAND_BIT(alarm, BAND_15M, portb, 3)
 */
#define SET_BAND_BIT(bm, band, var, bit) do { \
  const int8_t diff = (bit)-(band); \
  const uint8_t shift = (diff >= 0) ? diff : -diff; \
  if (diff >= 0) { \
    (bm) |= ((var) & ~_BV(bit)) >> shift; \
  } else { \
    (bm) |= ((var) & ~_BV(bit)) << shift; \
  } \
} while (0)


//! Counter to keep track of when to call QF_tick()
static uint16_t counter_qf_tick_interval = 0;
//! Variable containing the QF tick interval
static uint16_t qf_tick_interval = QF_TICK_INTERVAL;


static void bsp_init_timer_0(void);
static void bsp_init_ports(void);


void bsp_init(void) {
  delay_ms(250);
  bsp_init_ports();
  delay_ms(250);
  bsp_init_timer_0();
}


void bsp_init_timer_2(void) {
  TCCR2 = 0;
  TCNT2 = 0;
    /* Normal operation, toggle on compare, prescale clk/64 */
  TCCR2 = (1 << WGM21) | (0 << WGM20) | (0 << CS22) | (1 << CS21) | (1 << CS20);
  TIFR |= (1 << OCF2);
    /* Will trigger an interrupt each with an interval of 130us */
  OCR2 = 30;
  TIMSK |= (1 << OCIE2);
}


uint8_t bsp_get_tx_active(void) {
  return PINF & 0x3f;
}


uint16_t get_pa_alarm(void) {
  uint16_t alarm = 0;
  const uint8_t portb = PORTB;
  const uint8_t porte = PORTE;

  SET_BAND_BIT(alarm, BAND_160M, porte, 7);
  SET_BAND_BIT(alarm, BAND_80M, porte, 6);
  SET_BAND_BIT(alarm, BAND_40M, portb, 7);
  SET_BAND_BIT(alarm, BAND_20M, portb, 6);
  SET_BAND_BIT(alarm, BAND_15M, portb, 5);
  SET_BAND_BIT(alarm, BAND_10M, portb, 4);

  return alarm;
}


void bsp_set_pa_ptt(unsigned char band, int on) {
  int bv = 0;
  switch (band) {
    case BAND_160M:
      bv = _BV(0);
      break;
    case BAND_80M:
      bv = _BV(1);
      break;
    case BAND_40M:
      bv = _BV(2);
      break;
    case BAND_20M:
      bv = _BV(3);
      break;
    case BAND_15M:
      bv = _BV(4);
      break;
    case BAND_10M:
      bv = _BV(5);
      break;
    default:
      /* Send ERROR? */
      return;
  }

  if (on) {
    PORTC |= bv;
  }
  else {
    PORTC &= ~bv;
  }
}


void bsp_set_pa_mains(unsigned char band, int on) {
  int pdbv = 0;
  int pgbv = 0;
  switch (band) {
    case BAND_160M:
      pgbv = _BV(1);
      break;
    case BAND_80M:
      pgbv = _BV(0);
      break;
    case BAND_40M:
      pdbv = _BV(7);
      break;
    case BAND_20M:
      pdbv = _BV(6);
      break;
    case BAND_15M:
      pdbv = _BV(5);
      break;
    case BAND_10M:
      pdbv = _BV(4);
      break;
    default:
      /* Send ERROR? */
      return;
  }

  if (on) {
    PORTD |= pdbv;
    PORTG |= pgbv;
  }
  else {
    PORTD &= ~pdbv;
    PORTG &= ~pgbv;
  }
}


uint8_t bsp_mains_is_on(unsigned char band) {
  switch (band) {
    case BAND_160M:
      return (PORTG >> 1) & 0x01;
    case BAND_80M:
      return (PORTG >> 0) & 0x01;
    case BAND_40M:
      return (PORTD >> 7) & 0x01;
    case BAND_20M:
      return (PORTD >> 6) & 0x01;
    case BAND_15M:
      return (PORTD >> 5) & 0x01;
    case BAND_10M:
      return (PORTD >> 4) & 0x01;
    default:
      /* Send ERROR? */
      return 0;
  }
}


void bsp_set_pa_reset(unsigned char band, int on) {
  int bv = 0;
  switch (band) {
    case BAND_160M:
      bv = _BV(6);
      break;
    case BAND_80M:
      bv = _BV(4);
      break;
    case BAND_40M:
      bv = _BV(3);
      break;
    case BAND_20M:
      bv = _BV(2);
      break;
    case BAND_15M:
      bv = _BV(1);
      break;
    case BAND_10M:
      bv = _BV(0);
      break;
    default:
      /* Send ERROR? */
      return;
  }

  if (on) {
    PORTA |= bv;
  }
  else {
    PORTA &= ~bv;
  }
}




/**
 * \brief Initialize time 0 which is used as a 1ms time reference
 * 
 * Initialize timer 0 to use the main crystal clock and the output
 * compare interrupt feature to generate an interrupt approximately
 * once per millisecond to use as a general purpose time base.
 */
static void bsp_init_timer_0(void) {
  TCCR0 = 0;
    /* enable output compare interrupt */
  TIMSK |= (1 << OCIE0);
    /* CTC, prescale = 1024 */
  TCCR0 = (1 << WGM01) | (1 << CS02) | (1 << CS01) | (1 << CS00);
  TCNT0 = 0;
    /* match in aprox 1 ms,  */
  OCR0 = OCR0_1MS;
}


/**
 * \brief Set the direction and initial values of the ports
 */
static void bsp_init_ports(void) {
  DDRA = 0xFF;
  DDRB = 0x07;
  DDRC = 0xFF;
  DDRD = 0xFB;
  DDRE = 0x00;
  DDRF = 0x00;
  //PORTF = 0x3f; /* Enable pullups on TX_ACTIVE pins */
  DDRG = 0x03;

  PORTD |= (1 << 2);
  PORTD |= (1 << 3);
}






/**
 * \brief  Called by the QP framework on startup
 */
void QF_onStartup(void) {
}


/**
 * \brief  Called by the QP framework when no state machine want to run
 * 
 * The QP framework will call this function when no state machine object want
 * to run. The original purpose is to give the processor a chance to enter
 * low power mode. In this application we use the idle interrupt to drive the
 * normal main loop.
 */
void QF_onIdle(void) {          /* entered with interrupts LOCKED, see NOTE01 */
  QF_INT_ENABLE();
  bus_handler_poll();
}


/**
 * \brief Called by the QP framework if something goes seriously wrong
 * \param file The name of the file where the assertion occurred
 * \param line The line number where the assertion occurred
 * 
 * This function will be called from the QP framework when something goes
 * seriously wrong in a state machine. Assertions can be raised both by
 * the framework itself and also by the application code.
 * 
 * When the application goes into the assertion condition, it is not
 * possible to get out of that condition without resetting the card.
 * 
 * The card will continuously send an AMPLIFIER_ERROR message and also
 * an ASCII message containing the filename and the linenumber where
 * the assertion occurred.
 */
void Q_onAssert(char const /*Q_ROM */ *const Q_ROM_VAR file, int line) {
  qf_tick_interval = 0;         /* Disable calls to QF_tick() */

  char const Q_ROM_NOT_GNUC *Q_ROM_VAR s = file;
  char filename[256];
  char *ptr = filename;
  while ((*ptr++ = Q_ROM_BYTE(*s++)) != 0) ;
  *ptr = '\0';

  //QF_INT_DISABLE();
  counter_qf_tick_interval = ASSERT_TX_INTERVAL;
  for (;;) {
    bus_handler_poll_core();
    if (counter_qf_tick_interval >= ASSERT_TX_INTERVAL) {
      for (int i = 0; i < 6; ++i) {
        uint8_t msg[] = { i, 0 };
        bus_add_tx_message(bus_get_address(),
                           BUS_BROADCAST_ADDR,
                           0, BUS_CMD_AMPLIFIER_ERROR, sizeof(msg), msg);
      }
      bus_ascii_cmd_sendf(0, "ASSERT[%s:%d]\n", filename, line);
      counter_qf_tick_interval = 0;
    }
  }
}


/**
 * \brief Output compare 0 interrupt - "called" with 1ms intervals
 */
ISR(SIG_OUTPUT_COMPARE0) {
  bus_handler_tick();
  if (++counter_qf_tick_interval == qf_tick_interval) {
    QF_tick();
    counter_qf_tick_interval = 0;
  }
}
