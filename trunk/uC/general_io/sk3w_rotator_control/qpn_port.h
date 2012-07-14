#ifndef qpn_port_h
#define qpn_port_h

//! Setup macros for reading data from program memory
#define Q_ROM                   PROGMEM
#define Q_ROM_BYTE(rom_var_)    pgm_read_byte_near(&(rom_var_))
#define Q_ROM_PTR(rom_var_)     pgm_read_word_near(&(rom_var_))
#ifdef __GNUC__
  #define Q_ROM_NOT_GNUC
#else
  #define Q_ROM_NOT_GNUC          Q_ROM
#endif

//! Disable the simple Finite State Machine code to reduce size
#define Q_NFSM

//! Size of the signal (event) parameter in bytes
#define Q_PARAM_SIZE            2

//! The size in bytes of the variable that holds the timer event counter.
#define QF_TIMEEVT_CTR_SIZE     2

//! Maximum # active objects--must match EXACTLY the QF_active[] definition
#define QF_MAX_ACTIVE           5

//! Macros for enabling/disabling interrupts
#define QF_INT_DISABLE()        cli()
#define QF_INT_ENABLE()         sei()

                            /* interrupt locking policy for interrupt level */
/* #define QF_ISR_NEST */                    /* nesting of ISRs not allowed */

#include <avr/io.h>
#include <avr/interrupt.h>                                   /* cli()/sei() */
#include <avr/pgmspace.h> /* accessing data in the program memory (PROGMEM) */

#include <stdint.h>    /* Exact-width integer types. WG14/N843 C99 Standard */
#include <qepn.h>         /* QEP-nano platform-independent public interface */
#include <qfn.h>           /* QF-nano platform-independent public interface */

#endif                                                        /* qpn_port_h */
