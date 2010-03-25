#ifndef _MAIN_H_
#define _MAIN_H_

/*! PowerMeter message structure
 *  ------------------------------------------------------FWD_PWR-----REF_PWR------VSWR---------
 *  |0xFE|0xFE|fm_addr|to_addr|chksum|flags|0xA0|7|TYPE|byte0|byte1|byte0|byte1|byte0|byte1|0xFD
 *  --------------------------------------------------------------------------------------------
 *  TYPE is a specification what type of pickup it is, what the power limits etc is.
 *
 *  VSWR is represented by an int, for example 311 means 3.11:1, 121 means 1.21:1 etc.
 *
 *  FWD_PWR and REF_PWR is represented by an int as well. So for example 1245 would mean 1245 watts.
 *
 *  The PowerMeter does broadcast these messages at a certain interval which is configured in the pickup. That way
 *  we can read whatever PowerMeters values on the bus just by changing which address we wish to listen to. The type variable
 *  is meant to be used for auto-scaling the power meter limits.
 */

//! The size of the RX queue in buffers
#define BUS_RX_QUEUE_SIZE 10
//! The size of the TX queue in buffers
#define BUS_TX_QUEUE_SIZE	10

#endif