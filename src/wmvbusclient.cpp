#include "wmvbusclient.h"

WMVBusClient::WMVBusClient() {
  threadActive = false;
  busAddr = 0;
}

void WMVBusClient::printDebugMessage(BUS_MESSAGE message) {
  qDebug("----------------------------");
  qDebug("from_addr: \t\t0x%02X",message.from_addr);
  qDebug("to_addr: \t\t0x%02X",message.to_addr);
  qDebug("checksum: \t\t0x%02X",message.checksum);
  qDebug("flags: \t\t0x%02X",message.flags);
  qDebug("cmd: \t\t0x%02X",message.cmd);
  qDebug("length: \t\t0x%02X",message.length);

  for (int i=0;i<message.length;i++)
      qDebug("data[%i]: \t\t0x%02X",i,message.data[i]);

  qDebug(" ");
}

/*! Init the WMV bus client
 *  \param addr The clients address
 *  \param servIP The IP we wish to connect to
 *  \param servPort The Port which we wish to connect to
 *  \param listener A pointer to a function which should be called when there is a message to be parsed from the bus */
 void WMVBusClient::initClient(unsigned char addr, QString servIP, unsigned int servPort) {
  busAddr = addr;
  serverIP = servIP;
  serverPort = servPort;

  receivedMessage.clear();

  openConnection(serverIP,serverPort);
}

/*! Retrieve the clients address on the bus
 *  \return The address of the client on the bus */
unsigned char WMVBusClient::getBusAddress() {
		return(busAddr);
}

/*! Add a message to the transmit QUEUE
 *  \param fromAddr From which address is the message sent
 *  \param toAddr To which address should the message be sent
 *  \param flags Flags for the message
 *  \param cmd The command that should be sent
 *  \param length The length of the data
 *  \param data The Data that should be sent */
void WMVBusClient::addTxMessage(unsigned char fromAddr, unsigned char toAddr, unsigned char flags, unsigned char cmd, unsigned char length, unsigned char data[]) {
  BUS_MESSAGE new_message;
  unsigned char checksum = 0;

  new_message.from_addr = fromAddr;
  new_message.to_addr = toAddr;
  new_message.flags = flags;
  checksum += flags;
  new_message.cmd = cmd;
  checksum += cmd;
  new_message.length = length;
  checksum += length;

  for (unsigned char i=0;i<length;i++) {
    new_message.data[i] = data[i];
    checksum += data[i];
  }

  new_message.checksum = checksum;

  //Add the message to the TX queue
  txQueue.enqueue(new_message);
}

/*!\brief Adds a message to the RX queue which will be sent as soon as possible
* \param fromAddr The address of the sender
* \param toAddr The address to the receiever
* \param flags Different flags, see defines
* \param cmd The command performed
* \param length The length of the data received
* \param data The data receieved */
void WMVBusClient::addRxMessage(unsigned char fromAddr, unsigned char toAddr, unsigned char flags, unsigned char cmd, unsigned char length, unsigned char data[]) {
  mutex.lock();

  BUS_MESSAGE new_message;

  new_message.from_addr = fromAddr;
  new_message.to_addr = toAddr;
  new_message.flags = flags;
  new_message.cmd = cmd;
  new_message.length = length;

  for (unsigned char i=0;i<length;i++)
      new_message.data[i] = data[i];

  //Add the message to the RX queue
  rxQueue.enqueue(new_message);

  mutex.unlock();
}

/*! Send an acknowledge message */
void WMVBusClient::sendAck(unsigned char toAddr) {
  addTxMessage(busAddr, toAddr, 0, BUS_CMD_ACK, 0, NULL);
}

/*! Send an NOT acknowledge message */
void WMVBusClient::sendNack(unsigned char toAddr) {
  addTxMessage(busAddr, toAddr, 0, BUS_CMD_NACK, 0, NULL);
}

void WMVBusClient::sendMessage(BUS_MESSAGE message) {
  //serialPort->write(data,length);
}

/*! Function will check if there is anything waiting to be parsed on the device */
void WMVBusClient::receiveMsg() {
  mutex.lock();

  long numBytes;

  rxTimeout++;

  if (rxTimeout > 10) {
    receivedMessage.clear();
    rxTimeout = 0;

    if (receivedMessage.length() > 0)
      qDebug("CLEAR");
  }

  //qDebug("Enter");

  numBytes = tcpClient.bytesAvailable();

  if (numBytes == -1)
    qDebug("ERROR RX DATA");

  //qDebug("Enter2");

  if(numBytes > 0) {
    //qDebug("Enter3");

    if(numBytes > 128)
      numBytes = 128;

    receivedMessage.append(tcpClient.read(numBytes));
//    /*receivedMessage.append((unsigned char)0xEF);
//    receivedMessage.append((unsigned char)0x80);
//    receivedMessage.append((unsigned char)0xFE);
//    receivedMessage.append((unsigned char)0xFE);
//    receivedMessage.append((unsigned char)0x05);
//    receivedMessage.append((unsigned char)0x06);
//    receivedMessage.append((unsigned char)0x48);		//Checksum
//    receivedMessage.append((unsigned char)0x01);
//    receivedMessage.append((unsigned char)0x20);
//    receivedMessage.append((unsigned char)0x02);
//    receivedMessage.append((unsigned char)0x12);
//    receivedMessage.append((unsigned char)0x13);
//    receivedMessage.append((unsigned char)0xFD);

//    receivedMessage.append((unsigned char)0xFE);
//    receivedMessage.append((unsigned char)0xFE);
//    receivedMessage.append((unsigned char)0x06);
//    receivedMessage.append((unsigned char)0x03);
//    receivedMessage.append((unsigned char)0x99);		//Checksum
//    receivedMessage.append((unsigned char)0x01);
//    receivedMessage.append((unsigned char)0x15);
//    receivedMessage.append((unsigned char)0x05);
//    receivedMessage.append((unsigned char)0x16);
//    receivedMessage.append((unsigned char)0x17);
//    receivedMessage.append((unsigned char)0x18);
//    receivedMessage.append((unsigned char)0x19);
//    receivedMessage.append((unsigned char)0x20);
//    receivedMessage.append((unsigned char)0xFD);

//    receivedMessage.append((unsigned char)0xFE);
//    receivedMessage.append((unsigned char)0xFE);
//    receivedMessage.append((unsigned char)0x02);
//    receivedMessage.append((char)0x00);
//    receivedMessage.append((unsigned char)0x03);


    int char_count = receivedMessage.size()-1;

    int last_preamb_pos = 0;

    //qDebug("bytes: %i %i",len,char_count);
    //qDebug("RX MESS: %i",receivedMessage.length());
    //qDebug("QUEUE size: %i",rxQueue.size());

    //Go through the characters and divide them into messages which are added
    //to the RX queue. If a SYNC message is receieved though we "reset" the counter at once
    while(char_count > 0) {
       if (char_count >= receivedMessage.length())
         qDebug("ERROR 1");

      if ((unsigned char)receivedMessage.at(char_count) == BUS_MSG_POSTAMBLE) {
        //Found preamble of this message, if there is one
        for (int i=char_count;i>0;i--) {
          //Check that this really is the preamble
          if (receivedMessage.length() < i+1)
            qDebug("ERROR 2");

          if (((unsigned char)receivedMessage.at(i) == BUS_MSG_PREAMBLE) && (i>0) && ((unsigned char)receivedMessage.at(i+1) == BUS_MSG_PREAMBLE)) {
            last_preamb_pos = i;

            BUS_MESSAGE new_message;
            new_message.from_addr = (unsigned char)receivedMessage.at(i+2);
            new_message.to_addr = (unsigned char)receivedMessage.at(i+3);
            new_message.checksum = (unsigned char)receivedMessage.at(i+4);
            new_message.flags = (unsigned char)receivedMessage.at(i+5);
            new_message.cmd = (unsigned char)receivedMessage.at(i+6);
            new_message.length = (unsigned char)receivedMessage.at(i+7);

            for (int x=0;x<new_message.length;x++) {
              if (receivedMessage.length() <= (i+8+x)) {
                qDebug("ERROR 3");
                break;
              }

              new_message.data[x] = (unsigned char)receivedMessage.at(i+8+x);
            }

            if (receivedMessage.length() <= (char_count))
              qDebug("ERROR 4");

            receivedMessage.remove(i+1,char_count-i+1);

            //printDebugMessage(new_message);
            rxQueue.append(new_message);

            char_count = i;
            break;
          }
        }
      }

     // qDebug("CHAR COUNT: %i",char_count);
      char_count--;
    }

    //If we have scrap characters in the beginning of the buffer we will remove those, the ones after the first POSTAMBLE found are saved
    //since they might be a part of next message
    if (last_preamb_pos < receivedMessage.length())
      receivedMessage.remove(0,last_preamb_pos);

    //qDebug("EXIT WHILE");
  }

  mutex.unlock();
}

void WMVBusClient::closeConnection() {
  tcpClient.close();
}

/*! Check if there is any message in the RX queue
 *  \return 1 if there is a message waiting, 0 if not */
int WMVBusClient::messageInRXQueue() {
  mutex.lock();
  unsigned char size = rxQueue.size();
  mutex.unlock();

  if (size > 0)
    return(1);

  return(0);
}

BUS_MESSAGE WMVBusClient::getMessageInRXQueue() {
  mutex.lock();

  BUS_MESSAGE retRxQueue = rxQueue.dequeue();

  mutex.unlock();

  return(retRxQueue);
}

/*void WMVBusClient::run() {
		threadActive = true;

		while(threadActive) {
        qDebug("T1");
				//Check to see if there are any characters waiting from the bus
        receiveMsg();
        qDebug("T2");
				//If there is a message in the queue, then we send it to the "Main Application" for it to be parsed
        //if (messageInRXQueue())
          //msgListener->recvBusMessage(getMessageInRXQueue());
        if (messageInRXQueue())
          getMessageInRXQueue();
        qDebug("T3");


        QThread::msleep(10);
		}
}*/

bool WMVBusClient::isConnected() {
  return(tcpClient.isOpen());
}

void WMVBusClient::stopProcess() {
	threadActive = false;
  closeConnection();
}

void WMVBusClient::openConnection(QString servIP, unsigned int servPort) {
  QHostAddress addr(servIP);
  tcpClient.connectToHost(addr, servPort);

  if(tcpClient.waitForConnected(5000)) {
    #ifdef DEBUG
      qDebug("DEBUG >> PowerMeter connected");
    #endif
  }
}
