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

#define INTERFACE_TYPE_SERIAL	1
#define INTERFACE_TYPE_TCP		2

//! The serial acknowledge of the computer communication protocol
#define COMPUTER_COMM_ACK				0xFA
//! The serial NOT acknowledge of the computer communication protocol
#define COMPUTER_COMM_NACK			0xFB

class CommClass : public QThread
{
	public:
		CommClass();
		int openPort(QString deviceName);
		int openPort(QString address, quint16 port);
		int closePort();
		void receiveMsg();
		void sendMessage(char *data, int length);
		void sendMessage(QByteArray& data);
		void addTXMessage(unsigned char cmd, unsigned char length, unsigned char *data);
		void addTXMessage(unsigned char cmd, unsigned char data);
		void checkTXQueue();
		void stopProcess();
		void parseRXQueue();
		bool isOpen();
		int getRXQueueSize();
		QByteArray getRXQueueFirst();
	private:
		int interfaceType;
		void sendACK();
		void sendNACK();
	protected:
		bool threadActive;
		QextSerialPort *serialPort;
		QTcpSocket client;
		QByteArray receivedMessage;
		QLinkedList<QByteArray> txQueue;
		QLinkedList<QByteArray> rxQueue;
		bool lastMessageAcked;
		int sent_count;
		void run();
};

#endif

