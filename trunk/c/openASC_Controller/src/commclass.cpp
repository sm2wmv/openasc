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
#include "commclass.h"

//#define DEBUG	1

CommClass::CommClass() {
	threadActive = false;
}

int CommClass::openPort(QString deviceName) {
	serialPort = new QextSerialPort(deviceName);
	serialPort->setBaudRate(BAUD19200);
	serialPort->setDataBits(DATA_8);
	serialPort->setFlowControl(FLOW_OFF);
	serialPort->setParity(PAR_NONE);
	serialPort->setStopBits(STOP_1);

	sent_count = 0;

	lastMessageAcked = true;

	interfaceType = INTERFACE_TYPE_SERIAL;

	//Sets the timeout between each character read to 50 us
	serialPort->setTimeout(0,10);
	
	if (serialPort->open(QIODevice::ReadWrite)) {
		serialPort->flush();

		return(0);
	} else {
		return(1);
	}

}

int CommClass::openPort(QString address, quint16 port) {
	QHostAddress addr(address);
	client.connectToHost(addr, port);

	sent_count = 0;
	lastMessageAcked = true;
}

void CommClass::parseRXQueue() {

}

int CommClass::getRXQueueSize() {
	return(rxQueue.size());
}

QByteArray CommClass::getRXQueueFirst() {
	QByteArray temp = rxQueue.first();
	rxQueue.removeFirst();

	return(temp);
}

void CommClass::run() {
	threadActive = true;
	
	while(threadActive) {
		receiveMsg();

		checkTXQueue();
		
		QThread::usleep(100);
	}
}

void CommClass::stopProcess() {
	threadActive = false;
}

int CommClass::closePort() {
	if (interfaceType == INTERFACE_TYPE_SERIAL)
		serialPort->close();
	else
		client.close();
	
	return(0);
}

void CommClass::receiveMsg() {
	char buff[1024];
	int numBytes;
  
	numBytes = serialPort->bytesAvailable();
	
	if(numBytes > 0) {
		if(numBytes > 1024) 
			numBytes = 1024;
		
		int i = serialPort->read(buff, numBytes);
		
		receivedMessage.append(buff,i);
	}
	
	for (int i=0;i<receivedMessage.size();i++) {
		if (i>0)
			if ((((unsigned char)receivedMessage.at(i)) == 0xFE) && (((unsigned char)receivedMessage.at(i-1)) == 0xFE)) {
				for (int x=i;x<receivedMessage.size();x++) {
					if (((unsigned char)receivedMessage.at(x) == 0xFD) && ((unsigned char)receivedMessage.at(i+2) == (x-i-3))) {

						if ((unsigned char)receivedMessage.at(i+1) == COMPUTER_COMM_ACK) {
							if (txQueue.size() > 0)
								txQueue.removeFirst();

							lastMessageAcked = true;
						}
						else if ((unsigned char)receivedMessage.at(i+1) == COMPUTER_COMM_NACK) {
							qDebug("NACK");
						}
						else {
							rxQueue.append(receivedMessage.mid(i+1,(unsigned char)receivedMessage.at(i+2)+2));

							if (txQueue.size() > 0)
								txQueue.removeFirst();

							lastMessageAcked = true;
						}

						receivedMessage.remove(i-1,(unsigned char)receivedMessage.at(i+2)+5);
						
						return;
					}
				}
			}
	}
}

bool CommClass::isOpen() {
	return(serialPort->isOpen());
}

void CommClass::sendNACK() {
	char data[5];
	data[0] = (char)0xFE;
	data[1] = (char)0xFE;
	data[2] = (char)COMPUTER_COMM_NACK;
	data[3] = (char)0x00;
	data[4] = (char)0xFD;

	sendMessage(data,5);
}

void CommClass::sendACK() {
	char data[5];
	data[0] = (char)0xFE;
	data[1] = (char)0xFE;
	data[2] = (char)COMPUTER_COMM_ACK;
	data[3] = (char)0x00;
	data[4] = (char)0xFD;

	sendMessage(data,5);
}

void CommClass::sendMessage(char *data, int length) {
	if (interfaceType == INTERFACE_TYPE_SERIAL){
		serialPort->write(data,length);
	}
	else if (interfaceType == INTERFACE_TYPE_TCP) {
		client.write(data,length);
	}
}

void CommClass::sendMessage(QByteArray& data) {
	if (interfaceType == INTERFACE_TYPE_SERIAL) {
		serialPort->write(data);
	}
	else if (interfaceType == INTERFACE_TYPE_TCP) {
		client.write(data);
	}
}

void CommClass::addTXMessage(unsigned char cmd, unsigned char length, unsigned char *data) {
	QByteArray byteArray;
	byteArray.resize(5+length);

	byteArray[0] = 0xFE;
	byteArray[1] = 0xFE;
	byteArray[2] = cmd;
	byteArray[3] = length;
	
	for (int i=0;i<length;i++) {
		byteArray[i+4] = data[i];
	}
	
	//Add the postamble
	byteArray[4+length] = 0xFD;
	
	//Add the new message to the TX queue
	txQueue.append(byteArray);
}

void CommClass::addTXMessage(unsigned char cmd, unsigned char data) {
	QByteArray byteArray;
	byteArray.resize(6);

	byteArray[0] = 0xFE;
	byteArray[1] = 0xFE;
	byteArray[2] = cmd;
	byteArray[3] = 1;
	byteArray[4] = data;
	byteArray[5] = 0xFD;
	
	//Add the new message to the TX queue
	txQueue.append(byteArray);
}

void CommClass::checkTXQueue() {
	if ((txQueue.size() > 0) && (lastMessageAcked)) {
		lastMessageAcked = false;
		sendMessage(txQueue.first());
	}
}
