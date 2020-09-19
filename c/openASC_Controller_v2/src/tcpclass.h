#ifndef TCPCLASS_H
#define TCPCLASS_H

#include <QtNetwork>
#include <QString>
#include <QTcpSocket>
#include <QObject>
#include <QHostAddress>
#include <QDebug>

#define TCP_BUF_LEN	2048

class TCPClass : public QObject {
Q_OBJECT
public:
    explicit TCPClass(QObject *parent);
    void connectToHost(QString address, unsigned int port);
    bool isConnected();
    QByteArray getMessage(void);
    unsigned int rxQueueSize(void);
    void addTXMessage(unsigned char cmd, unsigned char length, QByteArray data);
    void addTXMessage(unsigned char cmd, unsigned char length, QString data);
    void addTXMessage(unsigned char cmd, unsigned char data);
    void addTXMessage(unsigned char cmd);
    void stopConnection(void);
    void receiveMsg();
    bool transmitMsg();
private:
		unsigned int tcpPort;
        QTcpSocket *client;
		QByteArray rxMessage;
		QLinkedList<QByteArray> txQueue;
		QLinkedList<QByteArray> rxQueue;
        unsigned int rxTimeout;
protected:

};

#endif // TCPCLASS_H
