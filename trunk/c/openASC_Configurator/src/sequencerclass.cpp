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

#include <QMainWindow>
#include <qsettings.h>

#include "sequencerclass.h"

SequencerClass::SequencerClass() {
	radioPreDelay = 0;
	radioPostDelay = 0;
	ampPreDelay = 0;
	ampPostDelay = 0;
	inhibitPreDelay = 0;
	inhibitPostDelay = 0;
	antennaPostDelay = 0;
	pttRadioEnabled = false;
	pttAmpEnabled = false;
	pttInhibitEnabled = false;
	inputEnabled = false;
}

void SequencerClass::writeSettings(QSettings& settings) {
	settings.beginGroup(sequencerName);
		
		settings.setValue("radioPreDelay",radioPreDelay);
		settings.setValue("radioPostDelay",radioPostDelay);
		settings.setValue("ampPreDelay",ampPreDelay);
		settings.setValue("ampPostDelay",ampPostDelay);
		settings.setValue("inhibitPreDelay",inhibitPreDelay);
		settings.setValue("inhibitPostDelay",inhibitPostDelay);
		settings.setValue("antennaPostDelay",antennaPostDelay);
		settings.setValue("pttRadioEnabled",pttRadioEnabled);
		settings.setValue("pttAmpEnabled",pttAmpEnabled);
		settings.setValue("pttInhibitEnabled",pttInhibitEnabled);
		
		settings.setValue("InputEnabled",inputEnabled);
		settings.setValue("InputInverted",inputInverted);
		
	settings.endGroup();
}

void SequencerClass::loadSettings(QSettings& settings) {
	settings.beginGroup(sequencerName);
		
	radioPreDelay = settings.value("radioPreDelay").toInt();
	radioPostDelay = settings.value("radioPostDelay").toInt();
	ampPreDelay = settings.value("ampPreDelay").toInt();
	ampPostDelay = settings.value("ampPostDelay").toInt();
	inhibitPreDelay = settings.value("inhibitPreDelay").toInt();
	inhibitPostDelay = settings.value("inhibitPostDelay").toInt();
	antennaPostDelay = settings.value("antennaPostDelay").toInt();
	pttRadioEnabled = settings.value("pttRadioEnabled").toBool();
	pttAmpEnabled = settings.value("pttAmpEnabled").toBool();
	pttInhibitEnabled = settings.value("pttInhibitEnabled").toBool();
	
	inputEnabled = settings.value("InputEnabled").toBool();
	inputInverted = settings.value("InputInverted").toBool();

	settings.endGroup();	
}

void SequencerClass::sendSettings(CommClass& serialPort) {
	unsigned char tx_buff[10];

	if (sequencerName == "SequencerFootswitch") {
		tx_buff[0] = CTRL_SET_SEQUENCER_FOOTSWITCH;

		if (inputEnabled)
			tx_buff[9] = (1<<0);
	}
	else if (sequencerName == "SequencerComputer") {
		tx_buff[0] = CTRL_SET_SEQUENCER_COMPUTER;

		if (inputEnabled)
			tx_buff[9] = (1<<3);
		if (inputInverted)
			tx_buff[9] |= (1<<5);
	}
	else if (sequencerName == "SequencerRadioSense") {
		tx_buff[0] = CTRL_SET_SEQUENCER_RADIO_SENSE;

		if (inputEnabled)
			tx_buff[9] = (1<<1);
		if (inputInverted)
			tx_buff[9] |= (1<<4);
	}

	//TODO: Inhibit polarity setting

	tx_buff[1] = radioPreDelay;
	tx_buff[2] = radioPostDelay;
	tx_buff[3] = ampPreDelay;
	tx_buff[4] = ampPostDelay;
	tx_buff[5] = inhibitPreDelay;
	tx_buff[6] = inhibitPostDelay;
	tx_buff[7] = antennaPostDelay;

	if (pttRadioEnabled)
		tx_buff[8] = (1<<0);
	if (pttAmpEnabled)
		tx_buff[8]  |= (1<<1);
	if (pttInhibitEnabled)
		tx_buff[8]  |= (1<<2);


	serialPort.addTXMessage(CTRL_SET_SEQUENCER_SETTINGS,10,tx_buff);
}

void SequencerClass::setName(QString name) {
	sequencerName = name;
}

void SequencerClass::setRadioPreDelay(int value) {
	radioPreDelay = value / 10;
}

void SequencerClass::setRadioPostDelay(int value) {
	radioPostDelay = value / 10;
}

void SequencerClass::setAmpPreDelay(int value) {
	ampPreDelay = value / 10;
}

void SequencerClass::setAmpPostDelay(int value) {
	ampPostDelay = value / 10;
}

void SequencerClass::setInhibitPreDelay(int value) {
	inhibitPreDelay = value / 10;
}

void SequencerClass::setInhibitPostDelay(int value) {
	inhibitPostDelay = value / 10;
}

void SequencerClass::setAntennaPostDelay(int value) {
	antennaPostDelay = value / 10;
}

int SequencerClass::getRadioPreDelay() {
	return(radioPreDelay*10);
}

int SequencerClass::getRadioPostDelay() {
	return(radioPostDelay*10);
}

int SequencerClass::getAmpPreDelay() {
	return(ampPreDelay*10);
}

int SequencerClass::getAmpPostDelay() {
	return(ampPostDelay*10);
}

int SequencerClass::getInhibitPreDelay() {
	return(inhibitPreDelay*10);
}

int SequencerClass::getInhibitPostDelay() {
	return(inhibitPostDelay*10);
}

int SequencerClass::getAntennaPostDelay() {
	return(antennaPostDelay*10);
}

void SequencerClass::setRadioPTTEnabled(bool state) {
	pttRadioEnabled = state;
}

void SequencerClass::setAmpPTTEnabled(bool state) {
	pttAmpEnabled = state;
}

void SequencerClass::setInhibitEnabled(bool state) {
	pttInhibitEnabled = state;
}

bool SequencerClass::getRadioPTTEnabled() {
	return(pttRadioEnabled);
}

bool SequencerClass::getAmpPTTEnabled() {
	return(pttAmpEnabled);
}

bool SequencerClass::getInhibitEnabled() {
	return(pttInhibitEnabled);
}

bool SequencerClass::getInputEnabled() {
	return(inputEnabled);
}

void SequencerClass::setInputEnabled(bool state) {
	inputEnabled = state;
}

bool SequencerClass::getInputInverted() {
	return(inputInverted);
}

void SequencerClass::setInputInverted(bool state) {
	inputInverted = state;
}
