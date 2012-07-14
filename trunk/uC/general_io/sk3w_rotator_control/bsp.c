/*! \file     general_io/sk3w_rotator_control/bsp.h
 *  \ingroup  general_io_group
 *  \brief    Board support package for the SK3W rotator controller
 *  \author   Tobias Blomberg, SN0SVX
 *  \date     2012-06-16
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

#include "qpn_port.h"
#include "bus_handler.h"
#include "bsp.h"



//! Used for timer compare to match 1 ms
#define OCR0_1MS 14
//! The tick interval for the QP framework
#define QF_TICK_INTERVAL    100
//! The TX interval for the assertion bus messages
#define ASSERT_TX_INTERVAL  5000
//! The number of rotators that we can control
#define ROTATOR_COUNT           QF_MAX_ACTIVE

#define ADC_INTERVAL            20

Q_DEFINE_THIS_FILE              /* Define file name to make assertions work */

//! Counter to keep track of when to call QF_tick()
static uint16_t counter_qf_tick_interval = 0;
//! Variable containing the QF tick interval
static uint16_t qf_tick_interval = QF_TICK_INTERVAL;

//! Counter to keep track of when to poll the ADC
unsigned int counter_adc_interval = 0;
//! Selected ADC channel
static uint8_t adc_ch = 0;
//! The last read ADC value
static uint16_t adc_value = 0;

static const int adc_mapping[ROTATOR_COUNT] = {0, 2, 4, 1, 3};


static void bsp_init_timer_0(void);
static void bsp_init_ports(void);
static void bsp_init_adc(void);


void bsp_init(void) {
  delay_ms(250);
  bsp_init_ports();
  delay_ms(250);
  bsp_init_timer_0();
  bsp_init_adc();
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


int8_t bsp_rotator_release_break(uint8_t rot_idx) {
  Q_REQUIRE(rot_idx < ROTATOR_COUNT);
  if ((rot_idx > 2) && (rot_idx < 6)) {
    PORTA |= _BV((rot_idx-3)*3+2);
    return 0;
  }
  return -1;
}


int8_t bsp_rotator_apply_break(uint8_t rot_idx) {
  Q_REQUIRE(rot_idx < ROTATOR_COUNT);
  if ((rot_idx > 2) && (rot_idx < 6)) {
    PORTA &= ~_BV((rot_idx-3)*3+2);
    return 0;
  }
  return -1;
}


void bsp_rotator_run_ccw(uint8_t rot_idx) {
  Q_REQUIRE(rot_idx < ROTATOR_COUNT);
  if (rot_idx <= 2) {
    PORTC &= ~_BV(4-rot_idx*2);
    PORTC |= _BV(5-rot_idx*2);
  } else {
    PORTA |= _BV((rot_idx-3)*3);
    PORTA &= ~_BV((rot_idx-3)*3+1);
  }
}


void bsp_rotator_run_cw(uint8_t rot_idx) {
  Q_REQUIRE(rot_idx < ROTATOR_COUNT);
  if (rot_idx <= 2) {
    PORTC |= _BV(4-rot_idx*2);
    PORTC &= ~_BV(5-rot_idx*2);
  } else {
    PORTA &= ~_BV((rot_idx-3)*3);
    PORTA |= _BV((rot_idx-3)*3+1);
  }
}


int8_t bsp_rotator_is_running(uint8_t rot_idx) {
  uint8_t is_running = 0;
  if (rot_idx <= 2) {
    is_running = PORTC & (_BV(4-rot_idx*2) | _BV(5-rot_idx*2));
  } else {
    is_running = PORTA & (_BV((rot_idx-3)*3) | _BV((rot_idx-3)*3+1));
  }
  return is_running != 0;
}


void bsp_rotator_stop(uint8_t rot_idx) {
  Q_REQUIRE(rot_idx < ROTATOR_COUNT);
  if (rot_idx <= 2) {
    PORTC &= ~(_BV(4-rot_idx*2) | _BV(5-rot_idx*2));
  } else {
    PORTA &= ~(_BV((rot_idx-3)*3) | _BV((rot_idx-3)*3+1));
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
  DDRD = 0x0B;
  DDRE = 0x00;
  DDRF = 0x00;
  DDRG = 0x00;

  PORTD |= (1<<2);
  PORTD |= (1<<3);

#if 0
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
#endif
}


static void bsp_init_adc(void) {
    /* AREF as reference voltage */
    /* ADC left adjust result.   */
  ADMUX = _BV(ADLAR);

    /* ADC interrupt enable */
    /* ADC prescaler division factor 128 */
  ADCSRA = _BV(ADIE) | _BV(ADPS2) | _BV(ADPS1) | _BV(ADPS0);
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

  if (counter_adc_interval >= ADC_INTERVAL) {
    counter_adc_interval = 0;

    bsp_direction_updated(adc_ch, adc_value);

      /* Switch to next ADC channel and start a conversion. */
    if (++adc_ch == ROTATOR_COUNT) {
      adc_ch = 0;
    }
    ADMUX = (ADMUX & 0xf0) | adc_mapping[adc_ch];
    ADCSRA |= (_BV(ADEN) | _BV(ADSC));
  }
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
      send_ascii_data(0, "ASSERT[%s:%d]\r\n", filename, line);
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
  counter_adc_interval++;
}



/*! \brief Interrupt service routine for ADC conversion finished */
SIGNAL(SIG_ADC) {
    /* Read the ADC conversion result */
  adc_value = ADCL;
  adc_value |= ADCH << 8;

    /* Turn off ADC clock (saves power) */
  ADCSRA &= ~_BV(ADEN);
}
