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

#ifndef COMMCLASS_H
#define COMMCLASS_H

#include <QMainWindow>
#include <QThread>
#include <QLinkedList>

#include "qextserialport.h"

//! The serial acknowledge of the computer communication protocol
#define COMPUTER_COMM_ACK				0xFA
//! The serial NOT acknowledge of the computer communication protocol
#define COMPUTER_COMM_NACK			0xFB

//! Command to force the openASC box into bootloader mode
#define COMPUTER_COMM_ENTER_BOOTLOADER		0x01
#define CTRL_REBOOT												0x02
#define CTRL_GET_FIRMWARE_REV							0x03
//! This function just replies with the same command, this is so we can see when something has been finished
#define CTRL_DONE													0x04

#define CTRL_SET_TIME  										0x10
#define CTRL_SET_ANT_DATA									0x11
#define CTRL_CREATE_EEPROM_TABLE					0x12
#define CTRL_SET_RX_ANT_DATA							0x13
#define CTRL_SET_RADIO_SETTINGS						0x14
#define CTRL_SET_DEVICE_SETTINGS					0x15
#define CTRL_SET_BAND_DATA								0x16
#define CTRL_SET_EXT_INPUT								0x17

/* Defines for the radio settings */
#define CTRL_SET_RADIO_SETTINGS_SAVE		0x01
#define CTRL_SET_RADIO_SETTINGS_ALL			0x02

/* Defines for the antenna data */
#define CTRL_SET_ANT_DATA_SAVE						0x01
#define CTRL_SET_ANT_DATA_TEXT						0x02
#define CTRL_SET_ANT_DATA_SUB_MENU_TYPE		0x03
#define CTRL_SET_ANT_DATA_ANT_FLAGS				0x04
#define CTRL_SET_ANT_DATA_COMB_ALLOWED		0x05
#define CTRL_SET_ANT_DATA_ANT_OUT_STR			0x06

/* Defines for the band data */
#define CTRL_SET_BAND_DATA_LIMITS					0x01
#define CTRL_SET_BAND_DATA_LOW_OUT_STR		0x02
#define CTRL_SET_BAND_DATA_HIGH_OUT_STR		0x03
#define CTRL_SET_BAND_DATA_SAVE						0x07

/* Defines for the rx antenna data */
#define CTRL_SET_RX_ANT_DATA_TEXT						0x01
#define CTRL_SET_RX_ANT_DATA_ANT_OUT_STR		0x02
#define CTRL_SET_RX_ANT_DATA_BAND_OUT_STR		0x03
#define CTRL_SET_RX_ANT_DATA_SAVE						0x07

/* Defines for the device settings */
#define CTRL_SET_DEVICE_SETTINGS_ADDRESS						0x01
#define CTRL_SET_DEVICE_SETTINGS_NR_NODES						0x02
#define CTRL_SET_DEVICE_SETTINGS_DEVICE_IS_MASTER		0x03

/* bit 0 = PTTInputFootswitch
 * bit 1 = PTTInputRadioSense
 * bit 2 = PTTInputComputerRTS
 * bit 3 = PTTInputRadioSenseInverted
 * bit 4 = PTTInputComputerRTSInverted
 * bit 5 = InhibitPolarityActiveLow */
#define CTRL_SET_DEVICE_SETTINGS_PTT_INPUTS		0x04
#define CTRL_SET_DEVICE_SETTINGS_EXT_INPUTS		0x05
#define CTRL_SET_DEVICE_SETTINGS_SAVE					0x07

#define OUTPUT_ADDR_DELIMITER			0xFF

class CommClass : public QThread
{
	public:
		CommClass();
		int openPort(QString deviceName);
		int closePort();
		void receiveMsg();
		void sendMessage(char *data, int length);
		void sendMessage(QByteArray& data);
		void addTXMessage(unsigned char cmd, unsigned char length, unsigned char *data);
		void addTXMessage(unsigned char cmd, unsigned char data);
		void checkTXQueue();
		void stopProcess();
		void parseRXQueue();
		bool isOpen();
	private:
	
	protected:
		bool threadActive;
		QextSerialPort *serialPort;
		QByteArray receivedMessage;
		QLinkedList<QByteArray> txQueue;
		QLinkedList<QByteArray> rxQueue;
		bool lastMessageAcked;
		int sent_count;
		void run();
};

#endif