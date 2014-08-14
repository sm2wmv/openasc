#include <stdio.h>
#include <stdlib.h>
#include <avr/io.h>
#include <avr/interrupt.h>

#include "ads1115.h"
#include "i2c.h"

//AIN0, 4.096v, single shot, no comparator 
#define ADS1115_CONF_CH0 ((1<<15) | (1<<14) | (0<<13) | (0<<12) | (0<<11) | (0<<10) | (1<<9) | (1<<8) | (1<<7) | (1<<6) | (1<<5) | (0<<3) | (1<<2) | (1<<1) | (1<<0))
//AIN1, 4.096v, single shot, no comparator
#define ADS1115_CONF_CH1 ((1<<15) | (1<<14) | (0<<13) | (1<<12) | (0<<11) | (0<<10) | (1<<9) | (1<<8) | (1<<7) | (1<<6) | (1<<5) | (0<<3) | (1<<2) | (1<<1) | (1<<0))

unsigned char retval[2];

void ads1115_init(void) {
	unsigned char bytes[3];
	
  bytes[0] = 0x01;
  bytes[1] = (ADS1115_CONF_CH0 >> 8);
  bytes[2] = (ADS1115_CONF_CH0 & 0xFF);

  i2cMasterSend(ADS1115_ADDR,3,bytes);

	bytes[0] = 0x00;
  i2cMasterSend(ADS1115_ADDR,1,bytes);
}

unsigned int ads1115_read_ch0(void) {
  unsigned char bytes[3];

	bytes[0] = 0x00;
  i2cMasterSend(ADS1115_ADDR,1,bytes);
	
  i2cMasterReceive(ADS1115_ADDR,2,(char *)retval);
	
  bytes[0] = 0x01;
  bytes[1] = (ADS1115_CONF_CH1 >> 8);
  bytes[2] = (ADS1115_CONF_CH1 & 0xFF);
  
  i2cMasterSend(ADS1115_ADDR,3,bytes);

  return((retval[0]<<8) + retval[1]);
}

unsigned int ads1115_read_ch1(void) {
  unsigned char bytes[3];

  bytes[0] = 0x00;
  i2cMasterSend(ADS1115_ADDR,1,bytes);
		
  i2cMasterReceive(ADS1115_ADDR, 2, (char *)retval);

  bytes[0] = 0x01;
  bytes[1] = (ADS1115_CONF_CH0 >> 8);
  bytes[2] = (ADS1115_CONF_CH0 & 0xFF);
  
  i2cMasterSend(ADS1115_ADDR,3,bytes);

  return((retval[0]<<8) + retval[1]);  
}