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
#include "commclass.h"
#include "mainwindowimpl.h"

unsigned int ackCount = 0;

CommClass::CommClass() {
  threadActive = false;
}

int CommClass::openPort(QString deviceName, BaudRateType baudrate) {
  serialPort = new QextSerialPort(deviceName);
  serialPort->setBaudRate(baudrate);
  serialPort->setDataBits(DATA_8);
  serialPort->setFlowControl(FLOW_OFF);
  serialPort->setParity(PAR_NONE);
  serialPort->setStopBits(STOP_1);

  //Sets the timeout between each character read to 50 us
  serialPort->setTimeout(0,10);

  txTimeout = 0;
  rxTimeout = 0;
  resendCount = 0;
  resendFlag = 0;
  txMsgAcked = 1;

  if (serialPort->open(QIODevice::ReadWrite)) {
    serialPort->flush();
    return(0);
  } else {
    return(1);
  }

}

void CommClass::parseRXQueue() {
}

void CommClass::run() {
  threadActive = true;

  while(threadActive) {
    receiveMsg();

    checkTXQueue();

    rxTimeout++;

    if (rxMessage.size() > 0) {
      if (rxTimeout > COMM_INTERFACE_RX_TIMEOUT_LIMIT) {
        rxMessage.clear();

        rxTimeout = 0;
      }
    }

    if (txMsgAcked == 0) {
      if (txTimeout > COMM_INTERFACE_TX_TIMEOUT_LIMIT) {
        resendFlag = 1;
        txTimeout = 0;
      }
      else
        txTimeout++;
    }

    QThread::usleep(1000);
  }
}

void CommClass::stopProcess() {
  threadActive = false;
}

int CommClass::closePort() {
  serialPort->close();

  return(0);
}

void CommClass::checkTXQueue() {
  if (txQueue.size() > 0) {
    if (txMsgAcked) {
      sendMessage(txQueue.first());

      resendFlag = 0;
      txMsgAcked = 0;
      resendCount = 0;
      txTimeout = 0;
    }
    else if (resendFlag) {
      if(resendCount < COMM_INTERFACE_MAX_RESENDS) {
				//qDebug("RESEND");
        if (txQueue.size() > 0)
          sendMessage(txQueue.first());

        resendCount++;
        txTimeout = 0;
      }
      else {
				//qDebug("I GIVE UP! DROPPING MESSAGE");
        if (txQueue.size() > 0)
          txQueue.removeFirst();

        txMsgAcked = 1;
        resendCount = 0;
        txTimeout = 0;
      }
    }
  }
}

int CommClass::getRXQueueSize() {
  return(rxQueue.size());
}

struct_message CommClass::getRXQueueFirst() {
  struct_message message = rxQueue.first();
  rxQueue.removeFirst();

  return(message);
}

void CommClass::resetRXStatus() {

}

void CommClass::sendACK() {
	//qDebug("SENDING ACK");

  struct_message message;

  message.checksum = COMM_CLASS_ACK;
  message.cmd = COMM_CLASS_ACK;
  message.length = 0;

  sendMessage(message);
}

void CommClass::sendNACK() {
  struct_message message;

  message.checksum = COMM_CLASS_NACK;
  message.cmd = COMM_CLASS_NACK;
  message.length = 0;

  sendMessage(message);
}

void CommClass::receiveMsg() {
  char buff[1024];
  unsigned char prev_char = 0;
  unsigned char curr_char = 0;
  unsigned char char_count = 0;
  unsigned char checksum = 0;
  int lastFoundIndex = 0;
  unsigned char preamble_found = 0;
  int numBytes;

  struct_message message;

  numBytes = serialPort->bytesAvailable();

  if(numBytes > 0) {
    rxTimeout = 0;

    if(numBytes > 1024)
      numBytes = 1024;

    int i = serialPort->read(buff, numBytes);

    rxMessage.append(buff,i);

/*    for (int i=0;i<rxMessage.size();i++)
      printf("CHAR: 0x%02X\n\r",(unsigned char)(rxMessage.at(i)));//qDebug() << "CHAR: " << i << ": " << rxMessage.at(i);

    qDebug() << "SIZE: " << rxMessage.size();*/
  }

  for (int i=0;i<rxMessage.size();i++) {
    curr_char = rxMessage.at(i);

    if (preamble_found) {
      if ((curr_char == COMM_CLASS_POSTAMBLE) && (message.length == (char_count - 3))){
				//qDebug("POSTAMBLE FOUND");
        if (checksum == message.checksum) {
					//qDebug("CHECKSUM OK");
          if (message.cmd == COMM_CLASS_ACK) {
					 // qDebug() << "ACK #" << ackCount++ << " RXED";
            if (txQueue.size() > 0)
              txQueue.removeFirst();  //Remove the first message in the queue

            txMsgAcked = 1;
            resendFlag = 0;
            resendCount = 0;
          }
          else if (message.cmd == COMM_CLASS_NACK) {
            resendFlag = 1;
						//qDebug("CHECKSUM FAIL");
          }
          else {
						//qDebug("ADDED MSG\n\r");

						//qDebug() << "CMD: " << message.cmd;
						//qDebug() << "LENGTH: " << message.length;
					 // for (unsigned char i=0;i<message.length;i++)
						 // qDebug() << "DATA[" << i << "]: " << message.data[i];

            rxQueue.append(message);
            sendACK();
          }

          checksum = 0;
          char_count = 0;
        }
        else {
					//qDebug("RX CHECKSUM FAIL");
          resendFlag = 1;
          resendCount = 0;
          lastFoundIndex = i;
        }

        lastFoundIndex = i;
      }
      else {
        if (char_count < (COMM_INTERFACE_DATA_LENGTH+3)) {
          switch(char_count) {
            case 0:
              message.checksum = curr_char;
              break;
          case 1:
              message.cmd = curr_char;
              checksum += curr_char;
              break;
          case 2:
              message.length = curr_char;
              checksum += curr_char;
              break;
          default:
              message.data[char_count-3] = curr_char;
              checksum += curr_char;
              break;
          }

          char_count++;
        }
        else {
          char_count = 0;
          checksum = 0;
          curr_char = 0;
        }
      }
    }
    else if ((prev_char == COMM_CLASS_PREAMBLE) && (curr_char == COMM_CLASS_PREAMBLE)) {
      preamble_found = 1;
      checksum = 0;
      char_count = 0;
    }

    prev_char = curr_char;


//      qDebug() << "Loop: " << i;
  }

  if (lastFoundIndex != 0)
    rxMessage.remove(0,lastFoundIndex+1);
}

bool CommClass::isOpen() {
  return(serialPort->isOpen());
}

void CommClass::sendMessage(struct_message message) {
  QByteArray txArray;

  txArray.append(COMM_CLASS_PREAMBLE);
  txArray.append(COMM_CLASS_PREAMBLE);
  txArray.append(message.checksum);
  txArray.append(message.cmd);
  txArray.append(message.length);

  for (unsigned char i=0;i<message.length;i++)
    txArray.append(message.data[i]);

  txArray.append(COMM_CLASS_POSTAMBLE);

  serialPort->write(txArray);
}

void CommClass::addTXMessage(unsigned char cmd, unsigned char length, char *data) {
  struct_message message;

  message.checksum = 0;
  message.cmd = cmd;
  message.checksum += cmd;
  message.length = length;
  message.checksum += length;

  for (unsigned char i=0;i<length;i++) {
    message.data[i] = data[i];
    message.checksum += data[i];
  }

  txQueue.append(message);
}

void CommClass::addTXMessage(unsigned char cmd, char data) {
  struct_message message;

  message.checksum = 0;
  message.cmd = cmd;
  message.checksum += cmd;
  message.length = 1;
  message.checksum += 1;

  message.data[0] = data;
  message.checksum += data;

  txQueue.append(message);
}
