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
private:
		unsigned int tcpPort;
		QTcpSocket client;
		bool threadActive;
		void receiveMsg();
		QByteArray rxMessage;
protected:
		void run();
};

#endif // TCPCLASS_H
