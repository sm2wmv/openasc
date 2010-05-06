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

#include "rotatorclass.h"

RotatorClass::RotatorClass() {
	lastIndex = 0;
	startHeadingList.clear();
	degreesList.clear();
	nameList.clear();
	addressList.clear();
	delayList.clear();
	view360degList.clear();
}

void RotatorClass::writeSettings(QSettings& settings) {
	settings.beginGroup("RotatorSettings");
	
	settings.setValue("LastRotatorIndex",lastIndex);
	
	settings.beginWriteArray("Rotator");

	for (int i=0;i<getRotatorCount();i++){
		settings.setArrayIndex(i);
		settings.setValue("RotatorName",getRotatorName(i));
		settings.setValue("RotatorAddress",getRotatorAddress(i));
		settings.setValue("RotatorSubAddr",getRotatorSubAddress(i));
		settings.setValue("RotatorStartHeading",getRotatorStartHeading(i));
		settings.setValue("RotatorDegrees",getRotatorDegrees(i));
		settings.setValue("RotatorDelay",getRotatorDelay(i));
		settings.setValue("RotatorIndex",getRotatorIndex(i));
		settings.setValue("Rotator360degView",getRotator360degView(i));
	}
		
	settings.endArray();

	settings.endGroup();
}

void RotatorClass::loadSettings(QSettings& settings) {
	addressList.clear();
	nameList.clear();
	startHeadingList.clear();
	degreesList.clear();
	delayList.clear();
	indexList.clear();
	
	lastIndex = settings.value("LastRotatorIndex").toInt();
	
	settings.beginGroup("RotatorSettings");
	
	int size = settings.beginReadArray("Rotator");
	
	for (int i=0;i<size;i++) {
		settings.setArrayIndex(i);
		nameList.append(settings.value("RotatorName").toString());
		addressList.append(settings.value("RotatorAddress").toInt());
		subAddressList.append(settings.value("RotatorSubAddr").toInt());
		startHeadingList.append(settings.value("RotatorStartHeading").toInt());
		degreesList.append(settings.value("RotatorDegrees").toInt());
		delayList.append(settings.value("RotatorDelay").toInt());
		indexList.append(settings.value("RotatorIndex").toInt());
		view360degList.append(settings.value("Rotator360degView").toBool());
	}
	
	settings.endArray();

	settings.endGroup();
}

void RotatorClass::addRotator(QString rotatorName, unsigned char rotatorAddress, unsigned char subAddress, unsigned int rotatorStartHeading, unsigned int rotatorDegrees, unsigned char rotatorDelay, bool view360deg) {
	nameList.append(rotatorName);
	addressList.append(rotatorAddress);
	subAddressList.append(subAddress);
	startHeadingList.append(rotatorStartHeading);
	degreesList.append(rotatorDegrees);
	delayList.append(rotatorDelay);
	indexList.append(++lastIndex);
	view360degList.append(view360deg);
}

unsigned char RotatorClass::changeRotatorProperties(int index, QString rotatorName, unsigned char rotatorAddress, unsigned char subAddress, unsigned int rotatorStartHeading, unsigned int rotatorDegrees, unsigned char rotatorDelay, bool view360deg) {
	if (index > -1) {
		nameList.replace(index, rotatorName);	
		subAddressList.replace(index, subAddress);
		addressList.replace(index, rotatorAddress);
		startHeadingList.replace(index, rotatorStartHeading);
		degreesList.replace(index, rotatorDegrees);
		delayList.replace(index, rotatorDelay);
		view360degList.replace(index, view360deg);
	}
	else
		return(1);
	
	return(0);
}

bool RotatorClass::getRotator360degView(int index) {
	if (index < view360degList.count())
		return(view360degList.at(index));
	else
		return(false);
}

QString RotatorClass::getRotatorName(int index) {
	if (index < nameList.count())
		return(nameList.at(index));
	else
		return("SOMETHING WRONG!");
}

unsigned char RotatorClass::getRotatorAddress(int index) {
	if (index < addressList.count())
		return(addressList.at(index));
	else
		return(0);
}

unsigned char RotatorClass::getRotatorSubAddress(int index) {
	if (index < subAddressList.count())
		return(subAddressList.at(index));
	else
		return(0);
}

unsigned int RotatorClass::getRotatorStartHeading(int index) {
	if (index < startHeadingList.count())
		return(startHeadingList.at(index));
	else
		return(0);
}

unsigned int RotatorClass::getRotatorDegrees(int index) {
	if (index < degreesList.count())
		return(degreesList.at(index));
	else
		return(0);
}

unsigned char RotatorClass::getRotatorDelay(int index) {
	if (index < delayList.count())
		return(delayList.at(index));
	else
		return(0);
}

int RotatorClass::getRotatorIndex(int index) {
	if (index < indexList.count())
		return(indexList.at(index));
	else
		return(-1);
}

QStringList RotatorClass::getNameList() {
	return(nameList);
}

int RotatorClass::getRotatorCount() {
	return(nameList.count());
}

int RotatorClass::deleteRotator(int index) {
	if ((index > -1) && (index < nameList.count())) {
		nameList.removeAt(index);
		addressList.removeAt(index);
		startHeadingList.removeAt(index);
		degreesList.removeAt(index);
		delayList.removeAt(index);
		indexList.removeAt(index);
		view360degList.removeAt(index);
	}
}
