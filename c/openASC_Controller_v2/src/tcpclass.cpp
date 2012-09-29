#include "tcpclass.h"
#include <QHostAddress>

QByteArray PREAMBLE;

TCPClass::TCPClass() {
	PREAMBLE.append(0xFE);
	PREAMBLE.append(0xFE);

	threadActive = false;
}

void TCPClass::startTransfer() {
	threadActive = true;
}

void TCPClass::connect(QString address, unsigned int port) {
	QHostAddress addr(address);
	client.connectToHost(addr, port);
}

void TCPClass::stopConnection(void) {
	client.abort();
}

bool TCPClass::isConnected() {
	return(client.isOpen());
}

void TCPClass::run() {
	threadActive = true;

	while(threadActive) {
		receiveMsg();
		transmitMsg();

		QThread::usleep(100);
	}
}

void TCPClass::receiveMsg() {
	QByteArray buff;
	unsigned int numBytes = client.bytesAvailable();

	if (numBytes > 0)
		qDebug() << "Bytes available: " << client.bytesAvailable();

	if (numBytes > 0) {
		if (numBytes > TCP_BUF_LEN)
			numBytes = TCP_BUF_LEN;

		 buff = client.read(TCP_BUF_LEN);

		//rxMessage.clear();
		rxMessage.append(buff,buff.length());

		int postamble_pos = -1;
		int preamble_pos = -1;
		int char_count = 0;

		while(char_count < rxMessage.size()) {
			preamble_pos = -1;

			preamble_pos = rxMessage.indexOf(PREAMBLE,0);
			int curr_pos = preamble_pos+5;

			if (preamble_pos != -1) {
				unsigned char data_size[2];

				data_size[0] = rxMessage.at(preamble_pos+3);
				data_size[1] = rxMessage.at(preamble_pos+4);
				int msg_size = (data_size[0] << 8) + data_size[1];

				while(curr_pos < rxMessage.size()) {
					if ((rxMessage.at(curr_pos) == (char)0xFD) && (msg_size == (curr_pos-5))) {
						postamble_pos = curr_pos;
						break;
					}

					curr_pos++;
				}

				if (postamble_pos != -1) {
					qDebug() << "Data size: " << msg_size;

					//The message seem to be of valid size
					if ((postamble_pos-preamble_pos - 5) == msg_size) {
						qDebug() << "DATA OK";

						rxQueue.append(rxMessage.mid(preamble_pos+2,preamble_pos+postamble_pos-2));
						rxMessage.remove(preamble_pos,postamble_pos-preamble_pos+1);
					}
				}

				qDebug() << "Size left: " << rxMessage.size();
				qDebug() << "Preamble POS " << preamble_pos;
				qDebug() << "Postamble POS " << postamble_pos;
			}
			else {
				//If we dont find any preamble in the data, then its corrupt and we clear the whole buffer
				rxMessage.clear();
			}

			char_count++;
		}
	}
}

unsigned int TCPClass::rxQueueSize(void) {
	return(rxQueue.size());
}

QByteArray TCPClass::getMessage(void) {
	return(rxQueue.takeFirst());
}

void TCPClass::transmitMsg() {
	if (txQueue.size() > 0) {
		client.write(txQueue.takeFirst());
	}
}

void TCPClass::addTXMessage(unsigned char cmd, unsigned char length, QByteArray data) {
	QByteArray temp;

	temp.append(0xFE);
	temp.append(0xFE);
	temp.append(cmd);
	temp.append(length);
	temp.append(data);
	temp.append(0xFD);

	txQueue.append(temp);
}

void TCPClass::addTXMessage(unsigned char cmd, unsigned char length, QString data) {
	QByteArray temp;

	temp.append(0xFE);
	temp.append(0xFE);
	temp.append(cmd);
	temp.append(length);
	temp.append(data);
	temp.append(0xFD);

	txQueue.append(temp);
}

void TCPClass::addTXMessage(unsigned char cmd, unsigned char data) {
	QByteArray temp;

	temp.append(0xFE);
	temp.append(0xFE);
	temp.append(cmd);
	temp.append(1);
	temp.append(data);
	temp.append(0xFD);

	txQueue.append(temp);
}

void TCPClass::addTXMessage(unsigned char cmd) {
	QByteArray temp;

	temp.append(0xFE);
	temp.append(0xFE);
	temp.append(cmd);
	temp.append((char)0);
	temp.append(0xFD);

	txQueue.append(temp);
}
