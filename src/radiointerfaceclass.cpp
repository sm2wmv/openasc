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

#include "radiointerfaceclass.h"

RadioInterfaceClass::RadioInterfaceClass() {
	radioType = 0;
	radioMode = 0;
	radioBaudrate = 1;
	radioStopbits = 0;
	radioCIVAddress = 0x04;
	radioPollInterval = 100;
}

void RadioInterfaceClass::writeSettings(QSettings& settings) {
	settings.beginGroup("RadioInterface");
		settings.setValue("radioType",radioType);
		settings.setValue("radioMode",radioMode);
		settings.setValue("radioBaudrate",radioBaudrate);
		settings.setValue("radioStopbits",radioStopbits);
		settings.setValue("radioCIVAddress",radioCIVAddress);
		settings.setValue("radioPollInterval",radioPollInterval);
	settings.endGroup();
}

void RadioInterfaceClass::loadSettings(QSettings& settings) {
	settings.beginGroup("RadioInterface");
		radioType = settings.value("radioType").toInt();
		radioMode = settings.value("radioMode").toInt();
		radioBaudrate = settings.value("radioBaudrate").toInt();
		radioStopbits = settings.value("radioStopbits").toInt();
		radioCIVAddress = settings.value("radioCIVAddress").toInt();
		radioPollInterval = settings.value("radioPollInterval").toInt();
	settings.endGroup();	
}

void RadioInterfaceClass::sendSettings(CommClass& serialPort) {
	unsigned char tx_buff[20];
	
	tx_buff[0] = CTRL_SET_RADIO_SETTINGS_ALL;
	tx_buff[1] = radioType;
	tx_buff[2] = radioMode;
	tx_buff[3] = radioBaudrate;
	tx_buff[4] = radioStopbits;
	tx_buff[5] = radioCIVAddress;
	tx_buff[6] = radioPollInterval;
	tx_buff[7] = 0;	//PTT MODE //TODO: Support this
	tx_buff[8] = 0; //PTT INPUT //TODO: Support this
	
	serialPort.addTXMessage(CTRL_SET_RADIO_SETTINGS,9,tx_buff);

	//Save the settings to the EEPROM
	tx_buff[0] = CTRL_SET_RADIO_SETTINGS_SAVE;
	serialPort.addTXMessage(CTRL_SET_RADIO_SETTINGS,1,tx_buff);
}

void RadioInterfaceClass::setRadioType(unsigned char type) {
	radioType = type;
}

void RadioInterfaceClass::setRadioMode(unsigned char mode) {
	radioMode = mode;
}

unsigned char RadioInterfaceClass::getRadioType() {
	return(radioType);
}

unsigned char RadioInterfaceClass::getRadioMode() {
	return(radioMode);
}

void RadioInterfaceClass::setBaudrate(unsigned char baudrate) {
	radioBaudrate = baudrate;
}

unsigned char RadioInterfaceClass::getBaudrate() {
	return(radioBaudrate);
}

void RadioInterfaceClass::setStopbits(unsigned char stopbits) {
	radioStopbits = stopbits;
}

unsigned char RadioInterfaceClass::getStopbits() {
	return(radioStopbits);
}

void RadioInterfaceClass::setCIVAddress(unsigned char address) {
	radioCIVAddress = address;
}

unsigned char RadioInterfaceClass::getCIVAddress() {
	return(radioCIVAddress);
}

void RadioInterfaceClass::setPollInterval(unsigned char interval) {
	radioPollInterval = interval;
}

unsigned char RadioInterfaceClass::getPollInterval() {
	return(radioPollInterval);
}
