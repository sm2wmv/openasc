/***************************************************************************
 *   Copyright (C) 2011 by Mikael Larsmark, SM2WMV   *
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

#include "commclass.h"

//! Character for enter
#define CHAR_ENTER 13
//! Character for space
#define CHAR_SPACE 32
//! Character for ESC
#define CHAR_ESC 27
//! Character for backspace
#define CHAR_BS 8

int txTimeout = 0;

CommClass::CommClass() {
	threadActive = false;
	lastMessageAcked = true;
}

/*! \brief Set the interface type of the function
 * \param type The type of the interface */
void CommClass::setInterfaceType(int type) {
	interfaceType = type;
}

/*! \brief Set the network details
 * \param port The TCP port to connect to
 * \param addr The network address */
void CommClass::setNetworkDetails(int port, QString addr) {
	networkPort = port;
	networkIPAddress = addr;
}

/*! \brief Set the serial port device name
 * \param port The name of the serial port device name */
void CommClass::setSerialPort(QString port) {
	serialPortName = port;
}

/*! \brief Opens the port you have configured and starts communication
		\return 0 if everything opens OK, 1 otherwise */
int CommClass::openPort() {
	if (interfaceType == INTERFACE_TYPE_SERIAL) {
		serialPort = new QextSerialPort(serialPortName);
		serialPort->setBaudRate(BAUD19200);
		serialPort->setDataBits(DATA_8);
		serialPort->setFlowControl(FLOW_OFF);
		serialPort->setParity(PAR_NONE);
		serialPort->setStopBits(STOP_1);

		//Sets the timeout between each character read to 50 us
		serialPort->setTimeout(0,10);

		if (serialPort->open(QIODevice::ReadWrite)) {
			serialPort->flush();
			return(0);
		}
		else
			return(1);

		receivedMessage.clear();
	}
	else if (interfaceType == INTERFACE_TYPE_TCP) {

	}
}

void CommClass::run() {
	threadActive = true;

	if (interfaceType == INTERFACE_TYPE_SERIAL) {
	}

	while(threadActive) {
		receiveMsg();

		if (lastMessageAcked) {
			if (txQueue.count() > 0) {
				sendText(txQueue.first());

				lastMessageAcked = false;
				txTimeout = 0;
			}
		}

		txTimeout++;

		if (txTimeout > 100) {
			qDebug("TX timeout");

			if (txQueue.count() > 0)
				txQueue.removeFirst();

			lastMessageAcked = true;

			txTimeout = 0;
		}

		QThread::msleep(1);
	}
}

void CommClass::stopProcess() {
	threadActive = false;
}

bool CommClass::isOpen() {
	return(serialPort->isOpen());
}

void CommClass::receiveMsg() {
	if (interfaceType == INTERFACE_TYPE_SERIAL) {
		char buff[1024];
		int numBytes;

		numBytes = serialPort->bytesAvailable();

		if(numBytes > 0) {
			if(numBytes > 1024)
				numBytes = 1024;

			int i = serialPort->read(buff, numBytes);

			receivedMessage.append(buff,i);
		}
	
		for (int i=0;i<receivedMessage.length();i++) {
			if (receivedMessage.at(i) == CHAR_ENTER) {
				rxQueue.append(receivedMessage.mid(0,i));
				receivedMessage.remove(0,i+1);
			}
		}
	}
}

void CommClass::sendText(QString text) {
	if (interfaceType == INTERFACE_TYPE_SERIAL) {
		QByteArray array(text.toAscii());

		serialPort->write(array);
	}
}

void CommClass::addTXMessage(QString msg) {
	if (msg.at(msg.length()-1) != 13)
		msg.append(13);

	txQueue.append(msg);
}

QByteArray CommClass::getFirstInQueue() {
	if (rxQueue.count() > 0) {
		QByteArray temp = rxQueue.first();
		rxQueue.removeFirst();

		return(temp);
	}

	return(NULL);
}

int CommClass::getRXQueueCount() {
	return(rxQueue.count());
}

bool CommClass::messageAcked() {
	if (txQueue.count() > 0)
		txQueue.removeFirst();

	lastMessageAcked = true;
}
