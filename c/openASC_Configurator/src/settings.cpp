#include "settings.h"

SettingsClass::SettingsClass() {
	//Set the default values
	networkAddress = 0x01;
	deviceIsMaster = false;
	nrOfDevices = 10;

	powerMeterAddress = 0;
	powerMeterVSWRAlarmValue = 0;
	powerMeterUpdateRateText = 0;
	powerMeterUpdateRateBargraph = 0;

	pttInterlockInput = 0;
}

void SettingsClass::writeSettings(QSettings& settings) {
	settings.beginGroup("Settings");

	settings.setValue("networkAddress",networkAddress);
	
	if (deviceIsMaster == true)
		settings.setValue("deviceIsMaster",1);
	else
		settings.setValue("deviceIsMaster",0);
	
	settings.setValue("nrOfDevices",nrOfDevices);
	
	settings.setValue("PowerMeterAddress",powerMeterAddress);
	settings.setValue("PowerMeterVSWRAlarmValue",powerMeterVSWRAlarmValue);
	settings.setValue("PowerMeterDisplayUpdateRateText",powerMeterUpdateRateText);
	settings.setValue("PowerMeterDisplayUpdateRateBargraph",powerMeterUpdateRateBargraph);

	settings.setValue("PTTInterlockInput",pttInterlockInput);

	settings.endGroup();
}

void SettingsClass::loadSettings(QSettings& settings) {
	settings.beginGroup("Settings");

	networkAddress = settings.value("networkAddress").toInt();
	
	if (settings.value("deviceIsMaster").toInt() == 1)
		deviceIsMaster = true;
	else
		deviceIsMaster = false;
	
	powerMeterAddress = settings.value("PowerMeterAddress").toInt();
	powerMeterVSWRAlarmValue = settings.value("PowerMeterVSWRAlarmValue").toDouble();
	powerMeterUpdateRateText = settings.value("PowerMeterDisplayUpdateRateText").toInt();
	powerMeterUpdateRateBargraph = settings.value("PowerMeterDisplayUpdateRateBargraph").toInt();

	pttInterlockInput = settings.value("PTTInterlockInput").toInt();

	nrOfDevices = settings.value("nrOfDevices").toInt();
	
	settings.endGroup();
}

void SettingsClass::sendSettings(CommClass& serialPort) {
	unsigned char tx_buff[8];
		
	tx_buff[0] = CTRL_SET_DEVICE_SETTINGS_NETWORK;
	tx_buff[1] = networkAddress;
	tx_buff[2] = nrOfDevices;

	if (deviceIsMaster)
		tx_buff[3] = 1;
	else
		tx_buff[3] = 0;
	
	serialPort.addTXMessage(CTRL_SET_DEVICE_SETTINGS, 4, tx_buff);

/*	tx_buff[0] = CTRL_SET_POWERMETER_SETTINGS;
	tx_buff[1] = powerMeterAddress;
	tx_buff[2] = ((unsigned int)(powerMeterVSWRAlarmValue * 100) & 0xFF00) >> 8;
	tx_buff[3] = (unsigned int)(powerMeterVSWRAlarmValue * 100) & 0x00FF;
	tx_buff[4] = (powerMeterUpdateRateText >> 8);
	tx_buff[5] = (powerMeterUpdateRateText & 0x00FF);
	tx_buff[6] = (powerMeterUpdateRateBargraph >> 8);
	tx_buff[7] = (powerMeterUpdateRateBargraph & 0x00FF);

	serialPort.addTXMessage(CTRL_SET_DEVICE_SETTINGS, 8, tx_buff);
*/
	tx_buff[0] = CTRL_SET_DEVICE_SETTINGS_OTHER;
	tx_buff[1] = pttInterlockInput;

	serialPort.addTXMessage(CTRL_SET_DEVICE_SETTINGS, 1, tx_buff);

	tx_buff[0] = CTRL_SET_DEVICE_SETTINGS_SAVE;
	serialPort.addTXMessage(CTRL_SET_DEVICE_SETTINGS, 1, tx_buff);
}

void SettingsClass::setPTTInterlockInput(unsigned char value) {
		pttInterlockInput = value;
}

unsigned char SettingsClass::getPTTInterlockInput(void) {
		return(pttInterlockInput);
}

void SettingsClass::setPowerMeterAddress(unsigned char address) {
		powerMeterAddress = address;
}

unsigned char SettingsClass::getPowerMeterAddress(void) {
		return(powerMeterAddress);
}

void SettingsClass::setPowerMeterVSWRAlarm(double swr) {
		powerMeterVSWRAlarmValue = swr;
}

double SettingsClass::getPowerMeterVSWRAlarm(void) {
		return(powerMeterVSWRAlarmValue);
}

void SettingsClass::setPowerMeterUpdateRateText(unsigned int rate) {
		powerMeterUpdateRateText = rate;
}

void SettingsClass::setPowerMeterUpdateRateBargraph(unsigned int rate) {
		powerMeterUpdateRateBargraph = rate;
}

unsigned int SettingsClass::getPowerMeterUpdateRateText(void) {
		return(powerMeterUpdateRateText);
}

unsigned int SettingsClass::getPowerMeterUpdateRateBargraph(void) {
		return(powerMeterUpdateRateBargraph);
}

void SettingsClass::setNetworkAddress(int value) {
	networkAddress = value;
}

int SettingsClass::getNetworkAddress() {
	return(networkAddress);
}

void SettingsClass::setDeviceIsMaster(bool state) {
	deviceIsMaster = state;
}

bool SettingsClass::getDeviceIsMaster() {
	return(deviceIsMaster);
}

void SettingsClass::setNumberOfDevices(int deviceCount) {
	nrOfDevices = deviceCount;
}

int SettingsClass::getNumberOfDevices() {
	return(nrOfDevices);
}
