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

#ifndef SEQUENCERCLASS_H
#define SEQUENCERCLASS_H

#include <QSettings>
#include "commclass.h"

class SequencerClass
{
	public:
		SequencerClass();
		void writeSettings(QSettings& settings);
		void loadSettings(QSettings& settings);
		void sendSettings(CommClass& serialPort);
		void setName(QString name);
		void setRadioPreDelay(int value);
		void setRadioPostDelay(int value);
		void setAmpPreDelay(int value);
		void setAmpPostDelay(int value);
		void setInhibitPreDelay(int value);
		void setInhibitPostDelay(int value);
		void setAntennaPostDelay(int value);
		
		int getRadioPreDelay();
		int getRadioPostDelay();
		int getAmpPreDelay();
		int getAmpPostDelay();
		int getInhibitPreDelay();
		int getInhibitPostDelay();
		int getAntennaPostDelay();
		
		void setRadioPTTEnabled(bool state);
		void setAmpPTTEnabled(bool state);
		void setInhibitEnabled(bool state);
		bool getRadioPTTEnabled();
		bool getAmpPTTEnabled();
		bool getInhibitEnabled();
		
		bool getInputEnabled();
		void setInputEnabled(bool state);
		bool getInputInverted();
		void setInputInverted(bool state);
	private:
	
	protected:
		unsigned char radioPreDelay;
		unsigned char radioPostDelay;
		unsigned char ampPreDelay;
		unsigned char ampPostDelay;
		unsigned char inhibitPreDelay;
		unsigned char inhibitPostDelay;
		unsigned char antennaPostDelay;
		bool pttRadioEnabled;
		bool pttAmpEnabled;
		bool pttInhibitEnabled;
		bool inputEnabled;
		bool inputInverted;
		QString sequencerName;
};


#endif
