/***************************************************************************
 *   Copyright (C) 2009 by Mikael Larsmark, SM2WMV   *
 *   mike@sm3wmv.com   *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 *   This program is distributed in the hope that it will be useful,       *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
 *   GNU General Public License for more details.                          *
 *                                                                         *
 *   You should have received a copy of the GNU General Public License     *
 *   along with this program; if not, write to the                         *
 *   Free Software Foundation, Inc.,                                       *
 *   59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.             *
 ***************************************************************************/

#ifndef __COMMCLASS_H__
#define __COMMCLASS_H__

#include <QMainWindow>
#include <QThread>
#include <QLinkedList>
#include <QtNetwork>
#include <QString>
#include <QTcpSocket>

#include "qextserialport.h"
#include "commands.h"

//Must be set to the same value in the uC source
#define	COMM_INTERFACE_DATA_LENGTH	20

//! The timeout limit for transmission
#define COMM_INTERFACE_TX_TIMEOUT_LIMIT 20

#define COMM_INTERFACE_RX_TIMEOUT_LIMIT 5

#define COMM_INTERFACE_MAX_RESENDS  5

struct struct_message {
  unsigned char checksum;
  unsigned char cmd;
  unsigned char length;
	char data[COMM_INTERFACE_DATA_LENGTH];
};

//! The serial acknowledge of the computer communication protocol
#define COMM_CLASS_ACK				0xFA
//! The serial NOT acknowledge of the computer communication protocol
#define COMM_CLASS_NACK			0xFB
//! The communication preamble
#define COMM_CLASS_PREAMBLE   0xFE
//! The communication postamble
#define COMM_CLASS_POSTAMBLE  0xFD

class CommClass : public QThread {
	public:
		CommClass();
    int openPort(QString deviceName, BaudRateType baudrate);
		int closePort();
		void receiveMsg();
    void sendMessage(struct_message message);
		void addTXMessage(unsigned char cmd, unsigned char length, char *data);
		void addTXMessage(unsigned char cmd, char data);
    void pollTXQueue();
		void stopProcess();
		bool isOpen();
		int getRXQueueSize();
    struct_message getRXQueueFirst();
	private:
    void resetRXStatus();
    void checkTXQueue();
    void parseRXQueue();
    void sendACK();
		void sendNACK();
    unsigned char txMsgAcked;
    unsigned char resendCount;
    unsigned char txTimeout;
    unsigned char rxTimeout;
    unsigned char resendFlag;
	protected:
		bool threadActive;
		QextSerialPort *serialPort;
    QByteArray rxMessage;
    QLinkedList<struct_message> txQueue;
    QLinkedList<struct_message> rxQueue;
		void run();
};

#endif

