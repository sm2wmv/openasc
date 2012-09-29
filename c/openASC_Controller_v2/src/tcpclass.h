#ifndef TCPCLASS_H
#define TCPCLASS_H

#include <QtNetwork>
#include <QObject>
#include <QString>
#include <QTcpSocket>
#include <QThread>

#define TCP_BUF_LEN	2048

class TCPClass : public QThread {
public:
    TCPClass();
		void startTransfer();
		void connect(QString address, unsigned int port);
		bool isConnected();
		QByteArray getMessage(void);
		unsigned int rxQueueSize(void);
		void addTXMessage(unsigned char cmd, unsigned char length, QByteArray data);
		void addTXMessage(unsigned char cmd, unsigned char length, QString data);
		void addTXMessage(unsigned char cmd, unsigned char data);
		void addTXMessage(unsigned char cmd);
		void stopConnection(void);
private:
		unsigned int tcpPort;
		QTcpSocket client;
		bool threadActive;
		void receiveMsg();
		void transmitMsg();
		QByteArray rxMessage;
		QLinkedList<QByteArray> txQueue;
		QLinkedList<QByteArray> rxQueue;
protected:
		void run();
};

#endif // TCPCLASS_H
