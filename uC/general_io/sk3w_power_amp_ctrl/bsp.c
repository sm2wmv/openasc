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

#define QF_TICK_INTERVAL    1000
#define ASSERT_TX_INTERVAL  5000

//! Counter to keep track of when to call QF_tick()
static uint16_t counter_qf_tick_interval = 0;
static uint16_t qf_tick_interval = QF_TICK_INTERVAL;


/*
  * Initialize timer0 to use the main crystal clock and the output
  * compare interrupt feature to generate an interrupt approximately
  * once per millisecond to use as a general purpose time base.
  */
static void bsp_init_timer_0(void) {
   TCCR0 = 0;
   TIMSK |= (1<<OCIE0);         /* enable output compare interrupt */
   TCCR0  = (1<<WGM01)|(1<<CS02)|(1<<CS01)|(1<<CS00); /* CTC, prescale = 1024 */
   TCNT0  = 0;
   OCR0   = OCR0_1MS;                     /* match in aprox 1 ms,  */
}


/*!Initializes timer 2, used for the communication bus and the interrupt is caught in bus.c
*/
void bsp_init_timer_2(void) {
	TCCR2 = 0;
	TCNT2 = 0;
	TCCR2 = (1<<WGM21) | (0<<WGM20) | (0<<CS22) | (1<<CS21) | (1<<CS20); //Normal operation, toggle on compare, prescale clk/64	
	TIFR |= (1<<OCF2);
	OCR2 = 30;	//Will trigger an interrupt each with an interval of 130us
	TIMSK |= (1<<OCIE2);
}


/*!Set the direction of the ports
*/
static void bsp_init_ports(void) {
	DDRA = 0xFF;
	DDRB = 0x07;
	DDRC = 0xFF;
	DDRD = 0xFB;
	DDRE = 0x00;
	DDRF = 0x00;
	PORTF = 0x3f;	/* Enable pullups on TX_ACTIVE pins */
	DDRG = 0x03;
	
	PORTD |= (1<<2);
	PORTD |= (1<<3);
}


uint8_t bsp_get_tx_active(void) {
	return PINF & 0x3f;
}

/*
static void get_pa_alarm(void) {
	uint8_t portb = PORTB;
	uint8_t porte = PORTE;
	
	pa_alarm[BAND_160M] = ((porte & ~_BV(7)) != 0);
	pa_alarm[BAND_80M] = ((porte & ~_BV(6)) != 0);
	pa_alarm[BAND_40M] = ((portb & ~_BV(7)) != 0);
	pa_alarm[BAND_20M] = ((portb & ~_BV(6)) != 0);
	pa_alarm[BAND_15M] = ((portb & ~_BV(5)) != 0);
	pa_alarm[BAND_10M] = ((portb & ~_BV(4)) != 0);
}
*/

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
	
	if (on)
	{
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
	
	if (on)
	{
		PORTD |= pdbv;
		PORTG |= pgbv;
	}
	else {
		PORTD &= ~pdbv;
		PORTG &= ~pgbv;
	}
}


uint8_t bsp_mains_is_on(unsigned char band)
{
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
	
	if (on)
	{
		PORTA |= bv;
	}
	else {
		PORTA &= ~bv;
	}
}





void QF_onStartup(void) {
  QF_INT_DISABLE();
	delay_ms(250);
	bsp_init_ports();
	delay_ms(250);

	bus_handler_init();
	
	//Timer which interrupts each ms
	bsp_init_timer_0();
		
  QF_INT_ENABLE();
}


void QF_onIdle(void) {        /* entered with interrupts LOCKED, see NOTE01 */
  QF_INT_ENABLE();
  bus_handler_poll();
}


void Q_onAssert(char const Q_ROM * const Q_ROM_VAR file, int line) {
    //(void)file;                                   /* avoid compiler warning */
    //(void)line;                                   /* avoid compiler warning */
    qf_tick_interval = 0; /* Disable calls to QF_tick() */
    
    char const Q_ROM * Q_ROM_VAR s = file;
    char filename[256];
    char *ptr = filename;
    while ((*ptr++ = Q_ROM_BYTE(*s++)) != 0) {}
    *ptr = '\0';
    
    //QF_INT_DISABLE();
    counter_qf_tick_interval = ASSERT_TX_INTERVAL;
    for (;;) {
      bus_handler_poll_core();
      if (counter_qf_tick_interval >= ASSERT_TX_INTERVAL) {
        for (int i=0; i<6; ++i) {
          uint8_t msg[] = { i, 0 };
          bus_add_tx_message(bus_get_address(),
                             BUS_BROADCAST_ADDR,
                             0,
                             BUS_CMD_AMPLIFIER_ERROR,
                             sizeof(msg),
                             msg);
        }
        send_ascii_data(0, "ASSERT[%s:%d]\r\n", filename, line);
        counter_qf_tick_interval = 0;
      }
    }
}


/*! \brief Output compare 0 interrupt - "called" with 1ms intervals*/
ISR(SIG_OUTPUT_COMPARE0) {
	bus_handler_tick();
  if (++counter_qf_tick_interval == qf_tick_interval) {
    QF_tick();
    counter_qf_tick_interval = 0;
  }
}

