#ifndef __SETTINGS_CLASS_H__
#define __SETTINGS_CLASS_H__

#include <qsettings.h>

#include "commclass.h"

class SettingsClass {
	public:
		SettingsClass();
		void writeSettings ( QSettings& settings );
		void loadSettings ( QSettings& settings );
		void sendSettings(CommClass& serialPort);
		
		void setNetworkAddress(int value);
		int getNetworkAddress();
		void setDeviceIsMaster(bool state);
		bool getDeviceIsMaster();
		void setNumberOfDevices(int deviceCount);
		int getNumberOfDevices();

		void setPowerMeterAddress(unsigned char address);
		unsigned char getPowerMeterAddress(void);
		void setPowerMeterVSWRAlarm(double swr);
		double getPowerMeterVSWRAlarm(void);
		void setPowerMeterUpdateRateText(unsigned int rate);
		void setPowerMeterUpdateRateBargraph(unsigned int rate);
		unsigned int getPowerMeterUpdateRateText(void);
		unsigned int getPowerMeterUpdateRateBargraph(void);
	private:
		int networkAddress;
		bool deviceIsMaster;
		int nrOfDevices;
		unsigned char powerMeterAddress;
		double powerMeterVSWRAlarmValue;
		unsigned int powerMeterUpdateRateText;
		unsigned int powerMeterUpdateRateBargraph;
	protected:
};

#endif
