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

#ifndef COMMCLASS_H
#define COMMCLASS_H

#include <QThread>
#include <QLinkedList>
#include "qextserialport.h"

#define INTERFACE_TYPE_SERIAL		0
#define INTERFACE_TYPE_TCP			1

typedef struct {

} struct_comm_msg;

class CommClass : public QThread {

public:
	CommClass();
	void setInterfaceType(int type);
	void setNetworkDetails(int port, QString addr);
	void setSerialPort(QString port);
	int openPort();
	void stopProcess();
	bool isOpen();
	void receiveMsg();	
	QByteArray getFirstInQueue();
	int getRXQueueCount();
	bool messageAcked();
	void addTXMessage(QString msg);
	void sendText(QString text);
private:
	int interfaceType;
	bool threadActive;
	QextSerialPort *serialPort;
	QString serialPortName;
	int networkPort;
	QString networkIPAddress;
	QByteArray receivedMessage;
	QLinkedList<QByteArray> rxQueue;
	QLinkedList<QString> txQueue;
	bool lastMessageAcked;
protected:
	void run();
};

#endif // COMMCLASS_H
