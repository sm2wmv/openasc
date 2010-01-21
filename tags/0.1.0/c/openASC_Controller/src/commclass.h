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

#define INTERFACE_TYPE_SERIAL	1
#define INTERFACE_TYPE_TCP		2

//! The serial acknowledge of the computer communication protocol
#define COMPUTER_COMM_ACK				0xFA
//! The serial NOT acknowledge of the computer communication protocol
#define COMPUTER_COMM_NACK			0xFB

//! Command to force the openASC box into bootloader mode
#define COMPUTER_COMM_ENTER_BOOTLOADER		0x01
#define CTRL_REBOOT												0x02
#define CTRL_GET_FIRMWARE_REV							0x03

#define COMPUTER_COMM_REDIRECT_DATA			0x10

#define REMOTE_CONTROL_BUTTON_PRESSED		0x10
#define REMOTE_CONTROL_RX_ANT_TEXT			0x11
#define REMOTE_CONTROL_ANT_TEXT					0x12
#define REMOTE_CONTROL_ANT_STATUS				0x13
#define REMOTE_CONTROL_CHANGE_BAND			0x14

/* START OF BUTTON PRESSED DEFINES */
#define REMOTE_CTRL_BUTTON_TX1			1
#define REMOTE_CTRL_BUTTON_TX2			2
#define REMOTE_CTRL_BUTTON_TX3			3
#define REMOTE_CTRL_BUTTON_TX4			4

#define REMOTE_CTRL_BUTTON_RX1			5
#define REMOTE_CTRL_BUTTON_RX2			6
#define REMOTE_CTRL_BUTTON_RX3			7
#define REMOTE_CTRL_BUTTON_RX4			8

#define REMOTE_CTRL_BUTTON_RXANT1		9
#define REMOTE_CTRL_BUTTON_RXANT2		10
#define REMOTE_CTRL_BUTTON_RXANT3		11
#define REMOTE_CTRL_BUTTON_RXANT4		12
#define REMOTE_CTRL_BUTTON_RXANT5		13
#define REMOTE_CTRL_BUTTON_RXANT6		14
#define REMOTE_CTRL_BUTTON_RXANT7		15
#define REMOTE_CTRL_BUTTON_RXANT8		16
#define REMOTE_CTRL_BUTTON_RXANT9		17
#define REMOTE_CTRL_BUTTON_RXANT10	18

#define REMOTE_CTRL_BUTTON_TXRX_MODE		19
#define REMOTE_CTRL_BUTTON_RXANT				20
/* END OF BUTTON PRESSED DEFINES */

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
	private:
		int interfaceType;
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

