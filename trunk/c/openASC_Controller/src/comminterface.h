#ifndef COMMINTERFACE_H
#define COMMINTERFACE_H

#include <QThread>
#include <QTimer>
#include <QLinkedList>
#include <QString>

#include "qextserialport.h"

//! Timeout limit for the transmission of data, set in ms
#define COMPUTER_COMM_TX_TIMEOUT_LIMIT	100
//! Timeout limit for the receiption of data, set in ms
#define COMPUTER_COMM_RX_TIMEOUT_LIMIT	100

//! The preamble of a serial message
#define COMPUTER_COMM_PREAMBLE	0xFE
//! The postamble of a serial message
#define COMPUTER_COMM_POSTAMBLE	0xFD
//! The serial acknowledge of the computer communication protocol
#define COMPUTER_COMM_ACK				0xFA
//! The serial NOT acknowledge of the computer communication protocol
#define COMPUTER_COMM_NACK			0xFB

#define COMM_MESSAGE_DATA_LENGTH	20

typedef struct {
	//! The checksum of the message
	unsigned char checksum;
	//! The command of the comm interface message
	unsigned char command;
	//! The length of the data field in this message
	unsigned char length;
	//! The data
	unsigned char data[COMM_MESSAGE_DATA_LENGTH];
} comm_message_struct;

class CommInterface : public QThread {
public:
	CommInterface();
	int openPort(QString deviceName);
	bool isOpen();
	int closePort();
	void addTXMessage(unsigned char cmd, unsigned char length, unsigned char *data);
	void addTXMessage(unsigned char cmd, unsigned char data);
	void checkTXQueue();
	comm_message_struct getFirstInRXQueue();
	int getRXQueueCount();
private:
	bool threadActive;
	void checkRXMsg();
	QextSerialPort *serialPort;
protected:
	void sendMessage(char *data, int length);
	void sendMessage(QByteArray& data);
	void run();
	QByteArray rxedMessage;
	QLinkedList<QByteArray> txQueue;
	QLinkedList<QByteArray> rxQueue;
	bool messageAcked;
	void sendACK();
	void sendNACK();
};

#endif // COMMINTERFACE_H
