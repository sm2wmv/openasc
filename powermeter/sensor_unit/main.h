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

//#define CAL_MODE	1

//#define PICKUP_UNIT_ID_1
#define PICKUP_UNIT_ID_2

//! The size of the RX queue in buffers
#define BUS_RX_QUEUE_SIZE 10
//! The size of the TX queue in buffers
#define BUS_TX_QUEUE_SIZE	10

#ifndef CAL_MODE
	//! The transmit interval of the power in ms, used when the meter is considered as active */
	#define POWER_TRANSMIT_INTERVAL1 200
	//! The transmit interval of the power in ms, used when the meter is considered as not active */
	#define POWER_TRANSMIT_INTERVAL2 2000

	//! The interval which the units polls the power */
	#define POWER_POLL_INTERVAL	40
#endif

#ifdef CAL_MODE
	//! The transmit interval of the power in ms, used when the meter is considered as active */
	#define POWER_TRANSMIT_INTERVAL1 1000
	//! The transmit interval of the power in ms, used when the meter is considered as not active */
	#define POWER_TRANSMIT_INTERVAL2 2000

	//! The interval which the units polls the power */
	#define POWER_POLL_INTERVAL	1000
#endif	

#define POWER_LAST_CHANGE_TIME 1000

/*! 150w pickup configuration */
#define PICKUP_TYPE_150W        0
/*! 1000w pickup configuration */
#define PICKUP_TYPE_1000W       1
/*! 1500w pickup configuration */
#define PICKUP_TYPE_1500W       2
/*! 2000w pickup configuration */
#define PICKUP_TYPE_2000W       3
/*! 3000w pickup configuration */
#define PICKUP_TYPE_3000W       4
/*! 5000w pickup configuration */
#define PICKUP_TYPE_5000W       5
/*! 10000w pickup configuration */
#define PICKUP_TYPE_10000W      6
/*! 15000w pickup configuration */
#define PICKUP_TYPE_15000W      7

//! Under this value is considered to be no transmission occuring (in watts)
#define NO_FWD_PWR_LIMIT	10

#endif
