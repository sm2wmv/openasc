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
	minHeadingList.clear();
	maxHeadingList.clear();
	nameList.clear();
	addressList.clear();
	delayList.clear();
}

void RotatorClass::writeSettings(QSettings& settings) {
	settings.beginGroup("RotatorSettings");
	
	settings.setValue("LastRotatorIndex",lastIndex);
	
	settings.beginWriteArray("Rotator");

	for (int i=0;i<getRotatorCount();i++){
		settings.setArrayIndex(i);
		settings.setValue("RotatorName",getRotatorName(i));
		settings.setValue("RotatorAddress",getRotatorAddress(i));
		settings.setValue("RotatorMinHeading",getRotatorMinHeading(i));
		settings.setValue("RotatorMaxHeading",getRotatorMaxHeading(i));
		settings.setValue("RotatorDelay",getRotatorDelay(i));
		settings.setValue("RotatorIndex",getRotatorIndex(i));
	}
		
	settings.endArray();

	settings.endGroup();
}

void RotatorClass::loadSettings(QSettings& settings) {
	addressList.clear();
	nameList.clear();
	minHeadingList.clear();
	maxHeadingList.clear();
	delayList.clear();
	indexList.clear();
	
	lastIndex = settings.value("LastRotatorIndex").toInt();
	
	settings.beginGroup("RotatorSettings");
	
	int size = settings.beginReadArray("Rotator");
	
	for (int i=0;i<size;i++) {
		settings.setArrayIndex(i);
		nameList.append(settings.value("RotatorName").toString());
		addressList.append(settings.value("RotatorAddress").toInt());
		minHeadingList.append(settings.value("RotatorMinHeading").toInt());
		maxHeadingList.append(settings.value("RotatorMaxHeading").toInt());
		delayList.append(settings.value("RotatorDelay").toInt());
		indexList.append(settings.value("RotatorIndex").toInt());
	}
	
	settings.endArray();

	settings.endGroup();
}

void RotatorClass::addRotator(QString rotatorName, unsigned char rotatorAddress, int rotatorMinHeading, int rotatorMaxHeading, unsigned char rotatorDelay) {
	nameList.append(rotatorName);
	addressList.append(rotatorAddress);
	minHeadingList.append(rotatorMinHeading);
	maxHeadingList.append(rotatorMaxHeading);
	delayList.append(rotatorDelay);
	indexList.append(++lastIndex);
}

unsigned char RotatorClass::changeRotatorProperties(int index, QString rotatorName, unsigned char rotatorAddress, int rotatorMinHeading, int rotatorMaxHeading, unsigned char rotatorDelay) {
	if (index > -1) {
		nameList.replace(index, rotatorName);	
		addressList.replace(index, rotatorAddress);
		minHeadingList.replace(index, rotatorMinHeading);
		maxHeadingList.replace(index, rotatorMaxHeading);
		delayList.replace(index, rotatorDelay);
	}
	else
		return(1);
	
	return(0);
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

int RotatorClass::getRotatorMinHeading(int index) {
	if (index < minHeadingList.count())
		return(minHeadingList.at(index));
	else
		return(0);
}

int RotatorClass::getRotatorMaxHeading(int index) {
	if (index < maxHeadingList.count())
		return(maxHeadingList.at(index));
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
		minHeadingList.removeAt(index);
		maxHeadingList.removeAt(index);
		delayList.removeAt(index);
		indexList.removeAt(index);
	}
}