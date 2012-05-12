#include "tcpclass.h"
#include <QHostAddress>

TCPClass::TCPClass() {
	threadActive = false;
}

void TCPClass::startTransfer() {
	threadActive = true;
}

void TCPClass::connect(QString address, unsigned int port) {
	QHostAddress addr(address);
	client.connectToHost(addr, port);
}

bool TCPClass::isConnected() {
	return(client.isOpen());
}

void TCPClass::run() {
	threadActive = true;

	while(threadActive) {
		receiveMsg();

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

		rxMessage.clear();
		rxMessage.append(buff,buff.length());
	}
}

unsigned int TCPClass::rxQueueSize(void) {
	return(rxMessage.size());
}

QByteArray TCPClass::getMessage(void) {
	return(rxMessage);
}
