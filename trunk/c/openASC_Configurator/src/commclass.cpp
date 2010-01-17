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

//#define DEBUG

CommClass::CommClass() {
	threadActive = false;
}

int CommClass::openPort(QString deviceName) {

	serialPort = new QextSerialPort(deviceName);
	serialPort->setBaudRate(BAUD115200);
	serialPort->setDataBits(DATA_8);
	serialPort->setFlowControl(FLOW_OFF);
	serialPort->setParity(PAR_NONE);
	serialPort->setStopBits(STOP_1);
	sent_count = 0;
	lastMessageAcked = true;
	
	//Sets the timeout between each character read to 50 us
	serialPort->setTimeout(0,10);
	
	if (serialPort->open(QIODevice::ReadWrite)) {
		serialPort->flush();
		return(0);
	} else {
		return(1);
	}

}

void CommClass::parseRXQueue() {
	QByteArray temp = rxQueue.first();
	rxQueue.removeFirst();
	
	unsigned char cmd = (unsigned char)temp.at(0);
	unsigned char length = (unsigned char)temp.at(1);
	unsigned char data[length+1];

	for (int i=0;i<length;i++)
		data[i] = temp.at(i+2);

	data[length] = 0;
	
	switch(cmd) {
		case CTRL_GET_FIRMWARE_REV:
			qDebug("Firmware rev: %s\n\r",data);
			
			if (txQueue.size() > 0)
				txQueue.removeFirst();
	
			lastMessageAcked = true;
			break;
		case COMPUTER_COMM_ACK:
			#ifdef DEBUG
				qDebug("ACKED");
			#endif
				if (txQueue.size() > 0)
					txQueue.removeFirst();
	
				lastMessageAcked = true;
			break;
		case COMPUTER_COMM_NACK:
			#ifdef DEBUG
				qDebug("NACK!!");
				exit(0);
			#endif
			break;
		case CTRL_DONE:
			if (txQueue.size() > 0)
				txQueue.removeFirst();
			
			lastMessageAcked = true;

			qDebug("DONE");
			break;
	}
}

void CommClass::run() {
	threadActive = true;
	
	while(threadActive) {
		receiveMsg();
		
		if (rxQueue.size() > 0)
			parseRXQueue();
		
		checkTXQueue();
		
		QThread::usleep(100);
	}
}

void CommClass::stopProcess() {
	threadActive = false;
}

int CommClass::closePort() {
	serialPort->close();
	
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
			if ((((unsigned char)receivedMessage.at(i)) == 0xFE) && (((unsigned char)receivedMessage.at(i)) == 0xFE)) {
				for (int x=i;x<receivedMessage.size();x++) {
					if (((unsigned char)receivedMessage.at(x) == 0xFD) && ((unsigned char)receivedMessage.at(i+2) == (x-i-3))) {
						rxQueue.append(receivedMessage.mid(i+1,(unsigned char)receivedMessage.at(i+2)+2));
/*						for (int x=0;x<receivedMessage.size();x++)
							qDebug("%i - 0x%02X ",x,(unsigned char)receivedMessage.at(x));*/
						
						receivedMessage.remove(i-1,(unsigned char)receivedMessage.at(i+2)+5);
						
						/*for (int x=0;x<receivedMessage.size();x++)
							qDebug("%i - 0x%02X ",x,(unsigned char)receivedMessage.at(x));
						*/
						return;
					}
				}
			}
	}
}

bool CommClass::isOpen() {
	return(serialPort->isOpen());
}

void CommClass::sendMessage(char *data, int length) {
	serialPort->write(data,length);
}

void CommClass::sendMessage(QByteArray& data) {
	#ifdef DEBUG
		for (int i=0;i<data.count();i++)
			qDebug("0x%02X ",data.at(i));
	#endif

	serialPort->write(data);
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
