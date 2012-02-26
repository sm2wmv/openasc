#include <stdio.h>
#include <stdlib.h>
#include <avr/io.h>
#include <avr/interrupt.h>

#include "ads1115.h"
#include "i2c.h"

//AIN0, 2.048v, single shot, no comparator 
#define ADS1115_CONF_CH0 ((0<<15) | (1<<14) | (0<<13) | (0<<12) | (0<<11) | (0<<10) | (1<<9) | (1<<8) | (1<<7) | (0<<6) | (0<<5) | (1<<1) | (1<<0))
//AIN1, 2.048v, single shot, no comparator
#define ADS1115_CONF_CH1 ((0<<15) | (1<<14) | (0<<13) | (1<<12) | (0<<11) | (0<<10) | (1<<9) | (1<<8) | (1<<7) | (0<<6) | (0<<5) | (1<<1) | (1<<0))

unsigned int ads1115_read_ch0(void) {
  unsigned char bytes[3];
  
  bytes[0] = 0x01;
  bytes[1] = (ADS1115_CONF_CH0 >> 8);
  bytes[2] = (ADS1115_CONF_CH0 & 0xFF);
  
  i2cMasterSendNI(ADS1115_ADDR,3,bytes);
  
  bytes[0] = 0x00;
  i2cMasterSendNI(ADS1115_ADDR,1,bytes);
  
  i2cMasterReceiveNI(ADS1115_ADDR, 2, bytes);
  
  return((bytes[0]<<8) + bytes[1]);
}

unsigned int ads1115_read_ch1(void) {
  unsigned char bytes[3];
  
  bytes[0] = 0x01;
  bytes[1] = (ADS1115_CONF_CH1 >> 8);
  bytes[2] = (ADS1115_CONF_CH1 & 0xFF);
  
  i2cMasterSendNI(ADS1115_ADDR,3,bytes);
  
  bytes[0] = 0x00;
  i2cMasterSendNI(ADS1115_ADDR,1,bytes);
  
  i2cMasterReceiveNI(ADS1115_ADDR, 2, bytes);
  
  return((bytes[0]<<8) + bytes[1]);  
}