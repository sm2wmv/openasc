#ifndef _BOARD_H_
#define _BOARD_H_

/* This is the processor pinout and what is connected to each pin 
 * =====================================================================================
 *
 * JP1 header pinout (ISP header)
 * =====================================================================================
 * This header can be used either to program the device (pin compatible with AVR ISP)
 * or you can use it to communicate with other hardware devices via SPI. I would suggest
 * that you use PB4 in that case as the chip select pin.
 *
 *  -----
 * o|1 2|
 *  |3 4|
 *  |5 6|
 *  -----
 *
 *  Pin 1 = SPI MISO
 *  Pin 2 = NC
 *  Pin 3 = SPI SCK
 *  Pin 4 = SPI MOSI
 *  Pin 5 = uC RESET
 *  Pin 6 = GND
 * =====================================================================================
 *
 * 
 * PORTA
 * =====================================================================================
 * PA0	-	Output - LED Yellow
 * PA1	-	Output - LED Red
 * PA2	-	Input  - 
 * PA3	-	Input  -
 * PA4	-	Input  - TRANSFER RELAY, R2A1
 * PA5	-	Input  - TRANSFER RELAY, R1A1
 * PA6	-	Input  - Relay K12
 * PA7	-	Input  - Relay K14
 * 
 * =====================================================================================
 *
 *
 * PORTB
 * =====================================================================================
 * PB0	-	Output - Relay K4
 * PB1	-	Input  - SCK
 * PB2	-	Input  - VIP S3 (Mounted) (MUTE ON TX)
 * PB3	-	Input  - VIP S2 (Mounted)
 * PB4	-	Input  - VIP S1 (Not mounted)
 * PB5	-	Input  - VIP S4 (Not mounted)
 * PB6	-	Input  - 
 * PB7	-	Output - PTT R1 AMP
 * =====================================================================================
 *
 *
 * PORTC
 * =====================================================================================
 * PC0	-	Input  -
 * PC1	-	Input  -
 * PC2	-	Input  -
 * PC3	-	Input  -
 * PC4	-	Input  -
 * PC5	-	Input  -
 * PC6	-	Input  -
 * PC7	-	Input  -
 * =====================================================================================
 *
 *
 * PORTD
 * =====================================================================================
 * PD0	-	Input  - 
 * PD1	-	Input  -
 * PD2	-	Input  - USART RXD
 * PD3	-	Output - USART TXD
 * PD4	-	Input  - R1 TX/RX ant selection
 * PD5	-	Input  - R1 Priority SW POS 2
 * PD6  - Input  - R1 Priority SW POS 1
 * PD7	-	Input  - R1 Footswich
 * =====================================================================================
 *
 *
 * PORTE
 * =====================================================================================
 * PE0	-	Input  - MOSI (ISP)
 * PE1	-	Input  - MISO (ISP)
 * PE2	-	Output - Relay K13
 * PE3	-	Output - Relay K10
 * PE4	-	Output - Relay K11
 * PE5	-	Output - Relay K9
 * PE6	-	Output - Relay K8
 * PE7	-	Output - Relay K5K6
 * ===================================================================================== *
 *
 *
 * PORTF
 * =====================================================================================
 * PF0	-	Output - PTT R2 AMP
 * PF1	-	Output - PTT R2 Radio
 * PF2	-	Input  - R2 RX/TX ant sel
 * PF3	-	Input  - R2 Mult/Support switch
 * PF4	-	Input  - R2 Footswitch
 * PF5	-	Output - R2 PTT LED R2
 * PF6	- Output - R2 PTT LED R1
 * PF7	-	Input  - 
 * ===================================================================================== * *
 *
 *
 * PORTG
 * =====================================================================================
 * PG0	- Output - R1 PTT LED R2
 * PG1	- Output - R1 PTT LED R1
 * PG2	- Input  -
 * PG3	-	Output - R1 PTT RADIO
 * PG4	-	Input  -
 * ===================================================================================== * *
 */
 
#endif

#define VIP_S3_BIT  0
#define VIP_S2_BIT  1
#define VIP_S1_BIT  2
#define VIP_S4_BIT  3