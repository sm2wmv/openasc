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

#ifndef RADIOINTERFACECLASS_H
#define RADIOINTERFACECLASS_H

#include <QSettings>

//#include "mainwindowimpl.h"
#include "commclass.h"

class RadioInterfaceClass
{
	public:
		RadioInterfaceClass();
		
		void writeSettings(QSettings& settings);
		void loadSettings(QSettings& settings);
		void sendSettings(CommClass& serialPort);
		
		void setRadioType(unsigned char type);
		void setRadioMode(unsigned char mode);
		unsigned char getRadioType();
		unsigned char getRadioMode();
		void setBaudrate(unsigned char baudrate);
		unsigned char getBaudrate();
		void setStopbits(unsigned char stopbits);
		unsigned char getStopbits();
		void setCIVAddress(unsigned char address);
		unsigned char getCIVAddress();
		void setPollInterval(unsigned char interval);
		unsigned char getPollInterval();
		unsigned char setCATEnable(bool);
		bool getCATEnabled();
		void setSenseInput(unsigned char index);
		unsigned char getSenseInput();
	private:
	
	protected:
		unsigned char radioType;
		unsigned char radioMode;
		unsigned char radioBaudrate;
		unsigned char radioStopbits;
		unsigned char radioCIVAddress;
		unsigned char radioPollInterval;
		unsigned char radioCATEnabled;
		unsigned char radioSenseInput;
};

#endif
