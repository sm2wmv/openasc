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
#include "comminterface.h"
#include "qextserialport.h"
#include "commands.h"

//! Keeps track of how long ago it was we sent a mesage
int txTimeout = 0;
//! Keeps track of how long ago we receieved a character
int rxTimeout = 0;

CommInterface::CommInterface() {
	threadActive = false;
	messageAcked = true;
}

int CommInterface::openPort(QString deviceName) {
	serialPort = new QextSerialPort(deviceName);
	serialPort->setBaudRate(BAUD19200);
	serialPort->setDataBits(DATA_8);
	serialPort->setFlowControl(FLOW_OFF);
	serialPort->setParity(PAR_NONE);
	serialPort->setStopBits(STOP_1);

	//Sets the timeout between each character read to 50 us
	serialPort->setTimeout(0,10);

	if (serialPort->open(QIODevice::ReadWrite)) {
		serialPort->flush();

		//Set that the thread actually starts to execute all its tasks
		threadActive = true;

		return(0);
	} else {
		return(1);
	}
}

bool CommInterface::isOpen() {
	return(serialPort->isOpen());
}

void CommInterface::run() {
	while(threadActive) {
		if (txTimeout > COMPUTER_COMM_TX_TIMEOUT_LIMIT) {
			//TODO: FIX TIMEOUT for TX
		}

		if (rxTimeout > COMPUTER_COMM_RX_TIMEOUT_LIMIT) {
			//TODO: FIX TIMEOUT for RX
			// FLUSH RX buffer?
		}

		txTimeout++;
		rxTimeout++;

		QThread::usleep(1000);
	}
}

int CommInterface::closePort() {
	serialPort->close();
	return(0);
}

void CommInterface::checkRXMsg() {
	char buff[1024];
	int numBytes;

	numBytes = serialPort->bytesAvailable();

	if(numBytes > 0) {
		if(numBytes > 1024)
			numBytes = 1024;

		int i = serialPort->read(buff, numBytes);

		rxedMessage.append(buff,i);
	}

	for (int i=0;i<rxedMessage.size();i++) {
		if (i>0)
			if ((((unsigned char)rxedMessage.at(i)) == COMPUTER_COMM_PREAMBLE) && (((unsigned char)rxedMessage.at(i-1)) == COMPUTER_COMM_PREAMBLE)) {
				for (int x=i;x<rxedMessage.size();x++) {
					if (((unsigned char)rxedMessage.at(x) == COMPUTER_COMM_POSTAMBLE) && ((unsigned char)rxedMessage.at(i+2) == (x-i-3))) {

						if ((unsigned char)rxedMessage.at(i+1) == COMPUTER_COMM_ACK) {
							if (txQueue.size() > 0)
								txQueue.removeFirst();

							messageAcked = true;
						}
						else if ((unsigned char)rxedMessage.at(i+1) == COMPUTER_COMM_NACK) {
							qDebug("NACK");
						}
						else {
							rxQueue.append(rxedMessage.mid(i+1,(unsigned char)rxedMessage.at(i+2)+2));

							if (txQueue.size() > 0)
								txQueue.removeFirst();

							messageAcked = true;
						}

						rxedMessage.remove(i-1,(unsigned char)rxedMessage.at(i+2)+5);

						return;
					}
				}
			}
	}
}

void CommInterface::sendNACK() {
	char data[5];
	data[0] = (char)0xFE;
	data[1] = (char)0xFE;
	data[2] = (char)COMPUTER_COMM_NACK;
	data[3] = (char)0x00;
	data[4] = (char)0xFD;

	sendMessage(data,5);
}

void CommInterface::sendACK() {
	char data[5];
	data[0] = (char)0xFE;
	data[1] = (char)0xFE;
	data[2] = (char)COMPUTER_COMM_ACK;
	data[3] = (char)0x00;
	data[4] = (char)0xFD;

	sendMessage(data,5);
}

void CommInterface::sendMessage(char *data, int length) {
	serialPort->write(data,length);
}

void CommInterface::sendMessage(QByteArray& data) {
	serialPort->write(data);
}

void CommInterface::addTXMessage(unsigned char cmd, unsigned char length, unsigned char *data) {
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

void CommInterface::addTXMessage(unsigned char cmd, unsigned char data) {
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

void CommInterface::checkTXQueue() {
	if ((txQueue.size() > 0) && (messageAcked)) {
		messageAcked = false;
		sendMessage(txQueue.first());
	}
}

comm_message_struct CommInterface::getFirstInRXQueue() {
	comm_message_struct message;

	QByteArray temp = rxQueue.first();

	message.checksum = (char)temp.at(0);
	message.command = (char)temp.at(1);
	message.length = (char)temp.at(2);

	for (int i=0;i<message.length;i++)
		message.data[i] = (char)temp.at(3+i);

return(message);
}

int CommInterface::getRXQueueCount() {
	return(rxQueue.size());
}
