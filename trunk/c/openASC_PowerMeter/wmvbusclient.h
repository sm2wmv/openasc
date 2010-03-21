#ifndef WMVBUSCLIENT_H
#define WMVBUSCLIENT_H

#include <QObject>
#include <QQueue>
#include <QThread>
#include <QString>

#include "qextserialport.h"
//#include "mainwindow.h"
#include "ui_mainwindow.h"

/******* BUS specifics ********/

/*! The preamble of the BUS message */
#define BUS_MSG_PREAMBLE		0xFE
/*! The postamble of the BUS message */
#define BUS_MSG_POSTAMBLE		0xFD
/*! The acknowledge command of the bus */
#define BUS_MSG_ACK					0xFA
/*! The NOT acknowledge command of the bus */
#define BUS_MSG_NACK				0xFB

/*! Bus broadcast address - All broadcast messages should contain an ID explaining which kind of
		device that is sending the message */
#define BUS_BROADCAST_ADDR	0x00

//! Device ID for the mainbox
#define DEVICE_ID_MAINBOX						1
//! Device ID for the positive driver module
#define DEVICE_ID_DRIVER_POS				2
//! Device ID for the negative driver module
#define DEVICE_ID_DRIVER_NEG				3
//! Device ID for the rotator unit
#define DEVICE_ID_ROTATOR_UNIT			4
//! Device ID for a computer
#define DEVICE_ID_COMPUTER					5
//! Device ID for a power meter pickup
#define DEVICE_ID_POWERMETER_PICKUP	6
//! Device ID for a power meter
#define DEVICE_ID_POWERMETER				7

/*! The interval between each status message (time is in ms) */
#define BUS_DEVICE_STATUS_MESSAGE_INTERVAL	1500

/*! The number of times a message is resent before it's dropped and an error flag is set */
#define BUS_MAX_RESENDS	10

/*! Timeout limit for how long it can take without receiving a message before the buffer is cleared, this is counted as
		time, 5 would mean 5 * 130 us */
#define BUS_TIMEOUT_LIMIT 5

/*! This limit is used to detect if it was too long ago since we receieved a SYNC message
 *  from the master. If so it will stop with all outgoing communication. */
#define BUS_SYNC_TIMEOUT_LIMIT 10000

/*! The timeout limit between a message that was sent to when it will be a resend, this is counted as
		number of wraparounds on the bus, ie 5 would mean 5 wraparounds */
#define BUS_ACK_WRAPAROUND_LIMIT 10

/*! This bit is to set if the device is MASTER in the bus_status.flags variable */
#define BUS_STATUS_DEVICE_IS_MASTER_BIT	0
/*! This bit shows if it's allowed to send messages on the bus, ie first sync has been received */
#define BUS_STATUS_ALLOWED_TO_SEND_BIT	1
/*! This bit is set if the preamble is found in the bus_status.flags variable */
#define BUS_STATUS_PREAMBLE_FOUND_BIT		2
/*! This bit is to set if the MASTER has sent a SYNC message so we are allowed to start sending */
#define BUS_STATUS_MASTER_SENT_SYNC_BIT	3
/*! This bit is to set if the device own time slot is currently active, ie it is possibly allowed to send */
#define BUS_STATUS_TIME_SLOT_ACTIVE			4
/*! This bit is to indicate that we should try to send the message currently in the TX queue */
#define BUS_STATUS_SEND_MESSAGE					5
/*! This bit is to indicate that we are CURRENTLY sending a message */
#define BUS_STATUS_SEND_ACTIVE					6
/*! This bit is to indicate that we are CURRENTLY receiving a message */
#define BUS_STATUS_RECEIVE_ON						7
/*! This bit is to force a SYNC message to be sent */
#define BUS_STATUS_FORCE_SYNC						8
/*! This bit is to see if a message should be acked or not, used for the timeout of an acknowledge */
#define BUS_STATUS_MESSAGE_ACK_TIMEOUT	9

/*! This is the multiplier for the send window
		BUS_TIME_INTERRUPT_INTERVAL * BUS_TIME_MULTIPLIER = BUS_TIME_FRAME_LIMIT */
#define BUS_TIME_MULTIPLIER 8
/*! The interval of the timer interrupts, in us */
#define BUS_TIME_INTERRUPT_INTERVAL 130
/*! The time frame size of the bus time slots, in us */
#define BUS_TIME_FRAME_LIMIT	1040
/*! The dead time of the time slot, in us. This should be set pretty high to accept rather high clock drift */
#define BUS_SLOT_DEAD_TIME		260

//! Define the length of the bus message data field length
#define BUS_MESSAGE_DATA_SIZE 15

//! The message should be ACKED
#define BUS_MESSAGE_FLAGS_NEED_ACK	0

/*! BUS message structure */
typedef struct {
	//! From which address the bus message was sent
	unsigned char from_addr;
	//! To which address the bus message should be sent
	unsigned char to_addr;
	//! The checksum of the message
	unsigned char checksum;
	//! Different flags
	unsigned char flags;
	//! The command of the message
	unsigned char cmd;
	//! The length of the data sent in the message
	unsigned char length;
	//! The variables wanted to be sent
	unsigned char data[BUS_MESSAGE_DATA_SIZE];
} BUS_MESSAGE;

typedef struct {
		/*! See details in defines */
		unsigned int flags;
		//!Counter which keeps track of which frame (slot) we are in
		unsigned char frame_counter;
		//!The lower limit of the time slot
		unsigned int lower_limit;
		//!The upper limit of the time slot
		unsigned int upper_limit;
		//!The number of devices on the bus
		unsigned char device_count;
		//!The number of times the last message was sent
		unsigned char send_count;
		//! The number of wrap arounds
		unsigned char wraparounds;
		//!Nr of devices are x 4 so we don't need to do the division
		unsigned int device_count_mult;
} bus_status_struct;

class WMVBusClient : public QThread
{
public:
    WMVBusClient();
		void initClient(unsigned char addr, QString dev);
		unsigned char getBusAddress();
		void addTxMessage(unsigned char fromAddr, unsigned char toAddr, unsigned char flags, unsigned char cmd, unsigned char length, unsigned char data[]);
		void stopProcess();
		void receiveMsg();
		int messageInRXQueue();
		BUS_MESSAGE getMessageInRXQueue();
		void run();
private:
		 QQueue<BUS_MESSAGE> txQueue;
		 QQueue<BUS_MESSAGE> rxQueue;
		/*! \brief Function to be called when a message is recieved and should be parsed/executed */
		void sendNack(unsigned char toAddr);
		void sendAck(unsigned char toAddr);
		void addRxMessage(unsigned char fromAddr, unsigned char toAddr, unsigned char flags, unsigned char cmd, unsigned char length, unsigned char data[]);
		/*! \brief The address of the client on the bus */
		unsigned char busAddr;		
		QString deviceName;
		int openPort(QString deviceName);
		int closePort();
		void sendMessage(BUS_MESSAGE message);
protected:
		void printDebugMessage(BUS_MESSAGE message);
		bool threadActive;
		QextSerialPort *serialPort;
		QByteArray receivedMessage;
		bus_status_struct busStatus;
};

#endif // WMVBUSCLIENT_H
