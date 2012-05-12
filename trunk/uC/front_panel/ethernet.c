/*! \file front_panel/ethernet.c
 *  \brief Ethernet controller
 *  \ingroup front_panel_group
 *  \author Mikael Larsmark, SM2WMV
 *  \date 2012-05-10
 *  \code #include "front_panel/ethernet.c" \endcode
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
//
//    Note that I am very happy to have read the blog http://www.ermicro.com/blog/?p=1773 which helped me a lot
//    getting started with the WIZNET chip

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <avr/io.h>
#include <avr/eeprom.h>

//#define ETHERNET_DEBUG_ENABLED

#include "ethernet.h"
#include "main.h"
#include "../delay.h"

static unsigned char ethernet_chip_enabled;

#define SPI_PORT PORTB
#define SPI_DDR  DDRB
#define SPI_CS   4

// Wiznet W5100 Op Code
#define WIZNET_WRITE_OPCODE 0xF0
#define WIZNET_READ_OPCODE 0x0F

// Wiznet W5100 Register Addresses
#define MR         0x0000      // Mode Register
#define GAR        0x0001      // Gateway Address: 0x0001 to 0x0004
#define SUBR       0x0005      // Subnet mask Address: 0x0005 to 0x0008
#define SAR        0x0009      // Source Hardware Address (MAC): 0x0009 to 0x000E
#define SIPR       0x000F      // Source IP Address: 0x000F to 0x0012
#define RMSR       0x001A      // RX Memory Size Register
#define TMSR       0x001B      // TX Memory Size Register
#define S0_MR      0x0400      // Socket 0: Mode Register Address
#define S0_CR      0x0401      // Socket 0: Command Register Address
#define S0_IR      0x0402      // Socket 0: Interrupt Register Address
#define S0_SR      0x0403      // Socket 0: Status Register Address
#define S0_PORT    0x0404      // Socket 0: Source Port: 0x0404 to 0x0405
#define SO_TX_FSR  0x0420      // Socket 0: Tx Free Size Register: 0x0420 to 0x0421
#define S0_TX_RD   0x0422      // Socket 0: Tx Read Pointer Register: 0x0422 to 0x0423
#define S0_TX_WR   0x0424      // Socket 0: Tx Write Pointer Register: 0x0424 to 0x0425
#define S0_RX_RSR  0x0426      // Socket 0: Rx Received Size Pointer Register: 0x0425 to 0x0427
#define S0_RX_RD   0x0428      // Socket 0: Rx Read Pointer: 0x0428 to 0x0429
#define TXBUFADDR  0x4000      // W5100 Send Buffer Base Address
#define RXBUFADDR  0x6000      // W5100 Read Buffer Base Address
// S0_MR values
#define MR_CLOSE    0x00    // Unused socket
#define MR_TCP      0x01    // TCP
#define MR_UDP      0x02    // UDP
#define MR_IPRAW    0x03    // IP LAYER RAW SOCK
#define MR_MACRAW   0x04    // MAC LAYER RAW SOCK
#define MR_PPPOE    0x05    // PPPoE
#define MR_ND     0x20    // No Delayed Ack(TCP) flag
#define MR_MULTI    0x80    // support multicating
// S0_CR values
#define CR_OPEN          0x01   // Initialize or open socket
#define CR_LISTEN        0x02   // Wait connection request in tcp mode(Server mode)
#define CR_CONNECT       0x04   // Send connection request in tcp mode(Client mode)
#define CR_DISCON        0x08   // Send closing reqeuset in tcp mode
#define CR_CLOSE         0x10   // Close socket
#define CR_SEND          0x20   // Update Tx memory pointer and send data
#define CR_SEND_MAC      0x21   // Send data with MAC address, so without ARP process
#define CR_SEND_KEEP     0x22   // Send keep alive message
#define CR_RECV          0x40   // Update Rx memory buffer pointer and receive data
// S0_SR values
#define SOCK_CLOSED      0x00     // Closed
#define SOCK_INIT        0x13   // Init state
#define SOCK_LISTEN      0x14   // Listen state
#define SOCK_SYNSENT     0x15   // Connection state
#define SOCK_SYNRECV     0x16   // Connection state
#define SOCK_ESTABLISHED 0x17   // Success to connect
#define SOCK_FIN_WAIT    0x18   // Closing state
#define SOCK_CLOSING     0x1A   // Closing state
#define SOCK_TIME_WAIT   0x1B   // Closing state
#define SOCK_CLOSE_WAIT  0x1C   // Closing state
#define SOCK_LAST_ACK    0x1D   // Closing state
#define SOCK_UDP         0x22   // UDP socket
#define SOCK_IPRAW       0x32   // IP raw mode socket
#define SOCK_MACRAW      0x42   // MAC raw mode socket
#define SOCK_PPPOE       0x5F   // PPPOE socket
#define TX_BUF_MASK      0x07FF   // Tx 2K Buffer Mask:
#define RX_BUF_MASK      0x07FF   // Rx 2K Buffer Mask:
#define NET_MEMALLOC     0x05     // Use 2K of Tx/Rx Buffer

#define TCP_PORT         4475       // TCP/IP Port

#define MAX_BUF 128

unsigned char ethernet_buf[MAX_BUF];

static unsigned char sockstat = 0;
static unsigned char last_sockstat = 1;
static unsigned char sockreg = 0;
static unsigned char rx_msg_size=0;

void ethernet_spi_write(unsigned int addr,unsigned char data)
{
  // Activate the CS pin
  PORTG &= ~(1<<SPI_CS);
  // Start Wiznet W5100 Write OpCode transmission
  SPDR = WIZNET_WRITE_OPCODE;
  // Wait for transmission complete
  while(!(SPSR & (1<<SPIF)));
  // Start Wiznet W5100 Address High Bytes transmission
  SPDR = (addr & 0xFF00) >> 8;
  // Wait for transmission complete
  while(!(SPSR & (1<<SPIF)));
  // Start Wiznet W5100 Address Low Bytes transmission
  SPDR = addr & 0x00FF;
  // Wait for transmission complete
  while(!(SPSR & (1<<SPIF)));   

  // Start Data transmission
  SPDR = data;
  // Wait for transmission complete
  while(!(SPSR & (1<<SPIF)));
  // CS pin is not active
  PORTG |= (1<<SPI_CS);
}
unsigned char ethernet_spi_read(unsigned int addr)
{
  // Activate the CS pin
  PORTG &= ~(1<<SPI_CS);
  // Start Wiznet W5100 Read OpCode transmission
  SPDR = WIZNET_READ_OPCODE;
  // Wait for transmission complete
  while(!(SPSR & (1<<SPIF)));
  // Start Wiznet W5100 Address High Bytes transmission
  SPDR = (addr & 0xFF00) >> 8;
  // Wait for transmission complete
  while(!(SPSR & (1<<SPIF)));
  // Start Wiznet W5100 Address Low Bytes transmission
  SPDR = addr & 0x00FF;
  // Wait for transmission complete
  while(!(SPSR & (1<<SPIF)));   

  // Send Dummy transmission for reading the data
  SPDR = 0x00;
  // Wait for transmission complete
  while(!(SPSR & (1<<SPIF)));  

  // CS pin is not active
  PORTG |= (1<<SPI_CS);
  return(SPDR);
}

void ethernet_init(void) {
  PORTG |= (1<<SPI_CS);
    
  // Enable SPI, Master Mode 0, set the clock rate fck/2
  SPCR = (1<<SPE)|(1<<MSTR);
  SPSR |= (1<<SPI2X);
  
  #ifdef ETHERNET_DEBUG_ENABLED
    printf("Ethernet: SPI enabled\n");
  #endif
    
  // Ethernet Setup
  unsigned char mac_addr[] = {0x00,0x16,0x36,0xDE,0x58,0xF6};
  unsigned char ip_addr[] = {77,110,45,196};
  unsigned char sub_mask[] = {255,255,255,0};
  unsigned char gtw_addr[] = {192,168,2,1};
  
  // Setting the Wiznet W5100 Mode Register: 0x0000
  ethernet_spi_write(MR,0x80);            // MR = 0b10000000;
  delay_ms(1);
  #ifdef ETHERNET_DEBUG_ENABLED
    printf("ETH >> Reading MR: %d\n\n",ethernet_spi_read(MR));
    // Setting the Wiznet W5100 Gateway Address (GAR): 0x0001 to 0x0004
    printf("ETH >> Setting Gateway Address %d.%d.%d.%d\n",gtw_addr[0],gtw_addr[1],gtw_addr[2],gtw_addr[3]);
  #endif
  ethernet_spi_write(GAR + 0,gtw_addr[0]);
  ethernet_spi_write(GAR + 1,gtw_addr[1]);
  ethernet_spi_write(GAR + 2,gtw_addr[2]);
  ethernet_spi_write(GAR + 3,gtw_addr[3]);
  delay_ms(1);
  
  if ((ethernet_spi_read(GAR + 0) != gtw_addr[0])) {
    ethernet_chip_enabled = 0;
    return;
  }
  

  
  #ifdef ETHERNET_DEBUG_ENABLED
    printf("ETH >> Reading GAR: %d.%d.%d.%d\n\n",ethernet_spi_read(GAR + 0),ethernet_spi_read(GAR + 1),ethernet_spi_read(GAR + 2),ethernet_spi_read(GAR + 3));
  // Setting the Wiznet W5100 Source Address Register (SAR): 0x0009 to 0x000E
    printf("ETH >> Setting Source Address %.2x:%.2x:%.2x:%.2x:%.2x:%.2x\n",mac_addr[0],mac_addr[1],mac_addr[2],mac_addr[3],mac_addr[4],mac_addr[5]);
  #endif
  ethernet_spi_write(SAR + 0,mac_addr[0]);
  ethernet_spi_write(SAR + 1,mac_addr[1]);
  ethernet_spi_write(SAR + 2,mac_addr[2]);
  ethernet_spi_write(SAR + 3,mac_addr[3]);
  ethernet_spi_write(SAR + 4,mac_addr[4]);
  ethernet_spi_write(SAR + 5,mac_addr[5]);
  delay_ms(1);
  #ifdef ETHERNET_DEBUG_ENABLED
    printf("ETH >> Reading SAR: %.2x:%.2x:%.2x:%.2x:%.2x:%.2x\n\n",ethernet_spi_read(SAR + 0),ethernet_spi_read(SAR + 1),ethernet_spi_read(SAR + 2),ethernet_spi_read(SAR + 3),ethernet_spi_read(SAR + 4),ethernet_spi_read(SAR + 5));
    // Setting the Wiznet W5100 Sub Mask Address (SUBR): 0x0005 to 0x0008
    printf("ETH >> Setting Sub Mask Address %d.%d.%d.%d\n",sub_mask[0],sub_mask[1],sub_mask[2],sub_mask[3]);
  #endif
  ethernet_spi_write(SUBR + 0,sub_mask[0]);
  ethernet_spi_write(SUBR + 1,sub_mask[1]);
  ethernet_spi_write(SUBR + 2,sub_mask[2]);
  ethernet_spi_write(SUBR + 3,sub_mask[3]);
  delay_ms(1);
  #ifdef ETHERNET_DEBUG_ENABLED
    printf("ETH >> Reading SUBR: %d.%d.%d.%d\n\n",ethernet_spi_read(SUBR + 0),ethernet_spi_read(SUBR + 1),ethernet_spi_read(SUBR + 2),ethernet_spi_read(SUBR + 3));
    // Setting the Wiznet W5100 IP Address (SIPR): 0x000F to 0x0012
    printf("ETH >> Setting IP Address %d.%d.%d.%d\n",ip_addr[0],ip_addr[1],ip_addr[2],ip_addr[3]);
  #endif
  ethernet_spi_write(SIPR + 0,ip_addr[0]);
  ethernet_spi_write(SIPR + 1,ip_addr[1]);
  ethernet_spi_write(SIPR + 2,ip_addr[2]);
  ethernet_spi_write(SIPR + 3,ip_addr[3]);
  delay_ms(1);
  #ifdef ETHERNET_DEBUG_ENABLED
    printf("ETH >> Reading SIPR: %d.%d.%d.%d\n\n",ethernet_spi_read(SIPR + 0),ethernet_spi_read(SIPR + 1),ethernet_spi_read(SIPR + 2),ethernet_spi_read(SIPR + 3));

    // Setting the Wiznet W5100 RX and TX Memory Size, we use 2KB for Rx/Tx 4 channels
    printf("ETH >> Setting Wiznet RMSR and TMSR\n\n");
  #endif
  ethernet_spi_write(RMSR,NET_MEMALLOC);
  ethernet_spi_write(TMSR,NET_MEMALLOC);
  #ifdef ETHERNET_DEBUG_ENABLED
    printf("ETH >> Done Wiznet W5100 Initialized!\n");
  #endif
    
  #ifdef ETHERNET_DEBUG_ENABLED
    printf("ETH >> Created TCP socket on port %i\n",TCP_PORT);
  #endif

  ethernet_chip_enabled = 1;
}

void ethernet_close(uint8_t sock) {
  if (sock != 0) 
    return;

  // Send Close Command
  ethernet_spi_write(S0_CR,CR_CLOSE);
  // Waiting until the S0_CR is clear
  while(ethernet_spi_read(S0_CR));
}

void ethernet_disconnect(uint8_t sock) {
   if (sock != 0) 
     return;
   
   // Send Disconnect Command
   ethernet_spi_write(S0_CR,CR_DISCON);
   // Wait for Disconecting Process
   while(ethernet_spi_read(S0_CR));
}

unsigned char ethernet_socket(unsigned char sock, unsigned char protocol, unsigned int tcp_port) {
  unsigned char retval = 0;
  
  if (sock != 0)
    return(retval);
  
  if (ethernet_spi_read(S0_SR) == SOCK_CLOSED)
    ethernet_close(sock);
  
  ethernet_spi_write(S0_MR,protocol);
  ethernet_spi_write(S0_PORT,((tcp_port & 0xFF00) >> 8 ));
  ethernet_spi_write(S0_PORT + 1,(tcp_port & 0x00FF));
  ethernet_spi_write(S0_CR,CR_OPEN);                   // Open Socket
  
  // Wait for Opening Process
  while(ethernet_spi_read(S0_CR));
  
  // Check for Init Status
  if (ethernet_spi_read(S0_SR) == SOCK_INIT)
    retval=1;
  else
    ethernet_close(sock);

  return retval;
}

unsigned char ethernet_listen(unsigned char sock) {
  uint8_t retval = 0;
  if (sock != 0) 
    return retval;
  
  if (ethernet_spi_read(S0_SR) == SOCK_INIT) {
    // Send the LISTEN Command
    ethernet_spi_write(S0_CR,CR_LISTEN);

    // Wait for Listening Process
    while(ethernet_spi_read(S0_CR));
    // Check for Listen Status
    if (ethernet_spi_read(S0_SR) == SOCK_LISTEN)
      retval=1;
    else
      ethernet_close(sock);
  }
  return retval;
}

unsigned int ethernet_send_display_data(unsigned char sock, unsigned char *buf, unsigned int buflen) {
  unsigned char preamble[3] = {0xFE,0xFE,buflen};
  
  unsigned int ptr,offaddr,realaddr,txsize,timeout;   

  if (buflen <= 0 || sock != 0) 
    return 0;

  // Make sure the TX Free Size Register is available
  txsize=ethernet_spi_read(SO_TX_FSR);
  txsize=(((txsize & 0x00FF) << 8 ) + ethernet_spi_read(SO_TX_FSR + 1));

  timeout=0;
  while (txsize < (buflen+3)) {
    txsize=ethernet_spi_read(SO_TX_FSR);
    txsize=(((txsize & 0x00FF) << 8 ) + ethernet_spi_read(SO_TX_FSR + 1));
    // Timeout for approx 1000 ms
/*    if (timeout++ > 1000) {
      // Disconnect the connection
      ethernet_disconnect(sock);
      return 0;
    }*/
  }  

  // Read the Tx Write Pointer
  ptr = ethernet_spi_read(S0_TX_WR);
  offaddr = (((ptr & 0x00FF) << 8 ) + ethernet_spi_read(S0_TX_WR + 1));

  unsigned char i=0;
  
  while(i<3) {
    // Calculate the real W5100 physical Tx Buffer Address
    realaddr = TXBUFADDR + (offaddr & TX_BUF_MASK);
    // Copy the application data to the W5100 Tx Buffer
    ethernet_spi_write(realaddr,preamble[i]);
    offaddr++;
    i++;
  }
  
  while(buflen) {
    buflen--;
    // Calculate the real W5100 physical Tx Buffer Address
    realaddr = TXBUFADDR + (offaddr & TX_BUF_MASK);
    // Copy the application data to the W5100 Tx Buffer
    ethernet_spi_write(realaddr,*buf);
    offaddr++;
    buf++;
  }

  //Send postamble
  realaddr = TXBUFADDR + (offaddr & TX_BUF_MASK);
  ethernet_spi_write(realaddr,0xFD);
  offaddr++;
  
  // Increase the S0_TX_WR value, so it point to the next transmit
  ethernet_spi_write(S0_TX_WR,(offaddr & 0xFF00) >> 8 );
  ethernet_spi_write(S0_TX_WR + 1,(offaddr & 0x00FF)); 

  // Now Send the SEND command
  ethernet_spi_write(S0_CR,CR_SEND);

  // Wait for Sending Process
  while(ethernet_spi_read(S0_CR)); 

  return 1;
}

unsigned int ethernet_send(unsigned char sock,const unsigned char *buf, unsigned int buflen) {
  unsigned int ptr,offaddr,realaddr,txsize,timeout;   

  if (buflen <= 0 || sock != 0) 
    return 0;

  // Make sure the TX Free Size Register is available
  txsize=ethernet_spi_read(SO_TX_FSR);
  txsize=(((txsize & 0x00FF) << 8 ) + ethernet_spi_read(SO_TX_FSR + 1));

  timeout=0;
  while (txsize < buflen) {
    txsize=ethernet_spi_read(SO_TX_FSR);
    txsize=(((txsize & 0x00FF) << 8 ) + ethernet_spi_read(SO_TX_FSR + 1));
    // Timeout for approx 1000 ms
/*    if (timeout++ > 1000) {
      // Disconnect the connection
      ethernet_disconnect(sock);
      return 0;
    }*/
  }  

  // Read the Tx Write Pointer
  ptr = ethernet_spi_read(S0_TX_WR);
  offaddr = (((ptr & 0x00FF) << 8 ) + ethernet_spi_read(S0_TX_WR + 1));

  while(buflen) {
    buflen--;
    // Calculate the real W5100 physical Tx Buffer Address
    realaddr = TXBUFADDR + (offaddr & TX_BUF_MASK);
    // Copy the application data to the W5100 Tx Buffer
    ethernet_spi_write(realaddr,*buf);
    offaddr++;
    buf++;
  }

  // Increase the S0_TX_WR value, so it point to the next transmit
  ethernet_spi_write(S0_TX_WR,(offaddr & 0xFF00) >> 8 );
  ethernet_spi_write(S0_TX_WR + 1,(offaddr & 0x00FF)); 

  // Now Send the SEND command
  ethernet_spi_write(S0_CR,CR_SEND);

  // Wait for Sending Process
  while(ethernet_spi_read(S0_CR)); 

  return 1;
}

unsigned int ethernet_recv(unsigned char sock,unsigned char *buf,unsigned int buflen)
{
  unsigned int ptr,offaddr,realaddr;    

  if (buflen <= 0 || sock != 0) 
    return 1;   

  // If the request size > MAX_BUF,just truncate it
  if (buflen > MAX_BUF)
    buflen=MAX_BUF - 2;
  // Read the Rx Read Pointer
  ptr = ethernet_spi_read(S0_RX_RD);
  offaddr = (((ptr & 0x00FF) << 8 ) + ethernet_spi_read(S0_RX_RD + 1));

  while(buflen) {
    buflen--;
    realaddr=RXBUFADDR + (offaddr & RX_BUF_MASK);
    *buf = ethernet_spi_read(realaddr);
    offaddr++;
    buf++;
  }
  
  *buf='\0';        // String terminated character

  // Increase the S0_RX_RD value, so it point to the next receive
  ethernet_spi_write(S0_RX_RD,(offaddr & 0xFF00) >> 8 );
  ethernet_spi_write(S0_RX_RD + 1,(offaddr & 0x00FF)); 

  // Now Send the RECV command
  ethernet_spi_write(S0_CR,CR_RECV);
  return 1;
}

unsigned int ethernet_recv_size(void) {
  return ((ethernet_spi_read(S0_RX_RSR) & 0x00FF) << 8 ) + ethernet_spi_read(S0_RX_RSR + 1);
}

void ethernet_process(void) {
  sockstat = ethernet_spi_read(S0_SR);
  
  switch(sockstat) {
    case SOCK_CLOSED:
      if (ethernet_socket(sockreg,MR_TCP,TCP_PORT) > 0)
        if (ethernet_listen(sockreg) <= 0) {
          #ifdef ETHERNET_DEBUG_ENABLED
            printf("ETH >> SOCK LISTEN SUCCESS\n");
          #endif
        }
        #ifdef ETHERNET_DEBUG_ENABLED
          if (last_sockstat != sockstat)
            printf("ETH >> Socket CLOSED\n");
        #endif
      break;
    case SOCK_ESTABLISHED:
      #ifdef ETHERNET_DEBUG_ENABLED
        if (last_sockstat != sockstat)
          printf("ETH >> Socket ESTABLISHED\n");
      #endif
      rx_msg_size = ethernet_recv_size();
          
      if (rx_msg_size > 0) {
        #ifdef ETHERNET_DEBUG_ENABLED
          printf("ETH >> RX SIZE: %i\n", rx_msg_size);
        #endif
            
        ethernet_recv(sockreg,ethernet_buf,rx_msg_size);
        
        //ethernet_send(sockreg,"hahaha",6);
        
        #ifdef ETHERNET_DEBUG_ENABLED
          printf("ETH >> RX MSG: %s\n", ethernet_buf);
        #endif
      }
      break;
    case SOCK_INIT:
      #ifdef ETHERNET_DEBUG_ENABLED
        if (last_sockstat != sockstat)
          printf("ETH >> Socket INIT\n");
      #endif
      break;
    case SOCK_LISTEN:
      #ifdef ETHERNET_DEBUG_ENABLED
        if (last_sockstat != sockstat)
          printf("ETH >> Socket LISTEN\n");
      #endif
      break;
    case SOCK_LAST_ACK:
      #ifdef ETHERNET_DEBUG_ENABLED
        if (last_sockstat != sockstat)
          printf("ETH >> Socket LAST ACK\n");
      #endif
      ethernet_disconnect(sockreg);
      break;
    case SOCK_FIN_WAIT:
      #ifdef ETHERNET_DEBUG_ENABLED
        if (last_sockstat != sockstat)
          printf("ETH >> Socket SOCK_FIN_WAIT\n");
      #endif
      ethernet_disconnect(sockreg);
      break;
    case SOCK_CLOSING:
      #ifdef ETHERNET_DEBUG_ENABLED
        if (last_sockstat != sockstat)
          printf("ETH >> Socket SOCK_CLOSING\n");
      #endif
      ethernet_disconnect(sockreg);
      break;
    case SOCK_TIME_WAIT:
      #ifdef ETHERNET_DEBUG_ENABLED
        if (last_sockstat != sockstat)
          printf("ETH >> Socket SOCK_TIME_WAIT\n");
      #endif
      ethernet_disconnect(sockreg);
      break;
    case SOCK_CLOSE_WAIT:
      #ifdef ETHERNET_DEBUG_ENABLED
        if (last_sockstat != sockstat)
          printf("ETH >> Socket SOCK_CLOSE_WAIT\n");
      #endif
      ethernet_disconnect(sockreg);
      break;
    default:
      break;
  }
  
  last_sockstat = sockstat;
}

unsigned char ethernet_is_active(void) {
  if (ethernet_chip_enabled)
    if (sockstat == SOCK_ESTABLISHED)
      return(1);

  return(0);
}

unsigned char ethernet_get_chip_enabled(void) {
  return(ethernet_chip_enabled);
}
