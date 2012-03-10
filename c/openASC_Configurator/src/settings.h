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

		void setPowerMeterAddress(unsigned char band, unsigned char address);
		unsigned char getPowerMeterAddress(unsigned char band);
		void setPowerMeterVSWRAlarm(unsigned char band, float swr);
		float getPowerMeterVSWRAlarm(unsigned char band);
		void setPowerMeterRefPower(unsigned char band, unsigned int power);
		unsigned int getPowerMeterRefPower(unsigned char band);
		void setPowerMeterUpdateRateText(unsigned int rate);
		void setPowerMeterUpdateRateBargraph(unsigned int rate);
		unsigned int getPowerMeterUpdateRateText(void);
		unsigned int getPowerMeterUpdateRateBargraph(void);
		int getPowerMeterTextView();
		void setPowerMeterTextView(int value);
		void setPTTInterlockInput(unsigned char);
		unsigned char getPTTInterlockInput(void);
		bool getAmpFuncStatus(unsigned char index);
		void setAmpFuncStatus(unsigned char index, bool state);
		void setAmpControlEnabled(bool state);
		bool getAmpControlEnabled();
		void setAmpAddr(unsigned char addr);
		void setAmpSubAddr(unsigned char addr);
		unsigned char getAmpAddr();
		unsigned char getAmpSubAddr();
		unsigned char getAmpBandSegmentCount();
		void setAmpBandSegmentCount(unsigned char segments);
	private:
		int networkAddress;
		bool deviceIsMaster;
		int nrOfDevices;
		unsigned char powerMeterAddress[9];
		float powerMeterVSWRAlarmValue[9];
		unsigned int powerMeterRefPowerValue[9];
		unsigned int powerMeterUpdateRateText;
		unsigned int powerMeterUpdateRateBargraph;
		unsigned char pttInterlockInput; //0 = disabled, 1-7 -> ptt inputs
		bool ampControlEnabled;
		unsigned char ampAddr;
		unsigned char ampSubAddr;
		unsigned int ampFuncStatus;
		unsigned char ampBandSegmentCount;
	protected:
};

#endif
