#ifndef TCPCLASS_H
#define TCPCLASS_H

#include <QtNetwork>
#include <QObject>
#include <QString>
#include <QTcpSocket>
#include <QThread>

#define TCP_BUF_LEN	2048

//class TCPClass : public QThread {
class TCPClass {
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
    void receiveMsg();
    void transmitMsg();
private:
		unsigned int tcpPort;
		QTcpSocket client;
		bool threadActive;
		QByteArray rxMessage;
		QLinkedList<QByteArray> txQueue;
		QLinkedList<QByteArray> rxQueue;
    unsigned int rxTimeout;
protected:
    //void run();

public slots:

};

#endif // TCPCLASS_H
